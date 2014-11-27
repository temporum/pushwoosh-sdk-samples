//
//  PWRegisterDeviceRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWRegisterDeviceRequest : public PWRequest
{
    QString pushToken;

public:
    PWRegisterDeviceRequest(const QString & appId, const QString & pushToken)
    :   PWRequest(appId),
        pushToken(pushToken)
    {}

    const QString & getToken()const
    {
        return pushToken;
    }

    virtual QString methodName()const;
    virtual QVariantMap requestDictionary()const;
};
