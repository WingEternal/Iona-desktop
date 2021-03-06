#ifndef IONADESKTOP_CORE_PLUGIN_INTERFACE_H
#define IONADESKTOP_CORE_PLUGIN_INTERFACE_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QPluginLoader>

class PluginBase : public QObject
{
    Q_OBJECT
public:
    // Destructor
    virtual ~PluginBase(){}
    // Plugin name
    virtual QString getName() const
    { return name; }
    // Unique ID is the unique identifier to distinguish different plugins, must be override
    virtual int32_t getID() const = 0;
    // Plugin detail && remark
    virtual QString getDescription() const
    { return description; }
    // Plugin version
    virtual QString getVersion() const
    { return version; }
    // Plugin start API
    virtual int onInit(QWidget* parent) = 0;
    // Update Config Json when called. It is designed as a public method instead of a slot function, because it is necessary to ensure that the plugin's config is updated before saving, but the slot function is executed on the plugin's own thread and cannot block the main thread.
    virtual void updateConfig()
    { return; }
    void setConfig(QJsonObject* cfg)
    { config = cfg; }
    QJsonObject* getConfig() const
    { return config; }
private:
    QJsonObject *config;
protected:
    QString name;
    QString version;
    QString description;
};

#define PluginInterface_iid "com.dist.interface"
Q_DECLARE_INTERFACE(PluginBase, PluginInterface_iid)

#endif // IONADESKTOP_CORE_PLUGIN_INTERFACE_H
