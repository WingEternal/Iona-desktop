#ifndef IONADESKTOP_CORE_DISPLAY_WAV_HANDLER_H
#define IONADESKTOP_CORE_DISPLAY_WAV_HANDLER_H

#include <QObject>
#include <QString>
#include <QAudioOutput>
#include <QMediaPlayer>

namespace IonaDesktop {
namespace CoreDisplay {
    class WavHandler : public QObject
    {
        Q_OBJECT
    public:
        explicit WavHandler(QObject* parent = nullptr);
        ~WavHandler();
        void register_ss();
        float getRms() const;

    public slots:
        void play(const QUrl filePath);
        void update(const float deltaTimeSeconds, float& rms);
        
    private:
        bool LoadWavFile(const QString& filePath);
        void ReleasePcmData();
        float GetPcmSample();

        struct WavFileInfo
        {
            WavFileInfo() : _fileName(""), _numberOfChannels(0),
                _bitsPerSample(0), _samplingRate(0), _samplesPerChannel(0) {}
            QString _fileName; ///< ファイル名
            uint32_t _numberOfChannels; ///< チャンネル数
            uint32_t _bitsPerSample; ///< サンプルあたりビット数
            uint32_t _samplingRate; ///< サンプリングレート
            uint32_t _samplesPerChannel; ///< 1チャンネルあたり総サンプル数
        } _wavFileInfo;

        struct ByteReader {
            ByteReader() : _fileByte(NULL), _fileSize(0), _readOffset(0) {}
            uint8_t Get8()
            {
                const uint8_t ret = _fileByte[_readOffset];
                _readOffset++;
                return ret;
            }
            uint16_t Get16LittleEndian()
            {
                const uint16_t ret = (_fileByte[_readOffset + 1] << 8) | _fileByte[_readOffset];
                _readOffset += 2;
                return ret;
            }
            uint32_t Get24LittleEndian()
            {
                const uint32_t ret =
                    (_fileByte[_readOffset + 2] << 16) | (_fileByte[_readOffset + 1] << 8)
                    | _fileByte[_readOffset];
                _readOffset += 3;
                return ret;
            }
            uint32_t Get32LittleEndian()
            {
                const uint32_t ret =
                    (_fileByte[_readOffset + 3] << 24) | (_fileByte[_readOffset + 2] << 16)
                    | (_fileByte[_readOffset + 1] << 8) | _fileByte[_readOffset];
                _readOffset += 4;
                return ret;
            }
            bool GetCheckSignature(const QString& reference)
            {
                char getSignature[4] = { 0, 0, 0, 0 };
                if (reference.length() != 4)
                {
                    return false;
                }
                for (uint32_t signatureOffset = 0; signatureOffset < 4; signatureOffset++)
                {
                    getSignature[signatureOffset] = static_cast<char>(Get8());
                }
                return (getSignature[0] == reference[0]) && (getSignature[1] == reference[1])
                    && (getSignature[2] == reference[2]) && (getSignature[3] == reference[3]);
            }

            unsigned char* _fileByte; ///< ロードしたファイルのバイト列
            unsigned int _fileSize; ///< ファイルサイズ
            uint32_t _readOffset; ///< ファイル参照位置
        } _byteReader;

        float** _pcmData; ///< -1から1の範囲で表現された音声データ配列
        uint32_t _sampleOffset; ///< サンプル参照位置
        float _lastRms; ///< 最後に計測したRMS値
        float _userTimeSeconds; ///< デルタ時間の積算値[秒]

        QAudioOutput* _audio_output;
        QMediaPlayer* _player;

    private slots:
        void onPlayerMediaLoaded(const QMediaPlayer::MediaStatus);
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_WAV_HANDLER_H
