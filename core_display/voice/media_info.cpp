#include "voice/media_info.h"

using namespace IonaDesktop::CoreDisplay;

const MediaInfoArray::MediaInfo MediaInfoArray::start_info[1] = {
    {"/assets/voice/Iona01.wav", "潜水艦イ401、イオナ。\n急速せんこ～。"}};
const MediaInfoArray::MediaInfo MediaInfoArray::random_info[3] = {
    {"/assets/voice/Iona02.wav",
     "どうしてここにいるのが分かったの？提督。何か用？"},
    {"assets/voice/Iona03.wav",
     "提督と一緒に戦えて嬉しい。そうは見えないかもしれないけど。"},
    {"assets/voice/Iona03.wav",
     "提督は潜水艦が珍しい？二人きりになるとお腹を見たがるのは何故？"}};
const MediaInfoArray::MediaInfo MediaInfoArray::chime_info[24] = {
    {"/assets/voice/0000.wav",
     "マルマルマルマル。日付が変わった。新しい一日の始まり。"},
    {"/assets/voice/0100.wav",
     "マルヒトマルマル。提督が夜更かしするなら、私も付き合う。"},
    {"/assets/voice/0200.wav",
     "もしかして、私が寝るのを待っている？マルフタマルマル。提督、一体何をして"
     "いるの？"},
    {"/assets/voice/0300.wav",
     "マルサンマルマル。この時間に食事をとると、人間は太るらしい。"},
    {"/assets/voice/0400.wav",
     "「おはよう」？それとも「おやすみ」？マルヨンマルマル。夜が一番深い時間"
     "。"},
    {"/assets/voice/0500.wav",
     "マルゴーマルマル。提督、私と一緒にコーヒーを飲む？"},
    {"/assets/voice/0600.wav",
     "マルロクマルマル。艦内照明を夜間モードから昼間モードに切り替える。"},
    {"/assets/voice/0700.wav",
     "マルナナマルマル。人間の営みが動き出す。今日の天気は、多分晴れ。"},
    {"/assets/voice/0800.wav",
     "マルハチマルマル。作戦行動開始。まずは、今日見るアニメの録画予約をセット"
     "して。"},
    {"/assets/voice/0900.wav",
     "マルキューマルマル。挨拶の言葉は、まだ「おはよう」？それとも「こんにちは"
     "」？"},
    {"/assets/voice/1000.wav",
     "ヒトマルマルマル。朝でも昼でもない中途半端な時間。中途半端って、何？"},
    {"/assets/voice/1100.wav",
     "午前のティータイム、ヒトヒトマルマル。私はコンゴウと違って、紅茶よりも甘"
     "いココアが好き。"},
    {"/assets/voice/1200.wav",
     "ヒトフタマルマル。ランチの時間、今日のメニューを教えて。"},
    {"/assets/voice/1300.wav",
     "ヒトサンマルマル。急速浮上して、甲板で日向ぼっこ。"},
    {"/assets/voice/1400.wav",
     "気持ちいい海風。このまま一日が過ぎていくのも悪くない…あ、ヒトヨンマルマル"
     "。"},
    {"/assets/voice/1500.wav",
     "甘いお菓子でちょっと休憩。ヒトゴーマルマルは、おやつの時間。"},
    {"/assets/voice/1600.wav",
     "ヒトロクマルマル。午後の4時。提督、もう一頑張りヨーソロー。"},
    {"/assets/voice/1700.wav",
     "ヒトナナマルマル。浮上して一緒に夕焼けを見てくれる？提督。"},
    {"/assets/voice/1800.wav",
     "ヒトハチマルマル。提督も手伝って。晩御飯の仕度を始める。"},
    {"/assets/voice/1900.wav",
     "ヒトキューマルマル。海の底から美味しそうな匂い。今夜のメニューは魚雷型コ"
     "ロッケ。"},
    {"/assets/voice/2000.wav",
     "フタマルマルマル。デザートは別腹。でも、メンタルモデルはいくら食べても太"
     "らない。"},
    {"/assets/voice/2100.wav",
     "フタヒトマルマル。提督が頭を洗ってくれるなら、お風呂に入ってもいいけど"
     "。"},
    {"/assets/voice/2200.wav",
     "フタフタマルマル。見ちゃダメ、提督。日記をつけてるんだから。もし見たら、"
     "全力でぶっ飛ばす。"},
    {"/assets/voice/2300.wav",
     "フタサンマルマル。長い一日の終わりの時間。心配しなくてもいい。提督が眠る"
     "まで一緒にいてあげる。"}};
