//
//  PWUnregisterDeviceRequest.h
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWUnregisterDeviceRequest : public PWRequest
{
public:
    PWUnregisterDeviceRequest(const QString & appId)
    :   PWRequest(appId)
    {}

    virtual QString methodName()const;
};
