#include "core/main_widget.h"
#include "ui_main_widget.h"

#include <QDebug>

using namespace IonaDesktop::Core;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    SetupConfig();
    SetupTrayIcon();
    SetupPlugins();

    ui->setupUi(this);
    iona_widget_ptr = new IonaWidget(iona_paint_scale, this);
//    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::Window, true);
    setWindowFlag(Qt::NoDropShadowWindowHint);
    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
//    setWindowFlag(Qt::WindowTitleHint, false);
    setGeometry(main_window_geometry);
    iona_widget_ptr->move(iona_window_position);
}

MainWidget::~MainWidget()
{
    delete ui;
    qDebug() << "Main widget destructed.";
}

/* Tray Icon */
int MainWidget::SetupTrayIcon()
{
    /* setup system tray icon */
    tray_icon_ptr = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/icon/icon_anchor.png");
    tray_icon_ptr->setIcon(icon);
    tray_icon_ptr->setToolTip(QString("Iona-desktop"));
    connect(tray_icon_ptr, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(Slot_TrayIcon_Activated(QSystemTrayIcon::ActivationReason)));
    tray_icon_ptr->show();
    QMenu *tray_menu_ptr = new QMenu();
    tray_icon_ptr->setContextMenu(tray_menu_ptr);

    QAction *act_tray_reset_geometry_ptr = new QAction(QString("Reset Geometry"));
    connect(act_tray_reset_geometry_ptr, SIGNAL(triggered()), this, SLOT(Slot_TrayMenu_ResetGeometry()));
    tray_menu_ptr->addAction(act_tray_reset_geometry_ptr);

    QAction *act_tray_exit_ptr = new QAction(QString("Exit"));
    connect(act_tray_exit_ptr, SIGNAL(triggered()), this, SLOT(Slot_TrayMenu_Exit()));
    tray_menu_ptr->addAction(act_tray_exit_ptr);
    return 0;
}
// Tray Icon Slots
void MainWidget::Slot_TrayIcon_Activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
//    case QSystemTrayIcon::DoubleClick:

    default:
        break;
    }
}
void MainWidget::Slot_TrayMenu_ResetGeometry()
{
    main_window_geometry = default_main_window_geometry;
    I401_paint_scale = default_I401_paint_scale;
    iona_window_position = default_iona_window_position;
    iona_paint_scale = default_iona_paint_scale;
    setGeometry(main_window_geometry);
    repaint();
}
void MainWidget::Slot_TrayMenu_Exit()
{
    SaveConfig();
    exit(0);
}

/*  Event Handler */
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
        main_window_geometry = geometry();
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
    pixmapScaled = pixmap.scaled(I401_paint_scale, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 100, pixmapScaled);
//    setFixedSize(533, 400);
//    setMask(pixmapScaled.mask());
    event->accept();
}

/* Config - Json File Processing */
int MainWidget::SetupConfig()
{
    /* Setup Config */
    main_window_geometry = default_main_window_geometry;
    I401_paint_scale = default_I401_paint_scale;
    iona_window_position = default_iona_window_position;
    iona_paint_scale = default_iona_paint_scale;
    // Read File and cast to Object
    // find if there is a config file, if not, create one.
    QDir config_dir("config");
    if(!config_dir.exists()) {
        bool isMkdir = QDir().mkdir("config");
        if(!isMkdir) {
            qDebug() << "Fatal: Create config dir failed.";
            return -1;
        }
    }
    QFile json_file("./config/config.json");
    if(!json_file.open(QIODevice::ReadOnly)) {
        qDebug() << "Fatal: Failed to load Json file (rw). Use default config";
       return -1;
    }
    QByteArray json_byteArray = json_file.readAll();
    json_file.close();
    QJsonParseError json_error;
    QJsonDocument json_document(QJsonDocument::fromJson(json_byteArray,  &json_error));
    if(json_document.isNull() || json_error.error != QJsonParseError::NoError || !json_document.isObject()) {
        qDebug() << "Error: Json error:" << json_error.errorString();
        return -1;
    }
    QJsonObject root_object = json_document.object();

    // get Core params
    if(root_object.contains("main_window_geometry")) {
        QJsonValue param_value = root_object.value("main_window_geometry");
        if(param_value.isArray()) {
            QJsonArray array = param_value.toArray();
            if(array.size() == 4) {
                bool isValid = true;
                int geo_param[4];
                for(int i = 0; i < 4; i++) {
                    QJsonValue value = array.at(i);
                    if(value.isDouble())
                        geo_param[i] = static_cast<int>(value.toDouble());
                    else
                        isValid = false;
                }
                if(isValid) {
                    main_window_geometry = QRect(geo_param[0], geo_param[1], geo_param[2], geo_param[3]);
                    qDebug() << "Valid param main_window_geometry. Set.";
                }
            }
        }
    }
    if(root_object.contains("I401_paint_scale")) {
        QJsonValue param_value = root_object.value("I401_paint_scale");
        if(param_value.isArray()) {
            QJsonArray array = param_value.toArray();
            if(array.size() == 2) {
                bool isValid = true;
                int size_param[2];
                for(int i = 0; i < 2; i++) {
                    QJsonValue value = array.at(i);
                    if(value.isDouble())
                        size_param[i] = static_cast<int>(value.toDouble());
                    else
                        isValid = false;
                }
                if(isValid) {
                    I401_paint_scale = QSize(size_param[0], size_param[1]);
                    qDebug() << "Valid param I401_paint_scale. Set.";
                }
            }
        }
    }
    if(root_object.contains("iona_window_position")) {
        QJsonValue param_value = root_object.value("iona_window_position");
        if(param_value.isArray()) {
            QJsonArray array = param_value.toArray();
            if(array.size() == 2) {
                bool isValid = true;
                int size_param[2];
                for(int i = 0; i < 2; i++) {
                    QJsonValue value = array.at(i);
                    if(value.isDouble())
                        size_param[i] = static_cast<int>(value.toDouble());
                    else
                        isValid = false;
                }
                if(isValid) {
                    iona_window_position = QPoint(size_param[0], size_param[1]);
                    qDebug() << "Valid param iona_window_position. Set.";
                }
            }
        }
    }
    if(root_object.contains("iona_paint_scale")) {
        QJsonValue param_value = root_object.value("iona_paint_scale");
        if(param_value.isArray()) {
            QJsonArray array = param_value.toArray();
            if(array.size() == 2) {
                bool isValid = true;
                int size_param[2];
                for(int i = 0; i < 2; i++) {
                    QJsonValue value = array.at(i);
                    if(value.isDouble())
                        size_param[i] = static_cast<int>(value.toDouble());
                    else
                        isValid = false;
                }
                if(isValid) {
                    iona_paint_scale = QSize(size_param[0], size_param[1]);
                    qDebug() << "Valid param iona_paint_scale. Set.";
                }
            }
        }
    }

    // get Plugin params
    if(root_object.contains("plugins")) {
        QJsonValue plugins_full_value = root_object.value("plugins");
        if(plugins_full_value.isArray()) {
            QJsonArray plugins_array = plugins_full_value.toArray();
            for(int i = 0; i < plugins_array.size(); i++) {
                QJsonValue plugin_value = plugins_array.at(i);
                if(plugin_value.isObject()) {
                    QJsonObject plugin_obj = plugin_value.toObject();
                    if(plugin_obj.contains("unique_id")) {
                        QJsonValue plugin_id_value = plugin_obj.value("unique_id");
                        if(plugin_id_value.isDouble()) {
                            __int64 plugin_id = static_cast<__int64>(plugin_id_value.toDouble());
                            M_config.insert(std::make_pair(plugin_id, plugin_obj));
                            qDebug() << "Valid param plugin_" << plugin_id << ". Set.";
                        }
                    }
                }
            }
        }
    }
    return 0;
}
void MainWidget::SaveConfig()
{
    // Save Core config
    QJsonObject root_object;
    QJsonArray main_window_geometry_array;
    main_window_geometry_array.append(main_window_geometry.left());
    main_window_geometry_array.append(main_window_geometry.top());
    main_window_geometry_array.append(main_window_geometry.width());
    main_window_geometry_array.append(main_window_geometry.height());
    root_object.insert("main_window_geometry", main_window_geometry_array);
    QJsonArray I401_paint_scale_array;
    I401_paint_scale_array.append(I401_paint_scale.width());
    I401_paint_scale_array.append(I401_paint_scale.height());
    root_object.insert("I401_paint_scale", I401_paint_scale_array);
    QJsonArray iona_window_position_array;
    iona_window_position_array.append(iona_window_position.x());
    iona_window_position_array.append(iona_window_position.y());
    root_object.insert("iona_window_position", iona_window_position_array);
    QJsonArray iona_paint_scale_array;
    iona_paint_scale_array.append(iona_paint_scale.width());
    iona_paint_scale_array.append(iona_paint_scale.height());
    root_object.insert("iona_paint_scale", iona_paint_scale_array);

    // Save Plugins' config
    if(!M_plugins.empty()) {
        QJsonArray plugins_array;
        for(auto it = M_plugins.begin(); it != M_plugins.end(); it++) {
            it->second->getHandler()->updateConfig();
            plugins_array.append(M_config.at(it->first));
        }
        root_object.insert("plugins", plugins_array);
    }

    // Write  to file
    QJsonDocument json_document;
    json_document.setObject(root_object);
    QByteArray json_byteArray = json_document.toJson();
    QFile json_file("./config/config.json");
    if(!json_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Fatal: Failed to load Json file (rw). Use default config";
       return ;
    }
    json_file.write(json_byteArray);
    json_file.close();
}

/* Plugins */
int MainWidget::SetupPlugins()
{
    QDir plugins_dir("./lib");
    if(plugins_dir.entryList(QDir::Files).isEmpty())
        qDebug() << "No Plugin.";
    foreach(QString filename, plugins_dir.entryList(QDir::Files))
    {
        auto plugin_ptr = QSharedPointer<PluginInstance>
                (new PluginInstance(qobject_cast<QObject*>(this),
                plugins_dir.absoluteFilePath(filename)));
        if(plugin_ptr != nullptr) {
            __int64 unique_id = plugin_ptr->getHandler()->getID();
            M_plugins.insert(std::make_pair(unique_id, plugin_ptr));
            if(!M_config.count(unique_id)) {
                QJsonObject empty_object;
                empty_object.insert("unique_id", unique_id);
                M_config.insert(std::make_pair(unique_id, empty_object));
            }
            plugin_ptr->getHandler()->setConfig(&M_config.at(unique_id));
        }
    }
    return 0;
}
MainWidget::PluginInstance::PluginInstance(QObject* parent, QString path)
{
    loader_ptr = QSharedPointer<QPluginLoader>(new QPluginLoader(path));
    QObject* pluginObjPtr = loader_ptr->instance();
    if(pluginObjPtr)
    {
        plugin_handler = qobject_cast<PluginBase*>(pluginObjPtr);
        if(plugin_handler){
            this->plugin_handler->onInit(parent);
            qDebug() << "Load Plugin: " << this->plugin_handler->getName();
        }
    }
}
MainWidget::PluginInstance::~PluginInstance()
{
    delete plugin_handler;
    loader_ptr->unload();
    qDebug() << "Unload Plugin: " << this->plugin_handler->getName();
}
