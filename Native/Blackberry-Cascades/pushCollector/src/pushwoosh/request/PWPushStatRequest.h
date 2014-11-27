//
//  PWPushStatRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWPushStatRequest : public PWRequest
{
    QString hash;
public:
    PWPushStatRequest(const QString & appId, const QString & hash)
    :   PWRequest(appId),
        hash(hash)
    {}

    virtual QString methodName()const;
    virtual QVariantMap requestDictionary()const;
};
