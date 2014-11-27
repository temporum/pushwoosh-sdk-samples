//
//  PWRequestManager
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequestManager.h"
#include "PWRegisterDeviceRequest.h"

#include <QtCore/QDebug>
#include <bb/data/JsonDataAccess>

PWRequestManager::PWRequestManager(QObject *parent)
    : QObject(parent)
{
    // Connect to the sslErrors signal in order to see what errors we get when connecting to the push initiator
    connect(&m_accessManager,SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>& )),
            this, SLOT(onSslErrors(QNetworkReply*, const QList<QSslError>&)));

    connect(&m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpFinished(QNetworkReply*)));

}

PWRequestManager::~PWRequestManager()
{
    for(QMap<QNetworkReply *, PWRequest *>::iterator it = requestMap.begin(); it != requestMap.end(); ++it)
    {
        QNetworkReply * reply = it.key();
        PWRequest * request = it.value();
        delete reply;
        delete request;
    }

    requestMap.clear();
}

void PWRequestManager::sendRequest(PWRequest *request)
{
    QUrl url("https://cp.pushwoosh.com/json/1.3/" + request->methodName());

    qDebug() << "URL: " << url;

    QVariantMap map = request->requestDictionary();

    QString reqString;
    bb::data::JsonDataAccess jda;
    jda.saveToBuffer(map, &reqString);

    QString requestString = "{\"request\":";
    requestString += reqString;
    requestString += "}";

    qDebug() << "URL: " << requestString;

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    QByteArray byteArray;
    byteArray.append(requestString);
    QNetworkReply *reply = m_accessManager.post(req, byteArray);
    requestMap[reply] = request;
}

void PWRequestManager::httpFinished(QNetworkReply * reply)
{
    qDebug() << "httpFinished called";

    PWRequest * request = requestMap[reply];
    requestMap.remove(reply);
    if(!request)
    {
        request->deleteLater();
        reply->deleteLater();
        return;
    }

    int code = -1;
    QString description;

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if (status != 200)
    {
       QString reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
       request->errorDescription = reason;
       qDebug() << reason;

       //give empty map to parse response and trigger finish event
       QVariantMap map;
       request->parseResponse(map);
    }
    else
    {
        // Load the data using the reply QIODevice
        const QString resultData = QString::fromUtf8(reply->readAll());
        qDebug() << "response: " << resultData;

        bb::data::JsonDataAccess jda;
        QVariant jsonVar = jda.loadFromBuffer(resultData);
        QVariantMap jsonMap = jsonVar.value<QVariantMap>();
        request->parseResponse(jsonMap);
    }

    reply->deleteLater();
    request->deleteLater();
}

void PWRequestManager::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
    // Ignore all SSL errors here to be able to load from a secure address.
    // It might be a good idea to display an error message indicating that security may be compromised.
    // The errors we get are:
    // "SSL error: The issuer certificate of a locally looked up certificate could not be found"
    // "SSL error: The root CA certificate is not trusted for this purpose"
    // Seems to be a problem with how the server is set up and a known QT issue QTBUG-23625

    qDebug() << "onSslErrors called";
    //reply->ignoreSslErrors(errors);
}
