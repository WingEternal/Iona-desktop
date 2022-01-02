#include <QPalette>
#include <QPainter>

#include "core/iona_widget.h"

using namespace IonaDesktop::Core;

IonaWidget::IonaWidget(QWidget *parent)
    : QWidget(parent),
    voice_timer(this),
    voice_player(this),
    voice_label(this),
    label_animation_duration(100)
{
    // Geometry Setting
    setGeometry(140, 10, 500, 500);
    portrait_widget_ptr = new PortraitWidget(this);
    portrait_widget_ptr->setGeometry((width() - 360) / 2, 0, 360, 500);
    // Portrait SIG-SLOT
    connect(portrait_widget_ptr, SIGNAL(mousePress()), this, SLOT(Slot_Portrait_MousePress()));
    // Timer SIG-SLOT
    connect(&voice_timer, SIGNAL(timeout()), this, SLOT(Slot_VoiceTimer_Timeout()));
    voice_timer.start(500);
    // Voice Label Setting
    voice_label.setStyleSheet("font-family:微软雅黑; font-size:16px; color: black; background-color:rgb(255, 255, 255, 180); border-radius:15px;");
    voice_label.setWordWrap(true);
    voice_label.setAlignment(Qt::AlignCenter);
    const double voice_label_size_w_coeff = 0.5;
    voice_label.setGeometry(qRound((width() - voice_label_size_w_coeff * width()) / 2), qRound(0.55 * height()), qRound(voice_label_size_w_coeff * width()), qRound(0.15 * height()));
    voice_label.raise();

    label_opacity_fx = new QGraphicsOpacityEffect(&voice_label);
    voice_label.setGraphicsEffect(label_opacity_fx);
    voice_label.setWindowOpacity(0);
    label_animation = new QPropertyAnimation(label_opacity_fx, "opacity", this);
    label_animation->setDuration(label_animation_duration);
    label_animation->setStartValue(0);
    label_animation->setEndValue(1);
    label_animation->setEasingCurve(QEasingCurve::Linear);
    connect(&voice_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(Slot_Player_StateChanged(QMediaPlayer::State)));
    connect(label_animation, SIGNAL(finished()), this, SLOT(Slot_Animation_Finished()));

    voice_player.setMedia(QUrl("qrc:/charater/voice/Iona01.mp3"));
    voice_player.setVolume(35);
    voice_label.setText(QString("潜水艦イ401、イオナ。\n急速せんこ～。"));
    voice_player.play();
}

IonaWidget::~IonaWidget(){}

void IonaWidget::paintEvent(QPaintEvent *event)
{
    // Do nothing. it's an empty window.
#ifdef DEBUG_WINDOW_POSITION
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.drawRect(0, 0, width() - 1, height() - 1);
#endif
    event->accept();
}
void IonaWidget::Slot_Portrait_MousePress()
{
    qDebug() << "Judge: single click";
    if(voice_player.state() != QMediaPlayer::StoppedState)
        voice_player.stop();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int randVoice = qrand()%2 + 1;
    switch(randVoice)
    {
    case 1:
        voice_player.setMedia(QUrl("qrc:/charater/voice/Iona02.mp3"));
        voice_label.setText(QString("どうしてここにいるのが分かったの？提督。何か用？"));
        voice_label.show();
        break;
    case 2:
        voice_player.setMedia(QUrl("qrc:/charater/voice/Iona03.mp3"));
        voice_label.setText(QString("提督と一緒に戦えて嬉しい。そうは見えないかもしれないけど。"));
        voice_label.show();
        break;
    case 3:
        voice_player.setMedia(QUrl("qrc:/charater/voice/Iona04.mp3"));
        voice_label.setText(QString("提督は潜水艦が珍しい？二人きりになるとお腹を見たがるのは何故？"));
        voice_label.show();
        break;
    }
    voice_player.play();
}
void IonaWidget::Slot_VoiceTimer_Timeout()
{
    QTime currTime = QTime::currentTime();
    if(currTime.minute() == 0 && currTime.second() == 0 && voice_player.state() == QMediaPlayer::StoppedState)
    {
        switch (currTime.hour())
        {
        case 0:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0000.mp3"));
            voice_label.setText("マルマルマルマル。日付が変わった。新しい一日の始まり。");
            voice_player.play();
            break;
        case 1:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0100.mp3"));
            voice_label.setText("マルヒトマルマル。提督が夜更かしするなら、私も付き合う。");
            voice_player.play();
            break;
        case 2:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0200.mp3"));
            voice_label.setText("もしかして、私が寝るのを待っている？マルフタマルマル。提督、一体何をしているの？");
            voice_player.play();
            break;
        case 3:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0300.mp3"));
            voice_label.setText("マルサンマルマル。この時間に食事をとると、人間は太るらしい。");
            voice_player.play();
            break;
        case 4:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0400.mp3"));
            voice_label.setText("「おはよう」？それとも「おやすみ」？マルヨンマルマル。夜が一番深い時間。");
            voice_player.play();
            break;
        case 5:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0500.mp3"));
            voice_label.setText("マルゴーマルマル。提督、私と一緒にコーヒーを飲む？");
            voice_player.play();
            break;
        case 6:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0600.mp3"));
            voice_label.setText("マルロクマルマル。艦内照明を夜間モードから昼間モードに切り替える。");
            voice_player.play();
            break;
        case 7:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0700.mp3"));
            voice_label.setText("マルナナマルマル。人間の営みが動き出す。今日の天気は、多分晴れ。");
            voice_player.play();
            break;
        case 8:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0800.mp3"));
            voice_label.setText("マルハチマルマル。作戦行動開始。まずは、今日見るアニメの録画予約をセットして。");
            voice_player.play();
            break;
        case 9:
            voice_player.setMedia(QUrl("qrc:/charater/voice/0900.mp3"));
            voice_label.setText("マルキューマルマル。挨拶の言葉は、まだ「おはよう」？それとも「こんにちは」？");
            voice_player.play();
            break;
        case 10:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1000.mp3"));
            voice_label.setText("ヒトマルマルマル。朝でも昼でもない中途半端な時間。中途半端って、何？");
            voice_player.play();
            break;
        case 11:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1100.mp3"));
            voice_label.setText("午前のティータイム、ヒトヒトマルマル。私はコンゴウと違って、紅茶よりも甘いココアが好き。");
            voice_player.play();
            break;
        case 12:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1200.mp3"));
            voice_label.setText("ヒトフタマルマル。ランチの時間、今日のメニューを教えて。");
            voice_player.play();
            break;
        case 13:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1300.mp3"));
            voice_label.setText("ヒトサンマルマル。急速浮上して、甲板で日向ぼっこ。");
            voice_player.play();
            break;
        case 14:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1400.mp3"));
            voice_label.setText("気持ちいい海風。このまま一日が過ぎていくのも悪くない…あ、ヒトヨンマルマル。");
            voice_player.play();
            break;
        case 15:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1500.mp3"));
            voice_label.setText("甘いお菓子でちょっと休憩。ヒトゴーマルマルは、おやつの時間。");
            voice_player.play();
            break;
        case 16:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1600.mp3"));
            voice_label.setText("ヒトロクマルマル。午後の4時。提督、もう一頑張りヨーソロー。");
            voice_player.play();
            break;
        case 17:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1700.mp3"));
            voice_label.setText("ヒトナナマルマル。浮上して一緒に夕焼けを見てくれる？提督。");
            voice_player.play();
            break;
        case 18:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1800.mp3"));
            voice_label.setText("ヒトハチマルマル。提督も手伝って。晩御飯の仕度を始める。");
            voice_player.play();
            break;
        case 19:
            voice_player.setMedia(QUrl("qrc:/charater/voice/1900.mp3"));
            voice_label.setText("ヒトキューマルマル。海の底から美味しそうな匂い。今夜のメニューは魚雷型コロッケ。");
            voice_player.play();
            break;
        case 20:
            voice_player.setMedia(QUrl("qrc:/charater/voice/2000.mp3"));
            voice_label.setText("フタマルマルマル。デザートは別腹。でも、メンタルモデルはいくら食べても太らない。");
            voice_player.play();
            break;
        case 21:
            voice_player.setMedia(QUrl("qrc:/charater/voice/2100.mp3"));
            voice_label.setText("フタヒトマルマル。提督が頭を洗ってくれるなら、お風呂に入ってもいいけど。");
            voice_player.play();
            break;
        case 22:
            voice_player.setMedia(QUrl("qrc:/charater/voice/2200.mp3"));
            voice_label.setText("フタフタマルマル。見ちゃダメ、提督。日記をつけてるんだから。もし見たら、全力でぶっ飛ばす。");
            voice_player.play();
            break;
        case 23:
            voice_player.setMedia(QUrl("qrc:/charater/voice/2300.mp3"));
            voice_label.setText("フタサンマルマル。長い一日の終わりの時間。心配しなくてもいい。提督が眠るまで一緒にいてあげる。");
            voice_player.play();
            break;
        }
    }
}

void IonaWidget::Slot_Player_StateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        qDebug() << "media entry >> show";
        voice_label.show();
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
void IonaWidget::Slot_Animation_Finished()
{
    if(label_animation->direction() == QAbstractAnimation::Backward) {
        voice_label.hide();
        qDebug() << "Animation Finished >> Hide()";
    }
}

