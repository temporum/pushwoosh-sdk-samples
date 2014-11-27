#ifndef PUSHNOTIFICATIONSERVICE_HPP
#define PUSHNOTIFICATIONSERVICE_HPP

#include "PushNotificationService.h"

#include <bb/network/PushService>
#include <bb/network/PushStatus>
#include <bb/network/PushPayload>

#include <QObject>
#include <QVariantList>

#include "request/PWRequestManager.h"
#include "Configuration.h"

#define BB_OPEN_INVOCATION_ACTION "bb.action.OPEN"

/*!
 * Offers services related to the registering of a user to receive pushes, the
 * handling / processing of pushes, and the unregistering of a user from receiving pushes,
 * and the handling of a SIM card change.
 */
class PushNotificationService :  public QObject
{
    Q_OBJECT

public:
    PushNotificationService(QObject *parent = 0);

    void initializePushService(const QString & bbAppId, const QString & ppgUrl, const QString & pushwooshAppId,
            const QString & invokeTargetKeyPush, const QString & invokeTargetKeyOpen);

    void createSession();
    void registerForPushNotifications();
    void unregisterFromPushNotifications();

    void setTags(const QVariantMap & tags, QObject * slotObject, const char * callbackSlot);
    void getTags(const QVariantMap & tags, QObject * slotObject, const char * callbackSlot);

    void registerWithPushwoosh(const QString &token);
    void unregisterWithPushwoosh();

    void pushNotificationHandler(bb::network::PushPayload &pushPayload);


Q_SIGNALS:
    void noPushServiceConnection();

    void registeredForPushNotifications(const QString & token);
    void errorRegisteringForPushNotifications(const QString & error);

    void unregisteredFromPushNotifications();
    void errorUnregisteringFromPushNotifications(const QString & error);

private Q_SLOTS:
    void registrationFinished(PWRequest * request);
    void unregistrationFinished(PWRequest * request);

    //general push session established
    void onCreateSessionCompleted(const bb::network::PushStatus &status);
    //push channel created and token received
    void onCreateChannelCompleted(const bb::network::PushStatus &status, const QString &token);
    //unregistered from push
    void onDestroyChannelCompleted(const bb::network::PushStatus &status);
    void onPushTransportReady(bb::network::PushCommand::Type command);
    void onSimChanged();

private:
    Configuration m_configuration;
    bb::network::PushService *m_pushService;

    PWRequestManager requestManager;
};

#endif
