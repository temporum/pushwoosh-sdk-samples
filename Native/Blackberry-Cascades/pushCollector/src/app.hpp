/*!
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

#ifndef APP_HPP
#define APP_HPP

#include "pushwoosh/PushNotificationService.h"

#include "service/PushHandler.hpp"
#include <bb/cascades/GroupDataModel>

#include <bb/system/InvokeManager>
#include <bb/system/SystemCredentialsPrompt>

class PushContentController;

class App : public QObject
{
    Q_OBJECT

    // The data model that contains all received pushes
    Q_PROPERTY(bb::cascades::GroupDataModel* model READ model CONSTANT)
    Q_PROPERTY(bool modelIsEmpty READ modelIsEmpty NOTIFY modelIsEmptyChanged)

    // The title and body text for the notification dialog
    Q_PROPERTY(QString notificationTitle READ notificationTitle NOTIFY notificationChanged)
    Q_PROPERTY(QString notificationBody READ notificationBody NOTIFY notificationChanged)

    // The title and body text for the activity dialog
    Q_PROPERTY(QString activityDialogTitle READ activityDialogTitle NOTIFY activityDialogChanged)
    Q_PROPERTY(QString activityDialogBody READ activityDialogBody NOTIFY activityDialogChanged)

    // The controller object for the push content page
    Q_PROPERTY(PushContentController* currentPushContent READ currentPushContent CONSTANT)

public:
    App();

    /**
     * Calls the push service create channel
     */
    Q_INVOKABLE void createChannel();

    /**
     * Calls the push service destroy channel
     */
    Q_INVOKABLE void destroyChannel();

    Q_INVOKABLE void deletePush(const QVariantMap &item);

    Q_INVOKABLE void deleteAllPushes();

    Q_INVOKABLE void markAllPushesAsRead();

    /**
     * Marks the passed push as current one and prepares the controller
     * object of the PushContentPage.
     */
    Q_INVOKABLE void selectPush(const QVariantList &indexPath);

    Q_INVOKABLE QString convertToUtf8String(const QVariant &pushContent);

public Q_SLOTS:
    void onInvoked(const bb::system::InvokeRequest &request);
    void registeredForPushNotifications(const QString & token);
    void errorRegisteringForPushNotifications(const QString & error);
    void onNoPushServiceConnection();
    void onFullscreen();

    void setTagsFinished(PWRequest * request);
    void getTagsFinished(PWRequest * request);

Q_SIGNALS:
    void modelIsEmptyChanged();
    void notificationChanged();
    void activityDialogChanged();

    void openActivityDialog();
    void closeActivityDialog();

private:
    // A helper function to initialize the push session
    void initializePushSession();
    bool validateUser(const QString &dialogTitle, const QString &username, const QString &password);
    void setPromptDefaultText(bb::system::SystemCredentialsPrompt* prompt,const QString &username, const QString &password);
    void pushNotificationHandler(bb::network::PushPayload &pushPayload);
    void showDialog(const QString &title, const QString &message);
    void openActivityDialog(const QString &title, const QString &message);

    // used to open and display a push when a notification is selected in the BlackBerry Hub
    void openPush(QByteArray pushContent);

    // a helper function which marks the push as read, and updates the displayed push content
    void updatePushContent(Push &push, const QVariantList &indexPath);

    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    bool modelIsEmpty() const;

    QString notificationTitle() const;
    QString notificationBody() const;
    QString activityDialogTitle() const;
    QString activityDialogBody() const;

    PushContentController* currentPushContent() const;

    // The manager object to react to invocations
    bb::system::InvokeManager *m_invokeManager;

    PushNotificationService m_pushNotificationService;
    PushHandler m_pushHandler;

    // Whether or not the application has at some point in time been running in the foreground
    bool m_hasBeenInForeground;

    // Whether or not the Configuration is in the process of being saved
    bool m_configSaveAction;

    // The controller object for the push content page
    PushContentController* m_pushContentController;

    // The property values
    bb::cascades::GroupDataModel *m_model;
    QString m_notificationTitle;
    QString m_notificationBody;
    QString m_activityDialogTitle;
    QString m_activityDialogBody;
    bool m_launchApplicationOnPush;
};

#endif
