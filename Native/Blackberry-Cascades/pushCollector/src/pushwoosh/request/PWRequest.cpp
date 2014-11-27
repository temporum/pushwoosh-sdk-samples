//
//  PWRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"
#include <bb/device/HardwareInfo>

QVariantMap PWRequest::requestDictionary()const
{
    bb::device::HardwareInfo info;

    QVariantMap map;
    map.insert("application", appId);
    map.insert("hwid", info.pin().toUpper());

    return map;
}

void PWRequest::parseResponse(const QVariantMap & jsonMap)
{
    //get Pushwoosh code
    int statusCode = jsonMap["status_code"].value<int>();
    qDebug() << "statusCode: " << statusCode;

    if(statusCode != 200)
    {
        qDebug() << "request error";
        response = jsonMap;
        errorDescription = jsonMap["status_message"].value<QString>();
    }
    else
    {
        valid = true;
        internalParseResponse(jsonMap);
    }

    emit requestFinished(this);
}
