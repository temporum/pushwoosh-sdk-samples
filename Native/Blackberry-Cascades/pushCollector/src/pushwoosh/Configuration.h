#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <QString>

class Configuration
{
public:
    Configuration();
    virtual ~Configuration();

    QString providerApplicationId() const;
    QString ppgUrl() const;
    QString pushwooshAppId() const;
    QString invokeTargetKeyPush() const;
    QString invokeTargetKeyOpen() const;

    void setProviderApplicationId(const QString& providerApplicationId);
    void setPpgUrl(const QString& ppgUrl);
    void setPushwooshAppId(const QString& pwAppId);
    void setInvokeTargetKeyPush(const QString& key);
    void setInvokeTargetKeyOpen(const QString& key);

    void save();

private:
    QString m_providerApplicationId;
    QString m_ppgUrl;
    QString m_pushwooshAppId;
    QString m_invokeTargetKeyPush;
    QString m_invokeTargetKeyOpen;
};

#endif
