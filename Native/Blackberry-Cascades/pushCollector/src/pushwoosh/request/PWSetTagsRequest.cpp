//
//  PWSetTagsRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWSetTagsRequest.h"

QString PWSetTagsRequest::methodName()const
{
    return "setTags";
}

QVariantMap PWSetTagsRequest::requestDictionary()const
{
    QVariantMap map = PWRequest::requestDictionary();
    map.insert("tags", tags);
    return map;
}
