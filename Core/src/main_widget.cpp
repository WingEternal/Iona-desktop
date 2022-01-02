#include "core/main_widget.h"
#include <iostream>
#include <iomanip>
#include <QFileDialog>
#include <QDebug>

using namespace IonaDesktop::Core;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    SetupConfig();
    SetupTrayIcon();

    setAttribute(Qt::WA_TranslucentBackground);
    // Set as independent window
    setWindowFlag(Qt::Window);
    setWindowFlag(Qt::NoDropShadowWindowHint);
    // Hide taskbar icon
    setWindowFlag(Qt::Tool);
    // No frame
    setWindowFlag(Qt::FramelessWindowHint, true);
    // Stay on top
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
//    setWindowFlag(Qt::WindowTitleHint, false);
    setGeometry(main_window_posLT.x(), main_window_posLT.y(), 650, 500);
    pix_I401.load(":/charater/image/I-401.png");
    iona_widget_ptr = new IonaWidget(this);
    iona_widget_ptr->show();
    SetupPlugins();
}

MainWidget::~MainWidget()
{
    delete iona_widget_ptr;
    delete tray_icon_ptr;
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
    main_window_posLT = default_main_window_posLT;
    move(main_window_posLT.x(), main_window_posLT.y());
    update();
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
}
void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
//    if(mLeftPressed){
    if(m_left_pressed && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPos() - m_left_click_position);
        main_window_posLT = QPoint(geometry().left(), geometry().top());
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
//    pixmapScaled = pixmap.scaled(600,350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, height() / 2 - 175, pix_I401);
    event->accept();
}

/* Config - Json File Processing */
bool MainWidget::JsonArrayToIntArray(QJsonArray &json_array, int *array_ptr, int size)
{
    if(json_array.size() == size) {
            bool isValid = true;
            for(int i = 0; i < size; i++) {
                QJsonValue value = json_array.at(i);
                if(value.isDouble())
                    array_ptr[i] = static_cast<int>(value.toDouble());
                else { isValid = false; break; }
            }
            return isValid;
    }
    return false;
}
int MainWidget::SetupConfig()
{
    /* Setup Config */
    main_window_posLT = default_main_window_posLT;
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
    if(root_object.contains("Exit_posLT")) {
        QJsonValue param_value = root_object.value("Exit_posLT");
        if(param_value.isArray()) {
            int point_param[2];
            QJsonArray json_array = param_value.toArray();
            if(JsonArrayToIntArray(json_array, point_param, 2)) {
                main_window_posLT = QPoint(point_param[0], point_param[1]);
                qDebug() << "Valid param main_window_posLT. Set.";
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
                            qDebug() << "Valid param plugin-" << plugin_id << ". Set.";
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
    QJsonArray main_window_posLT_array;
    main_window_posLT_array.append(main_window_posLT.x());
    main_window_posLT_array.append(main_window_posLT.y());
    root_object.insert("Exit_posLT", main_window_posLT_array);

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
    QStringList suffix_filter_str;
#ifdef Q_OS_WIN
    suffix_filter_str << QString("*.dll");
#endif
#ifdef  Q_OS_LINUX
    filter << QString("*.so");
#endif
    plugins_dir.setFilter(QDir::Files | QDir::NoSymLinks);
    plugins_dir.setNameFilters(suffix_filter_str);

    foreach(QString filename, plugins_dir.entryList(QDir::Files))
    {
        auto plugin_ptr = QSharedPointer<PluginInstance>
                (new PluginInstance(this, plugins_dir.absoluteFilePath(filename)));
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
MainWidget::PluginInstance::PluginInstance(QWidget* parent, QString path)
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
