#include "main_widget/main_widget.h"
#include "ui_main_widget.h"

#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    iona_widget_ptr = new IonaWidget(this);
//    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::Window, true);
    setWindowFlag(Qt::NoDropShadowWindowHint);
    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
//    setWindowFlag(Qt::WindowTitleHint, false);
    setGeometry(1376, 570, 533, 500);
    iona_widget_ptr->move(170, 20);
    Q_INIT_RESOURCE(resource);

    // setup system tray icon
    trayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/icon/icon_anchor.png");
    trayIcon->setIcon(icon);
    trayIcon->setToolTip(QString("PluginCore"));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
    trayMenu = new QMenu(this);
    trayExit = new QAction(QString("Exit"));
    connect(trayExit, SIGNAL(triggered()), this, SLOT(trayMenu_exit()));
    trayMenu->addAction(trayExit);
    trayIcon->setContextMenu(trayMenu);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_left_pressed = true;
        m_left_click_delay.start();
        m_left_click_position = event->globalPos() - this->pos();
        event->accept();
    }
    if (event->button() == Qt::RightButton)
    {
//        QMenu* mRightClickMenu = new QMenu(this);
//        QAction* actChangeImage = mRightClickMenu->addAction("Change Image");
//        QMenu* changeImageSubMenu = new QMenu(this);
//        QAction* actChangeImageNormal = changeImageSubMenu->addAction("normal");
//        QAction* actChangeImageAnniversary = changeImageSubMenu->addAction("anniversary");
//        QAction* actChangeImageValentine = changeImageSubMenu->addAction("valentine");
//        QAction* actChangeImageSwimsuit = changeImageSubMenu->addAction("swimsuit");
//        QAction* actChangeImageHalloween = changeImageSubMenu->addAction("halloween");
//        actChangeImage->setMenu(changeImageSubMenu);

//        connect(actChangeImageNormal, &QAction::triggered, this, [=](){
//            imageFileName = "normal";
//            this->repaint();});
//        connect(actChangeImageAnniversary, &QAction::triggered, this, [=](){
//            imageFileName = "anniversary";
//            this->repaint();});
//        connect(actChangeImageValentine, &QAction::triggered, this, [=](){
//            imageFileName = "valentine";
//            this->repaint();});
//        connect(actChangeImageSwimsuit, &QAction::triggered, this, [=](){
//            imageFileName = "swimsuit";
//            this->repaint();});
//        connect(actChangeImageHalloween, &QAction::triggered, this, [=](){
//            imageFileName = "halloween";
//            this->repaint();});
//        mRightClickMenu->exec(QCursor::pos());
     }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
//    if(mLeftPressed){
    if(m_left_pressed && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPos() - m_left_click_position);
        event->accept();
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_left_pressed && m_left_click_delay.elapsed() >= 150)
    {
        qDebug() << "Judge: move, " << geometry();
    }
    m_left_pressed = false;
    event->ignore();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap, pixmapScaled;
    pixmap.load(":/charater/image/I-401.png");
    pixmapScaled = pixmap.scaled(QSize(600, 300), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 100, pixmapScaled);
//    setFixedSize(533, 400);
//    setMask(pixmapScaled.mask());
    event->accept();
}

void MainWidget::trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        this->show();
        break;
//    case QSystemTrayIcon::DoubleClick:

    default:
        break;
    }
}

void MainWidget::trayMenu_exit()
{
    exit(0);
}
