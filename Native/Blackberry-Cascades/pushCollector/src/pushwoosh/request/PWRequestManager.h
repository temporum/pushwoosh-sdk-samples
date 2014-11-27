//
//  PWRequestManager.h
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

class PWRequest;

class PWRequestManager : public QObject
{
    Q_OBJECT

public:
    PWRequestManager(QObject *parent = 0);
    virtual ~PWRequestManager();

    void sendRequest(PWRequest *request);

private Q_SLOTS:
    void httpFinished(QNetworkReply * reply);
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

private:
    QNetworkAccessManager m_accessManager;
    QMap<QNetworkReply *, PWRequest *> requestMap;
};
