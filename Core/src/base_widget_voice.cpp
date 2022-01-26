#include "core/base_widget.h"

using namespace IonaDesktop::Core;

void BaseWidget::setupVoice()
{
    voice_timer_ptr = new QTimer(this);
    voice_player_ptr = new QMediaPlayer(this);
    voice_label_ptr = new QLabel(this);
    voice_label_ptr->setStyleSheet("font-family:微软雅黑; font-size:16px; color: black; background-color:rgb(255, 255, 255, 180); border-radius:15px;");
    voice_label_ptr->setWordWrap(true);
    voice_label_ptr->setAlignment(Qt::AlignCenter);
    voice_label_ptr->setGeometry(260, 275, 220, 55);
    voice_label_ptr->raise();
    // Animation effects
    label_opacity_fx = new QGraphicsOpacityEffect(voice_label_ptr);
    voice_label_ptr->setGraphicsEffect(label_opacity_fx);
    voice_label_ptr->setWindowOpacity(0);
    label_animation = new QPropertyAnimation(label_opacity_fx, "opacity", this);
    label_animation->setDuration(label_animation_duration);
    label_animation->setStartValue(0);
    label_animation->setEndValue(1);
    label_animation->setEasingCurve(QEasingCurve::Linear);
    // SIG-SLOT
    connect(voice_timer_ptr, SIGNAL(timeout()), this, SLOT(Slot_VoiceTimer_Timeout()));
    connect(voice_player_ptr, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(Slot_Player_StateChanged(QMediaPlayer::State)));
    connect(label_animation, SIGNAL(finished()), this, SLOT(Slot_Animation_Finished()));
    // Play startup voice
    voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona01.mp3"));
    voice_player_ptr->setVolume(35);
    voice_label_ptr->setText(QString("潜水艦イ401、イオナ。\n急速せんこ～。"));
    voice_player_ptr->play();
    voice_timer_ptr->start(500);
}

void BaseWidget::Slot_Hitbox_Hit()
{
    qDebug() << "Judge: single click";
    if(voice_player_ptr->state() != QMediaPlayer::StoppedState)
        voice_player_ptr->stop();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int randVoice = qrand()%2 + 1;
    switch(randVoice)
    {
    case 1:
        voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona02.mp3"));
        voice_label_ptr->setText(QString("どうしてここにいるのが分かったの？提督。何か用？"));
        voice_label_ptr->show();
        break;
    case 2:
        voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona03.mp3"));
        voice_label_ptr->setText(QString("提督と一緒に戦えて嬉しい。そうは見えないかもしれないけど。"));
        voice_label_ptr->show();
        break;
    case 3:
        voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/Iona04.mp3"));
        voice_label_ptr->setText(QString("提督は潜水艦が珍しい？二人きりになるとお腹を見たがるのは何故？"));
        voice_label_ptr->show();
        break;
    }
    voice_player_ptr->play();
}

void BaseWidget::Slot_VoiceTimer_Timeout()
{
    QTime currTime = QTime::currentTime();
    if(currTime.minute() == 0 && currTime.second() == 0 && voice_player_ptr->state() == QMediaPlayer::StoppedState)
    {
        switch (currTime.hour())
        {
        case 0:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0000.mp3"));
            voice_label_ptr->setText("マルマルマルマル。日付が変わった。新しい一日の始まり。");
            voice_player_ptr->play();
            break;
        case 1:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0100.mp3"));
            voice_label_ptr->setText("マルヒトマルマル。提督が夜更かしするなら、私も付き合う。");
            voice_player_ptr->play();
            break;
        case 2:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0200.mp3"));
            voice_label_ptr->setText("もしかして、私が寝るのを待っている？マルフタマルマル。提督、一体何をしているの？");
            voice_player_ptr->play();
            break;
        case 3:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0300.mp3"));
            voice_label_ptr->setText("マルサンマルマル。この時間に食事をとると、人間は太るらしい。");
            voice_player_ptr->play();
            break;
        case 4:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0400.mp3"));
            voice_label_ptr->setText("「おはよう」？それとも「おやすみ」？マルヨンマルマル。夜が一番深い時間。");
            voice_player_ptr->play();
            break;
        case 5:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0500.mp3"));
            voice_label_ptr->setText("マルゴーマルマル。提督、私と一緒にコーヒーを飲む？");
            voice_player_ptr->play();
            break;
        case 6:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0600.mp3"));
            voice_label_ptr->setText("マルロクマルマル。艦内照明を夜間モードから昼間モードに切り替える。");
            voice_player_ptr->play();
            break;
        case 7:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0700.mp3"));
            voice_label_ptr->setText("マルナナマルマル。人間の営みが動き出す。今日の天気は、多分晴れ。");
            voice_player_ptr->play();
            break;
        case 8:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0800.mp3"));
            voice_label_ptr->setText("マルハチマルマル。作戦行動開始。まずは、今日見るアニメの録画予約をセットして。");
            voice_player_ptr->play();
            break;
        case 9:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/0900.mp3"));
            voice_label_ptr->setText("マルキューマルマル。挨拶の言葉は、まだ「おはよう」？それとも「こんにちは」？");
            voice_player_ptr->play();
            break;
        case 10:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1000.mp3"));
            voice_label_ptr->setText("ヒトマルマルマル。朝でも昼でもない中途半端な時間。中途半端って、何？");
            voice_player_ptr->play();
            break;
        case 11:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1100.mp3"));
            voice_label_ptr->setText("午前のティータイム、ヒトヒトマルマル。私はコンゴウと違って、紅茶よりも甘いココアが好き。");
            voice_player_ptr->play();
            break;
        case 12:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1200.mp3"));
            voice_label_ptr->setText("ヒトフタマルマル。ランチの時間、今日のメニューを教えて。");
            voice_player_ptr->play();
            break;
        case 13:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1300.mp3"));
            voice_label_ptr->setText("ヒトサンマルマル。急速浮上して、甲板で日向ぼっこ。");
            voice_player_ptr->play();
            break;
        case 14:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1400.mp3"));
            voice_label_ptr->setText("気持ちいい海風。このまま一日が過ぎていくのも悪くない…あ、ヒトヨンマルマル。");
            voice_player_ptr->play();
            break;
        case 15:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1500.mp3"));
            voice_label_ptr->setText("甘いお菓子でちょっと休憩。ヒトゴーマルマルは、おやつの時間。");
            voice_player_ptr->play();
            break;
        case 16:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1600.mp3"));
            voice_label_ptr->setText("ヒトロクマルマル。午後の4時。提督、もう一頑張りヨーソロー。");
            voice_player_ptr->play();
            break;
        case 17:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1700.mp3"));
            voice_label_ptr->setText("ヒトナナマルマル。浮上して一緒に夕焼けを見てくれる？提督。");
            voice_player_ptr->play();
            break;
        case 18:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1800.mp3"));
            voice_label_ptr->setText("ヒトハチマルマル。提督も手伝って。晩御飯の仕度を始める。");
            voice_player_ptr->play();
            break;
        case 19:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/1900.mp3"));
            voice_label_ptr->setText("ヒトキューマルマル。海の底から美味しそうな匂い。今夜のメニューは魚雷型コロッケ。");
            voice_player_ptr->play();
            break;
        case 20:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/2000.mp3"));
            voice_label_ptr->setText("フタマルマルマル。デザートは別腹。でも、メンタルモデルはいくら食べても太らない。");
            voice_player_ptr->play();
            break;
        case 21:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/2100.mp3"));
            voice_label_ptr->setText("フタヒトマルマル。提督が頭を洗ってくれるなら、お風呂に入ってもいいけど。");
            voice_player_ptr->play();
            break;
        case 22:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/2200.mp3"));
            voice_label_ptr->setText("フタフタマルマル。見ちゃダメ、提督。日記をつけてるんだから。もし見たら、全力でぶっ飛ばす。");
            voice_player_ptr->play();
            break;
        case 23:
            voice_player_ptr->setMedia(QUrl("qrc:/charater/voice/2300.mp3"));
            voice_label_ptr->setText("フタサンマルマル。長い一日の終わりの時間。心配しなくてもいい。提督が眠るまで一緒にいてあげる。");
            voice_player_ptr->play();
            break;
        }
    }
}

void BaseWidget::Slot_Player_StateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        qDebug() << "media entry >> show";
        voice_label_ptr->show();
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

void BaseWidget::Slot_Animation_Finished()
{
    if(label_animation->direction() == QAbstractAnimation::Backward) {
        voice_label_ptr->hide();
        qDebug() << "Animation Finished >> Hide()";
    }
}
