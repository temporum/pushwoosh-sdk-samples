//
//  PWRegisterDeviceRequest
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include <QSystemLocale>
#include <time.h>
#include "PWRegisterDeviceRequest.h"

#include <bps/bps.h>
#include <bps/deviceinfo.h>
#include <bb/data/JsonDataAccess>
#include <bb/ApplicationInfo>

static QString deviceVersion()
{
    QString deviceVersion;

    if (bps_initialize() == BPS_SUCCESS) {
        qDebug() << "bps initialized";
        deviceinfo_details_t *deviceDetails = 0;

        if (deviceinfo_get_details(&deviceDetails) == BPS_SUCCESS) {
            deviceVersion = deviceinfo_details_get_device_os_version(deviceDetails);
            deviceinfo_free_details(&deviceDetails);
        } else {
            qDebug() << "error retrieving device details";
        }

        bps_shutdown();
    } else {
        qDebug() << "error initializing bps";
    }

    return deviceVersion;
}

static QString deviceModel()
{
    QString deviceModel;

    if (bps_initialize() == BPS_SUCCESS) {
        qDebug() << "bps initialized";
        deviceinfo_details_t *deviceDetails = 0;

        if (deviceinfo_get_details(&deviceDetails) == BPS_SUCCESS) {
            deviceModel = deviceinfo_details_get_model_name(deviceDetails);
            deviceinfo_free_details(&deviceDetails);
        } else {
            qDebug() << "error retrieving device details";
        }

        bps_shutdown();
    } else {
        qDebug() << "error initializing bps";
    }

    return deviceModel;
}

QString PWRegisterDeviceRequest::methodName()const
{
    return "registerDevice";
}

QVariantMap PWRegisterDeviceRequest::requestDictionary()const
{
    QVariantMap map = PWRequest::requestDictionary();

    map.insert("device_type", 2);
    map.insert("push_token", pushToken);

    const QString localeString = QLocale().name();
    map.insert("language", localeString.left(2));

    map.insert("timezone", (int)timezone);
    map.insert("os_version", deviceVersion());
    map.insert("device_model", deviceModel());

    bb::ApplicationInfo appinfo;
    QString version = appinfo.version();

    map.insert("app_version", version);

    return map;
}
