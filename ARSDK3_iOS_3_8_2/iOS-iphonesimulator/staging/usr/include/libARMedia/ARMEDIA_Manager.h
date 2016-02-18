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
/**
 * @file   ARMEDIA_Manager.h
 * @author malick.sylla.ext@parrot.fr
 * @brief
 */
#import <Foundation/Foundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <ImageIO/CGImageProperties.h>
#import <CoreLocation/CoreLocation.h>

 /**
  * @brief status return by the callback.
 */
typedef enum 
{
    ARMEDIA_MANAGER_OK = 0,
    ARMEDIA_MANAGER_ALREADY_INITIALIZED,
    ARMEDIA_MANAGER_NOT_INITIALIZED
} eARMEDIA_MANAGER_ERROR;

/**
 * @brief notification key post.
 */
UIKIT_EXTERN NSString *const kARMediaManagerNotificationInitialized;
UIKIT_EXTERN NSString *const kARMediaManagerNotificationUpdating;
UIKIT_EXTERN NSString *const kARMediaManagerNotificationUpdated;
UIKIT_EXTERN NSString *const kARMediaManagerNotificationMediaAdded;
UIKIT_EXTERN NSString *const kARMediaManagerNotificationEndOfMediaAdding;
UIKIT_EXTERN NSString *const kARMediaManagerNotificationAccesDenied;
UIKIT_EXTERN NSString *const kARMediaManagerDefaultSkyControllerDateKey;

@interface ARMediaManager : NSObject
/**
 init a ARMedia_Manager Singleton.
 @return ARMedia_Manager Singleton.
 */
+ (ARMediaManager *)sharedInstance;
/**
 init a ARMedia_Manager with NSArray of NSString projectIDs.
 @param  NSArray for array of project.
 @return eARMEDIA_MANAGER_ERROR for enum of status.
 */
- (eARMEDIA_MANAGER_ERROR)initWithProjectIDs:(NSArray *)projectIDs;
/**
 update media of ARMedia_Manager library.
 @return eARMEDIA_MANAGER_ERROR for enum of status.
 */
- (eARMEDIA_MANAGER_ERROR)update;
/**
 add a media in a queue ARMedia_Manager.
 @param NSString for mediaPath.
 */
- (void)addMediaToQueue:(NSString *)mediaPath;
/**
 retreive the refresh project media dictionary.
 @param NSString for project to get, if nil all project are get.
 @return NSDictionary who containt media projectDictionary.
 */
- (NSDictionary *)getProjectDictionary:(NSString *)project;
/**
 @return Boolean if the ARMedia_Manager is update or not.
 */
- (BOOL)isUpdated;
/**
 @return Boolean if the ARMedia_Manager is updating or not.
 */
- (BOOL)isUpdating;

- (BOOL)isAdding;

@end