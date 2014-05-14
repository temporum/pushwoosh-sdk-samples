//
//  AppDelegate.m
//  GeoTagsSampleApp
//
//  Created by Vladislav Zozulyak on 31.07.12.
//  Copyright (c) 2012 exeshneg@gmail.com. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import <Pushwoosh/PushNotificationManager.h>

#define LOCATIONS_FILE @"PWLocationTracking"
#define LOCATIONS_FILE_TYPE @"log"

@interface AppDelegate ()
@property (nonatomic, strong) PWLocationTracker *locationTracker;
@end

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;
@synthesize navController;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
    self.viewController = [[[ViewController alloc] initWithNibName:@"ViewController" bundle:nil] autorelease];
    
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
	
	//-----------PUSHWOOSH PART-----------
	// set custom delegate for push handling, in our case - view controller
	PushNotificationManager * pushManager = [PushNotificationManager pushManager];
	pushManager.delegate = self.viewController;
	
	// handling push on app start
	[[PushNotificationManager pushManager] handlePushReceived:launchOptions];
	
	// make sure we count app open in Pushwoosh stats
	[[PushNotificationManager pushManager] sendAppOpen];
	
	// register for push notifications!
	[[PushNotificationManager pushManager] registerForPushNotifications];
    
    return YES;
}

// system push notification registration success callback, delegate to pushManager
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
	[[PushNotificationManager pushManager] handlePushRegistration:deviceToken];
}

// system push notification registration error callback, delegate to pushManager
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
	[[PushNotificationManager pushManager] handlePushRegistrationFailure:error];
}

// system push notifications callback, delegate to pushManager
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
	[[PushNotificationManager pushManager] handlePushReceived:userInfo];
}

// silent push handling for applications with the "remote-notification" background mode
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
	
    NSDictionary *pushDict = [userInfo objectForKey:@"aps"];
    BOOL isSilentPush = [[pushDict objectForKey:@"content-available"] boolValue];
    
    if (isSilentPush) {
        NSLog(@"Silent push notification:%@", userInfo);
        
        //load content here
        
		// must call completionHandler
        completionHandler(UIBackgroundFetchResultNewData);
    }
    else {
        [[PushNotificationManager pushManager] handlePushReceived:userInfo];

		// must call completionHandler
        completionHandler(UIBackgroundFetchResultNoData);
    }
}

- (void)dealloc
{
	self.navController = nil;
	[_window release];
	[_viewController release];
    [super dealloc];
}

+ (AppDelegate *) sharedDelegate {
	return (AppDelegate *) [UIApplication sharedApplication].delegate;
}

@end
