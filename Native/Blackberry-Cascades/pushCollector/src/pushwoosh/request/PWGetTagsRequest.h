//
//  PWGetTagsRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWGetTagsRequest : public PWRequest
{
    QVariantMap tags;

public:
    PWGetTagsRequest(const QString & appId)
    :   PWRequest(appId)
    {}

    virtual void internalParseResponse(const QVariantMap & reposne);

    const QVariantMap & getTags()const
    {
        return tags;
    }

    virtual QString methodName()const;
    virtual QVariantMap requestDictionary()const;
};
