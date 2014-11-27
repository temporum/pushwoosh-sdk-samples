//
//  PWSetTagsRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWSetTagsRequest : public PWRequest
{
    QVariantMap tags;

public:
    PWSetTagsRequest(const QString & appId, const QVariantMap & tags)
    :   PWRequest(appId),
        tags(tags)
    {}

    virtual QString methodName()const;
    virtual QVariantMap requestDictionary()const;
};
