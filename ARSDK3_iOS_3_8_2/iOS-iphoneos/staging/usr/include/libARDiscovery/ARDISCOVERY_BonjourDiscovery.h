/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
//
//  ARDiscovery.h
//  ARSDK 3
//
//  Created by Nicolas BRULEZ on 08/03/13.
//  Copyright (c) 2013 Parrot SA. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#include <libARSAL/ARSAL_CentralManager.h>
#import <libARDiscovery/ARDISCOVERY_Discovery.h>

#pragma mark Notifications

/**
 * Constant for devices services list updates notification
 * userInfo is a NSDictionnary with the following content:
 *  - key   : kARDiscoveryServicesList
 *  - value : NSArray of NSNetService
 */
#define kARDiscoveryNotificationServicesDevicesListUpdated      @"kARDiscoveryNotificationServicesDevicesListUpdated"
#define kARDiscoveryServicesList                                @"kARDiscoveryServicesList"

/**
 * Constant for controller services list updates notification
 * userInfo is a NSDictionnary with the following content:
 *  - key   : kARDiscoveryServicesList
 *  - value : NSArray of NSNetService
 */
#define kARDiscoveryNotificationServicesControllersListUpdated  @"kARDiscoveryNotificationServicesControllersListUpdated"

/**
 * Constant for publication notifications
 * userInfo is a NSDictionnary with the following content:
 *  - key   : kARDiscoveryServiceName
 *  - value : NSString with the name of the published service
 *            or @"" if no service is published
 */
#define kARDiscoveryNotificationServicePublished                @"kARDiscoveryNotificationServicePublished"
#define kARDiscoveryServiceName                                 @"kARDiscoveryServiceName"

/**
 * Constant for service resolution notifications
 * userInfo is a NSDictionnary with the following content:
 *  - key   : kARDiscoveryServiceResolved
 *  - value : NSNetService which was resolved
 */
#define kARDiscoveryNotificationServiceResolved                 @"kARDiscoveryNotificationServiceResolved"
#define kARDiscoveryServiceResolved                             @"kARDiscoveryServiceResolved"

/**
 * Constant for service resolution notifications
 * userInfo is always nil:
 */
#define kARDiscoveryNotificationServiceNotResolved              @"kARDiscoveryNotificationServiceNotResolved"

#pragma mark ARDiscovery interface
@interface ARBLEService : NSObject
@property (nonatomic, strong) ARSAL_CentralManager *centralManager;
@property (nonatomic, strong) CBPeripheral *peripheral;
@end

@interface ARService : NSObject
@property (nonatomic, strong) NSString *name;               ///< Name of the device
@property (nonatomic, assign) eARDISCOVERY_PRODUCT product; ///< Specific product
@property (nonatomic, strong) id service;                   ///< Can be NSNetService or ARBLEService
@property (nonatomic, strong) NSNumber *signal;
@end

@interface ARDiscovery : NSObject

+ (ARDiscovery *)sharedInstance;

#pragma mark Start / Stop
/**
 * Starts ARDiscovery if it isntt discovering
 */
- (void)start;

/**
 * Pause ARDiscovery BLE if it's discovering
 */
- (void)pauseBLE;

/**
 * Stops ARDiscovery if it's discovering
 */
- (void)stop;

#pragma mark - Getters

/**
 * Get the current list of controllers services
 * Returns a NSArray of NSNetServices
 */
- (NSArray *)getCurrentListOfControllersServices;

/**
 * Get the current list of devices services
 * Returns a NSArray of NSNetServices or ARBLEServices
 */
- (NSArray *)getCurrentListOfDevicesServices;

/**
 * Get the name of the currently published service
 * Returns @"" if no service is published
 */
- (NSString *)getCurrentPublishedServiceName;

#pragma mark - Resolve

/**
 * Remove an existing service from the list of known services.
 * Service will be re-added next time it's discovered.
 */
- (void)removeDeviceService:(ARService*)aService;

/**
 * Try to resolve the given service
 * Resolution is queued until all previous resolutions
 * are complete, or failed
 */
- (void)resolveService:(ARService *)aService;

/**
 * Convert the given service to resolved IP
 * You must to resolve service before to Convert it to IP
 */
- (NSString *)convertNSNetServiceToIp:(ARService *)service;

#pragma mark - Publication

/**
 * Publish a new controller service with the given name
 * Calling this function will unpublish any previous service
 * If serviceName is not unique, it will be postfixed with a number
 *  --> "MyServiceName" will become "MyServiceName1", "MyServiceName2" ...
 */
- (void)publishControllerServiceWithName:(NSString *)serviceName;

/**
 * Stop publication of current service
 * Does nothing if no service is currently published
 */
- (void)unpublishService;

@end
