//
//  PWAppOpenRequest.h
//  Pushwoosh SDK
//  (c) Pushwoosh 2014
//

#include "PWRequest.h"

class PWAppOpenRequest : public PWRequest
{
public:
    PWAppOpenRequest(const QString & appId)
    :   PWRequest(appId)
    {}

    virtual QString methodName()const;
};
