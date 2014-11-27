//
//  PWGetTagsRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include <QSystemLocale>
#include <time.h>
#include "PWGetTagsRequest.h"

#include <bps/bps.h>
#include <bps/deviceinfo.h>
#include <bb/data/JsonDataAccess>
#include <bb/ApplicationInfo>

QString PWGetTagsRequest::methodName()const
{
    return "getTags";
}

void PWGetTagsRequest::internalParseResponse(const QVariantMap & reponse)
{
    if(reponse["response"] == 0)
        return;

    QVariantMap responseMap = reponse["response"].value<QVariantMap>();
    if(responseMap["result"] == 0)
        return;

    tags = responseMap["result"].value<QVariantMap>();
}

QVariantMap PWGetTagsRequest::requestDictionary()const
{
    QVariantMap map = PWRequest::requestDictionary();
    return map;
}
