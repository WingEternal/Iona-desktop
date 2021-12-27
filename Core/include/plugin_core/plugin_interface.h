#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#pragma once
#include <QObject>
#include <QString>
#include <QPluginLoader>

class PluginBase
{
public:
    // Destructor
    virtual ~PluginBase(){}

    // Plugin name
    virtual QString getName() = 0;

    // Plugin detail && remark
    virtual QString getInformation() = 0;

    // Plugin version
    virtual QString getVersion() = 0;

    // Plugin start API
    virtual int onInit(QObject* parent) = 0;
};

#define PluginInterface_iid "com.dist.interface"
Q_DECLARE_INTERFACE(PluginBase, PluginInterface_iid)

#endif // PLUGIN_INTERFACE_H
