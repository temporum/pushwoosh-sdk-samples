//
//  PWPushStatRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWPushStatRequest.h"

QString PWPushStatRequest::methodName()const
{
    return "pushStat";
}

QVariantMap PWPushStatRequest::requestDictionary()const
{
    QVariantMap map = PWRequest::requestDictionary();

    map.insert("hash", hash);
    return map;
}
