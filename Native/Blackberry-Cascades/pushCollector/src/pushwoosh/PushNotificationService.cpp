#include "PushNotificationService.h"
#include <QDebug>
#include <bb/network/PushErrorCode>
#include <bb/data/JsonDataAccess>

#include <bb/system/InvokeRequest>
#include <bb/platform/Notification>

#include <bb/ApplicationInfo>
#include <bb/PackageInfo>

#include "request/PWRegisterDeviceRequest.h"
#include "request/PWUnregisterDeviceRequest.h"
#include "request/PWPushStatRequest.h"
#include "request/PWSetTagsRequest.h"
#include "request/PWGetTagsRequest.h"

using namespace bb::network;

PushNotificationService::PushNotificationService(QObject *parent)
    : QObject(parent)
    , m_pushService(0)
{
}

void PushNotificationService::initializePushService(const QString & bbAppId, const QString & ppgUrl, const QString & pushwooshAppId, const QString & invokeTargetKeyPush, const QString & invokeTargetKeyOpen)
{
    if (!m_pushService || bbAppId != m_configuration.providerApplicationId() || pushwooshAppId != m_configuration.pushwooshAppId()) {

        // If a PushService instance has never been created or if the app id has changed, then create a new PushService instance
        // Important note: App ids would not change in a real application, but this sample application allows this.
        // To allow the app id change, we delete the previously existing PushService instance.
        if (m_pushService) {

            // Disconnect the signals
            QObject::disconnect(m_pushService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
                    this, SLOT(onCreateSessionCompleted(const bb::network::PushStatus&)));
            QObject::disconnect(m_pushService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
                    this, SLOT(onCreateChannelCompleted(const bb::network::PushStatus&, const QString)));
            QObject::disconnect(m_pushService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
                    this, SLOT(onDestroyChannelCompleted(const bb::network::PushStatus&)));
            QObject::disconnect(m_pushService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
                        this, SLOT(onPushTransportReady(bb::network::PushCommand::Type)));
            QObject::disconnect(m_pushService, SIGNAL(simChanged()),
                    this, SLOT(onSimChanged()));

            delete m_pushService;
            m_pushService = NULL;
        }

        m_configuration.setProviderApplicationId(bbAppId);
        m_configuration.setPpgUrl(ppgUrl);
        m_configuration.setPushwooshAppId(pushwooshAppId);
        m_configuration.setInvokeTargetKeyPush(invokeTargetKeyPush);
        m_configuration.setInvokeTargetKeyOpen(invokeTargetKeyOpen);
        m_configuration.save();

        m_pushService = new PushService(m_configuration.providerApplicationId(), m_configuration.invokeTargetKeyPush(), this);

        //Connect the signals
        QObject::connect(m_pushService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
                this, SLOT(onCreateSessionCompleted(const bb::network::PushStatus&)));
        QObject::connect(m_pushService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
                this, SLOT(onCreateChannelCompleted(const bb::network::PushStatus&, const QString)));
        QObject::connect(m_pushService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
                this, SLOT(onDestroyChannelCompleted(const bb::network::PushStatus&)));
        QObject::connect(m_pushService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
                    this, SLOT(onPushTransportReady(bb::network::PushCommand::Type)));
        QObject::connect(m_pushService, SIGNAL(simChanged()),
                this, SLOT(onSimChanged()));
    }
}

void PushNotificationService::createSession()
{
    // Initialize the PushService if it has not been already
    if(!m_pushService)
    {
        qDebug()<< "push service is not initialized!";
        return;
    }

    // Check to see if the PushService has a connection to the Push Agent.
    // This can occur if the application doesn't have sufficient permissions to use Push.
    // For more information on required permissions, please refer to the developer guide.
    if (m_pushService->hasConnection()){
        m_pushService->createSession();
    } else {
        emit noPushServiceConnection();
    }
}

void PushNotificationService::registerForPushNotifications()
{
    if (m_pushService->hasConnection()){
        m_pushService->createChannel(m_configuration.ppgUrl());
    } else {
        emit noPushServiceConnection();
    }
}

void PushNotificationService::unregisterFromPushNotifications()
{
    if (m_pushService->hasConnection()){
        m_pushService->destroyChannel();
    }  else {
        emit noPushServiceConnection();
    }
}

void PushNotificationService::setTags(const QVariantMap & tags, QObject * slotObject, const char * callbackSlot)
{
    PWSetTagsRequest * tagsRequest = new PWSetTagsRequest(m_configuration.pushwooshAppId(), tags);

    if(slotObject != 0 && callbackSlot != 0)
        QObject::connect(tagsRequest, SIGNAL(requestFinished(PWRequest*)), slotObject, callbackSlot);

    requestManager.sendRequest(tagsRequest);
}

void PushNotificationService::getTags(const QVariantMap & tags, QObject * slotObject, const char * callbackSlot)
{
    PWGetTagsRequest * tagsRequest = new PWGetTagsRequest(m_configuration.pushwooshAppId());

    if(slotObject != 0 && callbackSlot != 0)
        QObject::connect(tagsRequest, SIGNAL(requestFinished(PWRequest*)), slotObject, callbackSlot);

    requestManager.sendRequest(tagsRequest);
}

void PushNotificationService::registerWithPushwoosh(const QString &token)
{
    PWRegisterDeviceRequest * registerRequest = new PWRegisterDeviceRequest(m_configuration.pushwooshAppId(), token);

    QObject::connect(registerRequest, SIGNAL(requestFinished(PWRequest*)),
                      this, SLOT(registrationFinished(PWRequest*)));

    requestManager.sendRequest(registerRequest);
}

void PushNotificationService::registrationFinished(PWRequest * request)
{
    PWRegisterDeviceRequest * regRequest = (PWRegisterDeviceRequest *)request;
    if(!request->isValid())
    {
        QString error = request->getError();
        emit errorRegisteringForPushNotifications(error);
        return;
    }

    emit registeredForPushNotifications(regRequest->getToken());
}

void PushNotificationService::unregisterWithPushwoosh()
{
    PWUnregisterDeviceRequest * unregisterRequest = new PWUnregisterDeviceRequest(m_configuration.pushwooshAppId());

    QObject::connect(unregisterRequest, SIGNAL(requestFinished(PWRequest*)),
                      this, SLOT(unregistrationFinished(PWRequest*)));

    requestManager.sendRequest(unregisterRequest);
}

void PushNotificationService::unregistrationFinished(PWRequest * request)
{
    PWUnregisterDeviceRequest * regRequest = (PWUnregisterDeviceRequest *)request;
    if(!regRequest->isValid())
    {
        QString error = request->getError();
        emit errorUnregisteringFromPushNotifications(error);
        return;
    }

    emit unregisteredFromPushNotifications();
}


void PushNotificationService::onCreateSessionCompleted(const bb::network::PushStatus &status)
{
    if (status.code() == PushErrorCode::NoError) {
        m_pushService->registerToLaunch();
//      m_pushService->unregisterFromLaunch();
    } else{
        emit errorRegisteringForPushNotifications(tr("Error: unable to create push session. (Error code: %0)").arg(status.code()));
    }
}

void PushNotificationService::onCreateChannelCompleted(const bb::network::PushStatus &status, const QString &token)
{
    qDebug()<< "creatChannelComplete status: " << status.code();
    qDebug()<< "createChannelComplete token: " << token;

    QString message;

    switch(status.code()){
    case  PushErrorCode::NoError:
        registerWithPushwoosh(token);
        return;
    case  PushErrorCode::TransportFailure:
        message = tr("Create channel failed as the push transport is unavailable. "
                  "Verify your mobile network and/or Wi-Fi are turned on. "
                  "If they are on, you will be notified when the push transport is available again.");
        break;
    case PushErrorCode::SubscriptionContentNotAvailable:
        message = tr("Create channel failed as the PPG is currently returning a server error. "
                  "You will be notified when the PPG is available again.");
        break;
    default:
        message = QString("Create channel failed with error code: %0").arg(status.code());
        break;
    }

    emit errorRegisteringForPushNotifications(message);
}

void PushNotificationService::onDestroyChannelCompleted(const bb::network::PushStatus &status)
{
    qDebug() << "onDestroyChannelCompleted: " << status.code();

    QString message;
    switch(status.code()){
    case  PushErrorCode::NoError:
        unregisterWithPushwoosh();
        return;
    case  PushErrorCode::TransportFailure:
        message = tr("Destroy channel failed as the push transport is unavailable. "
                  "Verify your mobile network and/or Wi-Fi are turned on. "
                  "If they are on, you will be notified when the push transport is available again.");
        break;
    case PushErrorCode::SubscriptionContentNotAvailable:
        message = tr("Destroy channel failed as the PPG is currently returning a server error. "
                  "You will be notified when the PPG is available again.");
        break;
    }

    emit errorUnregisteringFromPushNotifications(message);
}

void PushNotificationService::pushNotificationHandler(bb::network::PushPayload &pushPayload)
{
    bb::data::JsonDataAccess jda;
    QVariant jsonVar = jda.loadFromBuffer(pushPayload.data());
    QVariantMap jsonMap = jsonVar.value<QVariantMap>();

    QString hash = jsonMap["p"].value<QString>();

    //send stat
    PWPushStatRequest * statRequest = new PWPushStatRequest(m_configuration.pushwooshAppId(), hash);
    requestManager.sendRequest(statRequest);

    QString title = jsonMap["title"].value<QString>();
    QString message = jsonMap["m"].value<QString>();
    QString link = jsonMap["l"].value<QString>();
    QString customData = jsonMap["u"].value<QString>();
    QString richPage = jsonMap["h"].value<QString>();

    bb::ApplicationInfo appinfo;
    QString appName = appinfo.title();

    bb::PackageInfo packinfo;
    QString packageName = packinfo.name();

    // Create a notification for the push that will be added to the BlackBerry Hub
    bb::platform::Notification *notification = new bb::platform::Notification(packageName + pushPayload.id(),this);
    notification->setTitle(appName);
    notification->setBody(message);

    // Add an invoke request to the notification
    // This invoke will contain the seqnum of the push.
    // When the notification in the BlackBerry Hub is selected, this seqnum will be used to lookup the push in
    // the database and display it
    bb::system::InvokeRequest invokeRequest;
    invokeRequest.setTarget(m_configuration.invokeTargetKeyOpen());
    invokeRequest.setAction(BB_OPEN_INVOCATION_ACTION);
    invokeRequest.setMimeType("text/plain");
    invokeRequest.setData(pushPayload.data());

    notification->setInvokeRequest(invokeRequest);

    // Add the notification for the push to the BlackBerry Hub
    // Calling this method will add a "splat" to the application icon, indicating that a new push has been received
    notification->notify();

    // If an acknowledgement of the push is required (that is, the push was sent as a confirmed push
    // - which is equivalent terminology to the push being sent with application level reliability),
    // then you must either accept the push or reject the push
    if (pushPayload.isAckRequired()) {
        // In our sample, we always accept the push, but situations might arise where an application
        // might want to reject the push (for example, after looking at the headers that came with the push
        // or the data of the push, we might decide that the push received did not match what we expected
        // and so we might want to reject it)
        m_pushService->acceptPush(pushPayload.id());
    }
}

void PushNotificationService::onSimChanged()
{
}

void PushNotificationService::onPushTransportReady(bb::network::PushCommand::Type)
{
}
