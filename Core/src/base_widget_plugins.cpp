#include "core/base_widget.h"

using namespace IonaDesktop::Core;

int BaseWidget::setupPlugins()
{
    QDir plugins_dir("./lib");
    if(plugins_dir.entryList(QDir::Files).isEmpty())
        qDebug() << "No Plugin.";
    QStringList suffix_filter_str;
#ifdef Q_OS_WIN
    suffix_filter_str << QString("*.dll");
#endif
#ifdef  Q_OS_LINUX
    suffix_filter_str << QString("*.so");
#endif
    plugins_dir.setFilter(QDir::Files | QDir::NoSymLinks);
    plugins_dir.setNameFilters(suffix_filter_str);

    foreach(QString filename, plugins_dir.entryList(QDir::Files))
    {
        auto plugin_ptr = QSharedPointer<PluginInstance>
                (new PluginInstance(this, plugins_dir.absoluteFilePath(filename)));
        if(plugin_ptr != nullptr) {
            int32_t unique_id = plugin_ptr->getHandler()->getID();
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
