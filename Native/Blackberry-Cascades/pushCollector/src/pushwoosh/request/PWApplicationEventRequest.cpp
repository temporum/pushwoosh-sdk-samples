//
//  PWApplicationEventRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWApplicationEventRequest.h"

QString PWApplicationEventRequest::methodName()const
{
    return "applicationEvent";
}

QVariantMap PWApplicationEventRequest::requestDictionary()const
{
    QVariantMap map = PWRequest::requestDictionary();

    map.insert("goal", goal);
    if(count != -1)
        map.insert("count", count);

    return map;
}
