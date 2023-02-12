#include "voice/voice_label.h"
#include "app/app_msg_handle.h"
#include <QTimer>

namespace {
    struct MediaInfo
    {
        QUrl url;
        QString text;
    };
    const MediaInfo start_info[1] = {
        {QUrl("qrc:/charater/voice/Iona01.mp3"),
         "潜水艦イ401、イオナ。\n急速せんこ～。"}
    };
    const MediaInfo random_info[3] = {
        {QUrl("qrc:/charater/voice/Iona02.mp3"),
         "どうしてここにいるのが分かったの？提督。何か用？"},
        {QUrl("qrc:/charater/voice/Iona03.mp3"),
        "提督と一緒に戦えて嬉しい。そうは見えないかもしれないけど。"},
        {QUrl("qrc:/charater/voice/Iona03.mp3"),
        "提督は潜水艦が珍しい？二人きりになるとお腹を見たがるのは何故？"}
    };
    const MediaInfo chime_info[24] = {
        {QUrl("qrc:/charater/voice/0000.mp3"),
         "マルマルマルマル。日付が変わった。新しい一日の始まり。"},
        {QUrl("qrc:/charater/voice/0100.mp3"),
         "マルヒトマルマル。提督が夜更かしするなら、私も付き合う。"},
        {QUrl("qrc:/charater/voice/0200.mp3"),
         "もしかして、私が寝るのを待っている？マルフタマルマル。提督、一体何をしているの？"},
        {QUrl("qrc:/charater/voice/0300.mp3"),
         "マルサンマルマル。この時間に食事をとると、人間は太るらしい。"},
        {QUrl("qrc:/charater/voice/0400.mp3"),
         "「おはよう」？それとも「おやすみ」？マルヨンマルマル。夜が一番深い時間。"},
        {QUrl("qrc:/charater/voice/0500.mp3"),
         "マルゴーマルマル。提督、私と一緒にコーヒーを飲む？"},
        {QUrl("qrc:/charater/voice/0600.mp3"),
         "マルロクマルマル。艦内照明を夜間モードから昼間モードに切り替える。"},
        {QUrl("qrc:/charater/voice/0700.mp3"),
         "マルナナマルマル。人間の営みが動き出す。今日の天気は、多分晴れ。"},
        {QUrl("qrc:/charater/voice/0800.mp3"),
         "マルハチマルマル。作戦行動開始。まずは、今日見るアニメの録画予約をセットして。"},
        {QUrl("qrc:/charater/voice/0900.mp3"),
         "マルキューマルマル。挨拶の言葉は、まだ「おはよう」？それとも「こんにちは」？"},
        {QUrl("qrc:/charater/voice/1000.mp3"),
         "ヒトマルマルマル。朝でも昼でもない中途半端な時間。中途半端って、何？"},
        {QUrl("qrc:/charater/voice/1100.mp3"),
         "午前のティータイム、ヒトヒトマルマル。私はコンゴウと違って、紅茶よりも甘いココアが好き。"},
        {QUrl("qrc:/charater/voice/1200.mp3"),
         "ヒトフタマルマル。ランチの時間、今日のメニューを教えて。"},
        {QUrl("qrc:/charater/voice/1300.mp3"),
         "ヒトサンマルマル。急速浮上して、甲板で日向ぼっこ。"},
        {QUrl("qrc:/charater/voice/1400.mp3"),
         "気持ちいい海風。このまま一日が過ぎていくのも悪くない…あ、ヒトヨンマルマル。"},
        {QUrl("qrc:/charater/voice/1500.mp3"),
         "甘いお菓子でちょっと休憩。ヒトゴーマルマルは、おやつの時間。"},
        {QUrl("qrc:/charater/voice/1600.mp3"),
         "ヒトロクマルマル。午後の4時。提督、もう一頑張りヨーソロー。"},
        {QUrl("qrc:/charater/voice/1700.mp3"),
         "ヒトナナマルマル。浮上して一緒に夕焼けを見てくれる？提督。"},
        {QUrl("qrc:/charater/voice/1800.mp3"),
        "ヒトハチマルマル。提督も手伝って。晩御飯の仕度を始める。"},
        {QUrl("qrc:/charater/voice/1900.mp3"),
        "ヒトキューマルマル。海の底から美味しそうな匂い。今夜のメニューは魚雷型コロッケ。"},
        {QUrl("qrc:/charater/voice/2000.mp3"),
        "フタマルマルマル。デザートは別腹。でも、メンタルモデルはいくら食べても太らない。"},
        {QUrl("qrc:/charater/voice/2100.mp3"),
        "フタヒトマルマル。提督が頭を洗ってくれるなら、お風呂に入ってもいいけど。"},
        {QUrl("qrc:/charater/voice/2200.mp3"),
        "フタフタマルマル。見ちゃダメ、提督。日記をつけてるんだから。もし見たら、全力でぶっ飛ばす。"},
        {QUrl("qrc:/charater/voice/2300.mp3"),
         "フタサンマルマル。長い一日の終わりの時間。心配しなくてもいい。提督が眠るまで一緒にいてあげる。"}
    };
}

using namespace IonaDesktop::CoreDisplay;

VoiceLabel::VoiceLabel(QWidget *parent) : QLabel(parent)
{
    voice_timer = new QTimer(this);
    media_thrd = new QThread(this);
    voice_player = new QMediaPlayer;
    voice_player->moveToThread(media_thrd);
    audio_output = new QAudioOutput(this);
    voice_player->setAudioOutput(audio_output);
    connect(voice_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onPlayerMediaStatusChanged(const QMediaPlayer::MediaStatus)));
    connect(this, SIGNAL(playMedia()), voice_player, SLOT(play()));
    connect(media_thrd, &QThread::finished, voice_player, &QObject::deleteLater);
    media_thrd->start();

    this->setStyleSheet("font-family:微软雅黑; font-size:16px; color: black; background-color:rgba(255, 255, 255, 180); border-radius:15px;");
    this->setWordWrap(true);
    this->setAlignment(Qt::AlignCenter);
    this->setGeometry(250, 265, 220, 60);

    // Animation effects
    label_opacity_fx = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(label_opacity_fx);
    this->setWindowOpacity(0);
    label_animation = new QPropertyAnimation(label_opacity_fx, "opacity", this);
    label_animation->setDuration(label_animate_duration);
    label_animation->setStartValue(0);
    label_animation->setEndValue(1);
    label_animation->setEasingCurve(QEasingCurve::Linear);

    AppMsgHandler::getInstance().bindSlot("/hitbox_hit", this, SLOT(playRandomVoice()));
    connect(voice_timer, SIGNAL(timeout()), this, SLOT(onChimeTimerTimeout()));
    connect(voice_player, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)),
        this, SLOT(onPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState)));
    connect(label_animation, SIGNAL(finished()), this, SLOT(onAnimationFinished()));

    // Play startup voice
    voice_player->setSource(start_info[0].url);
    audio_output->setVolume(15);
    this->setText(start_info[0].text);
    voice_timer->start(500);
}

VoiceLabel::~VoiceLabel()
{
    media_thrd->quit();
}

void VoiceLabel::playRandomVoice()
{
    qDebug() << "Judge: single click";
    if(voice_player->playbackState() != QMediaPlayer::StoppedState)
        voice_player->stop();
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int rand_index = rand() % 3;
    voice_player->setSource(random_info[rand_index].url);
    this->setText(random_info[rand_index].text);
}

void VoiceLabel::onChimeTimerTimeout()
{
    QTime curr_time = QTime::currentTime();
    if(curr_time.minute() == 0 && curr_time.second() == 0
        && voice_player->playbackState() == QMediaPlayer::StoppedState)
    {
        voice_player->setSource(chime_info[curr_time.hour()].url);
        this->setText(chime_info[curr_time.hour()].text);
    }
}

void VoiceLabel::onPlayerMediaStatusChanged(const QMediaPlayer::MediaStatus status)
{
    qDebug() << "onPlayerMediaStatusChanged " << status;
    if(status == QMediaPlayer::LoadedMedia)
        emit playMedia();
}

void VoiceLabel::onPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        qDebug() << "media entry >> show";
        this->show();
        label_animation->setDirection(QAbstractAnimation::Forward);
        label_animation->start();
    }
    else if(state == QMediaPlayer::StoppedState)
    {
        qDebug() << "media entry >> hide";
        label_animation->setDirection(QAbstractAnimation::Backward);
        label_animation->start();
    }
}

void VoiceLabel::onAnimationFinished()
{
    if(label_animation->direction() == QAbstractAnimation::Backward)
    {
        this->hide();
        qDebug() << "Animation Finished >> Hide()";
    }
}
