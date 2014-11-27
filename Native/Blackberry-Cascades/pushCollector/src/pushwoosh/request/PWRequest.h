//
//  PWRequest.h
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#pragma once

#include <QtCore/QString>
#include <QtCore/QVariantMap>

class PWRequest : public QObject
{
    Q_OBJECT

    QString appId;

    bool valid;
    QString errorDescription;
    QVariantMap response;

    friend class PWRequestManager;

protected:
    virtual void internalParseResponse(const QVariantMap &)
    {}

public:
    PWRequest(const QString & appId)
    :   QObject(0),
        appId(appId),
        valid(false)
    {}

    virtual ~PWRequest()
    {}

    bool isValid()const
    {
        return valid;
    }

    QString getError()const
    {
        return errorDescription;
    }

    virtual QString methodName()const = 0;
    virtual QVariantMap requestDictionary()const;

    void parseResponse(const QVariantMap & jsonMap);

signals:
    void requestFinished(PWRequest * request);
};
