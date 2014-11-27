//
//  PWApplicationEventRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWApplicationEventRequest : public PWRequest
{
    QString goal;
    int count;
public:
    PWApplicationEventRequest(const QString & appId, const QString & goal, int count = -1)
    :   PWRequest(appId),
        goal(goal),
        count(count)
    {}

    virtual QString methodName()const;

    virtual QVariantMap requestDictionary()const;
};
