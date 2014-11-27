#include "Configuration.h"
#include <QSettings>

#define QSETTINGS_CONFIG_GROUP "pw_configuration"
#define PROVIDER_APP_ID_KEY "providerAppId"
#define PUSHWOOSH_APP_ID_KEY "pushwooshAppId"
#define PPG_URL_KEY "ppgUrl"
#define KEY_PUSH "keyPush"
#define KEY_OPEN "keyOpen"

Configuration::Configuration()
{
    QSettings settings;

    settings.beginGroup(QSETTINGS_CONFIG_GROUP);
    setPpgUrl(settings.value(PPG_URL_KEY, "").toString());
    setProviderApplicationId(settings.value(PROVIDER_APP_ID_KEY, "").toString());
    setPushwooshAppId(settings.value(PUSHWOOSH_APP_ID_KEY, "").toString());
    setInvokeTargetKeyPush(settings.value(KEY_PUSH, "").toString());
    setInvokeTargetKeyOpen(settings.value(KEY_OPEN, "").toString());
    settings.endGroup();
}

Configuration::~Configuration()
{
}

QString Configuration::providerApplicationId() const
{
    return m_providerApplicationId;
}

QString Configuration::ppgUrl() const
{
    return m_ppgUrl;
}

QString Configuration::pushwooshAppId() const
{
    return m_pushwooshAppId;
}

QString Configuration::invokeTargetKeyPush() const
{
    return m_invokeTargetKeyPush;
}

QString Configuration::invokeTargetKeyOpen() const
{
    return m_invokeTargetKeyOpen;
}

void Configuration::setProviderApplicationId(const QString& providerApplicationId)
{
    m_providerApplicationId = providerApplicationId;
}

void Configuration::setPpgUrl(const QString& ppgUrl)
{
    m_ppgUrl = ppgUrl;
}

void Configuration::setPushwooshAppId(const QString& pwAppId)
{
    m_pushwooshAppId = pwAppId;
}

void Configuration::setInvokeTargetKeyPush(const QString& key)
{
    m_invokeTargetKeyPush = key;
}
void Configuration::setInvokeTargetKeyOpen(const QString& key)
{
    m_invokeTargetKeyOpen = key;
}

void Configuration::save()
{
    QSettings settings;
    settings.beginGroup(QSETTINGS_CONFIG_GROUP);
    settings.setValue(PPG_URL_KEY, ppgUrl());
    settings.setValue(PROVIDER_APP_ID_KEY, providerApplicationId());
    settings.setValue(PUSHWOOSH_APP_ID_KEY, pushwooshAppId());
    settings.setValue(KEY_PUSH, invokeTargetKeyPush());
    settings.setValue(KEY_OPEN, invokeTargetKeyOpen());
    settings.endGroup();
}
