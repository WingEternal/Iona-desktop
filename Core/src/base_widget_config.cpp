#include "core/base_widget.h"

namespace IonaDesktop{
namespace Core {
    bool JsonArrayToIntArray(QJsonArray &json_array, int *array_ptr, int size)
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
}
}

using namespace IonaDesktop::Core;
int BaseWidget::setupConfig()
{
    window_global_posLT = default_main_window_posLT;
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
                window_global_posLT = QPoint(point_param[0], point_param[1]);
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
                            int32_t plugin_id = static_cast<int32_t>(plugin_id_value.toDouble());
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

void BaseWidget::saveConfig()
{
    QJsonObject root_object;
    QJsonArray main_window_posLT_array;
    UpdatePosLT();
    main_window_posLT_array.append(window_global_posLT.x());
    main_window_posLT_array.append(window_global_posLT.y());
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
