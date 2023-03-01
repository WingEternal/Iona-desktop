#include "app/app_msg_handler.h"
#include "voice/wav_handler.h"
#include <cmath>
#include <cstdint>
//#include "iona_gl/l2d_utils.h"
#include <QFile>

using namespace IonaDesktop::CoreDisplay;

WavHandler::WavHandler(QObject* parent)
    : QObject(parent),
      _pcmData(NULL),
      _sampleOffset(0),
      _lastRms(0.0f),
      _userTimeSeconds(0.0f)
{
    _audio_output = new QAudioOutput(this);
    _audio_output->setVolume(0.8f);
    _player = new QMediaPlayer(this);
    _player->setAudioOutput(_audio_output);
}

WavHandler::~WavHandler()
{
    if (_pcmData != NULL)
        ReleasePcmData();
}

void WavHandler::register_ss()
{
    AppMsgHandler::getInstance().bindSlot("/voice/play", this, SLOT(play(const QUrl)));
    AppMsgHandler::getInstance().bindSlot("/voice/rms", this, SLOT(update(const float, float&)), Qt::BlockingQueuedConnection);
    AppMsgHandler::getInstance().regSignal("/voice/playback_state", _player, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)));
    connect(_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onPlayerMediaLoaded(const QMediaPlayer::MediaStatus)));
}

float WavHandler::getRms() const
{
    return _lastRms;
}

void WavHandler::update(const float deltaTimeSeconds, float& rms)
{
    rms = 0.0f;
    // Directly return if player is not playing
    if(this->_player->playbackState() != QMediaPlayer::PlayingState)
        return;

    uint32_t goalOffset;
    if ((_pcmData == NULL)
        || (_sampleOffset >= _wavFileInfo._samplesPerChannel))
    {
        _lastRms = 0.0f;
    }

    // 経過時間後の状態を保持
    _userTimeSeconds += deltaTimeSeconds;
    goalOffset = static_cast<uint32_t>(_userTimeSeconds * _wavFileInfo._samplingRate);
    if (goalOffset > _wavFileInfo._samplesPerChannel)
    {
        goalOffset = _wavFileInfo._samplesPerChannel;
    }

    // RMS計測
    for (uint32_t channelCount = 0; channelCount < _wavFileInfo._numberOfChannels; channelCount++)
    {
        for (uint32_t sampleCount = _sampleOffset; sampleCount < goalOffset; sampleCount++)
        {
            float pcm = _pcmData[channelCount][sampleCount];
            rms += pcm * pcm;
        }
    }
    rms = sqrt(rms / (_wavFileInfo._numberOfChannels * (goalOffset - _sampleOffset)));

    _lastRms = rms;
    _sampleOffset = goalOffset;
}

void WavHandler::play(const QUrl filePath)
{
    // WAVファイルのロード
    _player->setSource(filePath);
    if(!LoadWavFile(filePath.toLocalFile()))
        return;
}

bool WavHandler::LoadWavFile(const QString& filePath)
{
    bool ret;

    // 既にwavファイルロード済みならば領域開放
    if (_pcmData != NULL)
        ReleasePcmData();

    // ファイルロード
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray byteArray = file.readAll();
        _byteReader._fileSize = byteArray.size();
        char* buffer = new char[_byteReader._fileSize];
        memcpy(buffer, byteArray.data(), _byteReader._fileSize);
        _byteReader._fileByte = reinterpret_cast<unsigned char*>(buffer);
    }
    else
    {
        _byteReader._fileSize = 0;
        _byteReader._fileByte = nullptr;
        qDebug() << "[WavHandler] Fail reading " << filePath;
    }
    _byteReader._readOffset = 0;

    // ファイルロードに失敗しているか、先頭のシグネチャ"RIFF"を入れるサイズもない場合は失敗
    if ((_byteReader._fileByte == NULL) || (_byteReader._fileSize < 4))
        return false;

    // ファイル名
    _wavFileInfo._fileName = filePath;

    do {
        // シグネチャ "RIFF"
        if (!_byteReader.GetCheckSignature("RIFF"))
        {
            ret = false;
            break;
        }
        // ファイルサイズ-8（読み飛ばし）
        _byteReader.Get32LittleEndian();
        // シグネチャ "WAVE"
        if (!_byteReader.GetCheckSignature("WAVE"))
        {
            ret = false;
            break;
        }
        // シグネチャ "fmt "
        if (!_byteReader.GetCheckSignature("fmt "))
        {
            ret = false;
            break;
        }
        // fmtチャンクサイズ
        const uint32_t fmtChunkSize = _byteReader.Get32LittleEndian();
        // フォーマットIDは1（リニアPCM）以外受け付けない
        if (_byteReader.Get16LittleEndian() != 1)
        {
            ret = false;
            break;
        }
        // チャンネル数
        _wavFileInfo._numberOfChannels = _byteReader.Get16LittleEndian();
        // サンプリングレート
        _wavFileInfo._samplingRate = _byteReader.Get32LittleEndian();
        // データ速度[byte/sec]（読み飛ばし）
        _byteReader.Get32LittleEndian();
        // ブロックサイズ（読み飛ばし）
        _byteReader.Get16LittleEndian();
        // 量子化ビット数
        _wavFileInfo._bitsPerSample = _byteReader.Get16LittleEndian();
        // fmtチャンクの拡張部分の読み飛ばし
        if (fmtChunkSize > 16)
        {
            _byteReader._readOffset += (fmtChunkSize - 16);
        }
        // "data"チャンクが出現するまで読み飛ばし
        while (!(_byteReader.GetCheckSignature("data"))
            && (_byteReader._readOffset < _byteReader._fileSize))
        {
            _byteReader._readOffset += _byteReader.Get32LittleEndian();
        }
        // ファイル内に"data"チャンクが出現しなかった
        if (_byteReader._readOffset >= _byteReader._fileSize)
        {
            ret = false;
            break;
        }
        // サンプル数
        {
            const uint32_t dataChunkSize = _byteReader.Get32LittleEndian();
            _wavFileInfo._samplesPerChannel = (dataChunkSize * 8) / (_wavFileInfo._bitsPerSample * _wavFileInfo._numberOfChannels);
        }
        // 領域確保
        _pcmData = static_cast<float**>(malloc(sizeof(float*) * _wavFileInfo._numberOfChannels));
        for (uint32_t channelCount = 0; channelCount < _wavFileInfo._numberOfChannels; channelCount++)
            _pcmData[channelCount] = static_cast<float*>(malloc(sizeof(float) * _wavFileInfo._samplesPerChannel));
        // 波形データ取得
        for (uint32_t sampleCount = 0; sampleCount < _wavFileInfo._samplesPerChannel; sampleCount++)
        {
            for (uint32_t channelCount = 0; channelCount < _wavFileInfo._numberOfChannels; channelCount++)
                _pcmData[channelCount][sampleCount] = GetPcmSample();
        }

        ret = true;

    }  while (false);

    // ファイル開放
    delete[] _byteReader._fileByte;
    _byteReader._fileByte = NULL;
    _byteReader._fileSize = 0;

    return ret;
}

float WavHandler::GetPcmSample()
{
    int32_t pcm32;

    // 32ビット幅に拡張してから-1～1の範囲に丸める
    switch (_wavFileInfo._bitsPerSample)
    {
    case 8:
        pcm32 = static_cast<int32_t>(_byteReader.Get8()) - 128;
        pcm32 <<= 24;
        break;
    case 16:
        pcm32 = _byteReader.Get16LittleEndian() << 16;
        break;
    case 24:
        pcm32 = _byteReader.Get24LittleEndian() << 8;
        break;
    default:
        // 対応していないビット幅
        pcm32 = 0;
        break;
    }

    return static_cast<float>(pcm32) / INT32_MAX;
}

void WavHandler::ReleasePcmData()
{
    for (uint32_t channelCount = 0; channelCount < _wavFileInfo._numberOfChannels; channelCount++)
    {
        free(_pcmData[channelCount]);
    }
    free(_pcmData);
    _pcmData = NULL;
}

void WavHandler::onPlayerMediaLoaded(const QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::LoadedMedia)
    {
        _sampleOffset = 0;
        _userTimeSeconds = 0.0f;
        _lastRms = 0.0f;
        _player->play();
    }
}
