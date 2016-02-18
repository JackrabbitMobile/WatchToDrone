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
 * @file   ARMediaObject.h
 * @author malick.sylla.ext@parrot.fr
 * @brief
 *
 **/
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <ImageIO/CGImageProperties.h>
#import <CoreLocation/CoreLocation.h>

#import <libARDataTransfer/ARDataTransfer.h>
#import <libARMedia/ARMedia.h>

@class ARMediaObject;

@protocol ARMediaObjectDelegate <NSObject>

- (void)mediaObjectDidUpdateThumbnail:(ARMediaObject*)mediaObject;

@end

@interface ARMediaObject : NSObject <NSCoding, NSCopying>

@property (nonatomic, weak)   id<ARMediaObjectDelegate> delegate;
@property (nonatomic, assign) NSUInteger index;
@property (nonatomic, strong) NSString *runDate;
@property (nonatomic, strong) NSString *productId;
@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *date;
@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSNumber *size;
@property (nonatomic, strong) UIImage *thumbnail;
@property (nonatomic, strong) NSNumber *mediaType;
@property (nonatomic, strong) NSURL *assetUrl;
@property (nonatomic, strong) NSString *uuid;
@property (nonatomic, assign) BOOL cloud;

- (void)updateDataTransferMedia:(ARDATATRANSFER_Media_t *)media withIndex:(NSUInteger)index;
- (void)updateThumbnailWithARDATATRANSFER_Media_t:(ARDATATRANSFER_Media_t *)media;
- (void)updateThumbnailWithNSUrl:(NSURL *)assetUrl;
@end
