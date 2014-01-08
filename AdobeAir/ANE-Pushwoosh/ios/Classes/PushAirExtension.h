//
//  PushAirExtension.h
//  Pushwoosh SDK
//  (c) Pushwoosh 2013
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "PushNotificationManager.h"
#import "FlashRuntimeExtensions.h"

void PushwooshContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctionsToTest, const FRENamedFunction** functionsToSet);
void PushwooshContextFinalizer(FREContext ctx);
void PushwooshExtInitializer(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, FREContextFinalizer* ctxFinalizerToSet );
void PushwooshExtFinalizer(void *extData);
