/*
* Copyright (c) 2012, 2013  BlackBerry Limited.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "app.hpp"

#include "PushContentController.hpp"

#include "pushwoosh/request/PWRequest.h"
#include "pushwoosh/request/PWSetTagsRequest.h"
#include "pushwoosh/request/PWGetTagsRequest.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Sheet>

#include <bb/network/PushErrorCode>

#include <bb/platform/Notification>

#include <bb/system/InvokeRequest>
#include <bb/system/SystemDialog>
#include <bb/system/SystemUiButton>

#include <bb/data/JsonDataAccess>

#define BB_OPEN_INVOCATION_ACTION "bb.action.OPEN"
#define NOTIFICATION_PREFIX "com.example.pushCollector_"

// This needs to match the invoke target specified in bar-descriptor.xml
// The Invoke target key for receiving new push notifications
#define INVOKE_TARGET_KEY_PUSH "com.example.pushCollector.invoke.push"

// This needs to match the invoke target specified in bar-descriptor.xml
// The Invoke target key when selecting a notification in the BlackBerry Hub
#define INVOKE_TARGET_KEY_OPEN "com.example.pushCollector.invoke.open"

using namespace bb::network;
using namespace bb::cascades;
using namespace bb::system;
using namespace bb::platform;

App::App()
    : m_invokeManager(new InvokeManager(this))
    , m_hasBeenInForeground(false)
    , m_configSaveAction(false)
    , m_pushContentController(new PushContentController(this))
    , m_model(new GroupDataModel(this))
{
    qmlRegisterType<bb::system::SystemUiButton>("bb.system", 1, 0, "SystemUiButton");
    qmlRegisterType<SystemDialog>("bb.system", 1, 0, "SystemDialog");
    qmlRegisterType<PushContentController>();

    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Push Collector");

    m_model->setSortingKeys(QStringList() << "pushdate" << "pushtime");
    m_model->setGrouping(ItemGrouping::ByFullValue);
    m_model->setSortedAscending(false);

    connect(m_model, SIGNAL(itemAdded(QVariantList)), SIGNAL(modelIsEmptyChanged()));
    connect(m_model, SIGNAL(itemUpdated(QVariantList)), SIGNAL(modelIsEmptyChanged()));
    connect(m_model, SIGNAL(itemRemoved(QVariantList)), SIGNAL(modelIsEmptyChanged()));
    connect(m_model, SIGNAL(itemsChanged(bb::cascades::DataModelChangeType::Type, QSharedPointer<bb::cascades::DataModel::IndexMapper>)), SIGNAL(modelIsEmptyChanged()));

    //TODO: place a real values here, received from BlackBerry
    m_pushNotificationService.initializePushService("XXXXX-XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", "http://pushapi.eval.blackberry.com", "XXXXX-XXXXX",
            INVOKE_TARGET_KEY_PUSH, INVOKE_TARGET_KEY_OPEN);

    // connect the push notification service signals and slots
    QObject::connect(&m_pushNotificationService, SIGNAL(registeredForPushNotifications(const QString &)),
            this, SLOT(registeredForPushNotifications(const QString &)));

    QObject::connect(&m_pushNotificationService, SIGNAL(errorRegisteringForPushNotifications(const QString &)),
            this, SLOT(errorRegisteringForPushNotifications(const QString &)));

    QObject::connect(&m_pushNotificationService, SIGNAL(noPushServiceConnection()),
                this, SLOT(onNoPushServiceConnection()));


    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_pushAPIHandler", this);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // populate the model with the pushes that are stored in the database
    m_model->insertList(m_pushHandler.pushes());

    // connect to InvokeManager "invoked" signal to handle incoming push notifications.
    // We will ignore non-push invoke requests.
    connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            SLOT(onInvoked(const bb::system::InvokeRequest&)));

    initializePushSession();
}

void App::registeredForPushNotifications(const QString & token)
{
    qDebug() << "subscribed for push with token: " << token;
}

void App::errorRegisteringForPushNotifications(const QString & error)
{
    qDebug() << "error subscribing for push: " << error;
}

void App::onFullscreen()
{
    m_hasBeenInForeground = true;
}

void App::onNoPushServiceConnection()
{
    emit closeActivityDialog();
    showDialog(tr("Push Collector"), tr("Error: Push Service could not connect to the Push Agent"));
}

void App::createChannel()
{
    m_pushNotificationService.registerForPushNotifications();

    QVariantMap map;
    map["Hello"] = 5;
    map["String"] = "Value";

    QList<QString> array;
    array.push_back(QString("One"));
    array.push_back(QString("Two"));
    map["Array"] = QVariant(array);

    //Set Tags and Get Tags example
    m_pushNotificationService.setTags(map, this, SLOT(setTagsFinished(PWRequest*)));
    m_pushNotificationService.getTags(this, SLOT(getTagsFinished(PWRequest*)));
}

void App::setTagsFinished(PWRequest * request)
{
    if(!request->isValid())
    {
        QString error = request->getError();
        qDebug() << "Set Tags Error: " << error;
        return;
    }

    qDebug() << "Set Tags Success";
}

void App::getTagsFinished(PWRequest * request)
{
    if(!request->isValid())
    {
        QString error = request->getError();
        qDebug() << "Get Tags Error: " << error;
        return;
    }

    PWGetTagsRequest * req = (PWGetTagsRequest *) request;
    QVariantMap tags = req->getTags();

    QString reqString;
    bb::data::JsonDataAccess jda;
    jda.saveToBuffer(tags, &reqString);

    qDebug() << "Get Tags Success: " << reqString;
}

void App::destroyChannel()
{
    m_pushNotificationService.unregisterFromPushNotifications();
}

void App::onInvoked(const InvokeRequest &request)
{
    if (request.action().compare(BB_PUSH_INVOCATION_ACTION) == 0) {
        qDebug() << "Received push action";
        // Received an incoming push
        // Extract it from the invoke request and then process it
        PushPayload payload(request);
        if (payload.isValid()) {
            pushNotificationHandler(payload);
        }
    } else if (request.action().compare(BB_OPEN_INVOCATION_ACTION) == 0){
        qDebug() << "Received open action";
        // Received an invoke request to open an existing push (ie. from a notification in the BlackBerry Hub)
        // The payload from the open invoke is the seqnum for the push in the database
        openPush(request.data());
    }
}

void App::pushNotificationHandler(bb::network::PushPayload &pushPayload)
{
    // Check for a duplicate push
    PushHistoryItem pushHistoryItem(pushPayload.id());

    if (m_pushHandler.checkForDuplicate(pushHistoryItem)) {
        // A duplicate was found, stop processing. Silently discard this push from the user
        qWarning() << QString("Duplicate push was found with ID: %0.").arg(pushPayload.id());
        return;
    }

    // Convert from PushPayload to Push so that it can be stored in the database
    Push push(pushPayload);
    // Save the push and set the sequence number (ID) of the push
    push.setSeqNum(m_pushHandler.save(push));

    m_pushNotificationService.pushNotificationHandler(pushPayload);

    // Convert from PushPayload to Push so that it can be stored in the database
    m_model->insert(push.toMap());

    if (!m_hasBeenInForeground) {
        Application::instance()->requestExit();
    }
}

void App::deletePush(const QVariantMap &item)
{
    SystemDialog deleteDialog;
    deleteDialog.setTitle("Delete");
    deleteDialog.setBody("Delete Item?");
    deleteDialog.confirmButton()->setLabel("Delete");

    if (deleteDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
        Push push(item);
        m_pushHandler.remove(push.seqNum());
        m_model->remove(item);

        // The push has been deleted, so delete the notification
        Notification::deleteFromInbox(NOTIFICATION_PREFIX + QString::number(push.seqNum()));
    }
}

void App::deleteAllPushes()
{
    SystemDialog deleteAllDialog;
    deleteAllDialog.setTitle("Delete ALL");
    deleteAllDialog.setBody("Delete All Items?");
    deleteAllDialog.confirmButton()->setLabel("Delete");

    if (deleteAllDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
        m_pushHandler.removeAll();

        // All the pushes have been deleted, so delete all the notifications for the app
        Notification::deleteAllFromInbox();
        m_model->clear();
    }
}

void App::markAllPushesAsRead()
{
    if (m_model->size() > 0) {
        // All the pushes have been marked as open/read, so delete all the notifications for the app
        Notification::deleteAllFromInbox();

        m_pushHandler.markAllAsRead();

        for (QVariantList indexPath = m_model->first(); !indexPath.isEmpty(); indexPath = m_model->after(indexPath)) {
            QVariantMap item =  m_model->data(indexPath).toMap();
            item["unread"] = false;
            m_model->updateItem(indexPath,item);
        }
    }
}

void App::selectPush(const QVariantList &indexPath)
{
    const QVariantMap item = m_model->data(indexPath).toMap();
    Push push(item);
    updatePushContent(push, indexPath);
}

void App::openPush(QByteArray pushContent)
{
    Push push;
    push.setContent(pushContent);

    QVariantList indexPath = m_model->findExact(push.toMap());
    updatePushContent(push, indexPath);
}

void App::updatePushContent(Push &push, const QVariantList &indexPath)
{
    push.setUnread(false);
    m_pushHandler.markAsRead(push.seqNum());

    m_model->updateItem(indexPath, push.toMap());

    // The push has been opened, so delete the notification
    Notification::deleteFromInbox(NOTIFICATION_PREFIX + QString::number(push.seqNum()));

    m_pushContentController->setCurrentPush(push);
}

QString App::convertToUtf8String(const QVariant &pushContent)
{
    return QString::fromUtf8(pushContent.toByteArray().data());
}

void App::initializePushSession()
{
    m_pushNotificationService.createSession();
}

void App::showDialog(const QString &title, const QString &message)
{
    m_notificationTitle = title;
    m_notificationBody = message;

    emit notificationChanged();
}

void App::openActivityDialog(const QString& title, const QString &message)
{
    m_activityDialogTitle = title;
    m_activityDialogBody = message;
    emit activityDialogChanged();

    emit openActivityDialog();
}

void App::setPromptDefaultText(SystemCredentialsPrompt* prompt,const QString &username, const QString &password)
{
    if (prompt) {
        prompt->usernameField()->setDefaultText(username);
        prompt->passwordField()->setDefaultText(password);
    }
}


bool App::validateUser(const QString &dialogTitle, const QString &username, const QString &password)
{
    return true;
}

bb::cascades::GroupDataModel* App::model() const
{
    return m_model;
}

bool App::modelIsEmpty() const
{
    return (m_model->size() == 0);
}

QString App::notificationTitle() const
{
    return m_notificationTitle;
}

QString App::notificationBody() const
{
    return m_notificationBody;
}

QString App::activityDialogTitle() const
{
    return m_activityDialogTitle;
}

QString App::activityDialogBody() const
{
    return m_activityDialogBody;
}

PushContentController* App::currentPushContent() const
{
    return m_pushContentController;
}
