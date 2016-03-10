

#import "InterfaceController.h"
#import <CoreMotion/CoreMotion.h>
#import <WatchConnectivity/WatchConnectivity.h>
#import <HealthKit/HealthKit.h>


@interface InterfaceController() <WCSessionDelegate,HKWorkoutSessionDelegate>

@property (nonatomic, weak) IBOutlet WKInterfaceLabel *xLabel;
@property (nonatomic, weak) IBOutlet WKInterfaceLabel *yLabel;
@property (nonatomic, weak) IBOutlet WKInterfaceLabel *zLabel;
@property (nonatomic, weak) IBOutlet WKInterfaceButton *sessionButton;

@property (nonatomic, strong) CMMotionManager *motionManager;
@property (nonatomic, strong) NSOperationQueue *motionQueue;
@property (nonatomic, strong) HKHealthStore *healthStore;
@property (nonatomic, strong) HKWorkoutSession *session;

@end


@implementation InterfaceController

#pragma mark - Interface lifecycle

- (void)awakeWithContext:(id)context {
    [super awakeWithContext:context];
    
    self.motionQueue = [NSOperationQueue mainQueue];
    
    if ([WCSession isSupported]) {
        WCSession *session = [WCSession defaultSession];
        session.delegate = self;
        [session activateSession];
    }
    self.motionManager = [[CMMotionManager alloc] init];
    self.motionManager.accelerometerUpdateInterval = 0.4;

    self.healthStore = [[HKHealthStore alloc] init];
}

- (void)willActivate {
    [super willActivate];
    
//    [[NSProcessInfo processInfo] performExpiringActivityWithReason:@"AccData" usingBlock:^(BOOL expired) {
//        [self.motionManager startAccelerometerUpdatesToQueue:self.motionQueue withHandler:^(CMAccelerometerData *accel, NSError *error){
//            CMAcceleration a = accel.acceleration;
//            [self.xLabel setText:[NSString stringWithFormat:@"X: %f",a.x]];
//            [self.yLabel setText:[NSString stringWithFormat:@"Y: %f",a.y]];
//            [self.zLabel setText:[NSString stringWithFormat:@"Z: %f",a.z]];
//            NSDictionary *message = @{@"watchAccData":[NSString stringWithFormat:@"%f,%f,%f",a.x,a.y,a.z]};
//            [[WCSession defaultSession] sendMessage:message replyHandler:nil errorHandler:nil];
//        }];
//    }];

}

- (void)didDeactivate {
    [super didDeactivate];
}

- (void)dealloc {
    [self.healthStore endWorkoutSession:self.session];
}

#pragma mark - HKWorkoutSessionDelegate methods

- (void)workoutSession:(HKWorkoutSession *)workoutSession didChangeToState:(HKWorkoutSessionState)toState fromState:(HKWorkoutSessionState)fromState date:(NSDate *)date {
    
    NSString *messageString;
    if (toState == HKWorkoutSessionStateNotStarted) {
        messageString = @"Workout Session Not Started";
    } else if (toState == HKWorkoutSessionStateEnded) {
        messageString = @"Ended Workout Session";
    } else if (toState == HKWorkoutSessionStateRunning) {
        messageString = @"Running Workout Session";
    }
    
    NSDictionary *message = @{@"workoutState":messageString};
    [[WCSession defaultSession] sendMessage:message replyHandler:nil errorHandler:nil];
    
    [[NSProcessInfo processInfo] performExpiringActivityWithReason:@"AccData" usingBlock:^(BOOL expired) {
        [self.motionManager startAccelerometerUpdatesToQueue:self.motionQueue withHandler:^(CMAccelerometerData *accel, NSError *error){
            CMAcceleration a = accel.acceleration;
            [self.xLabel setText:[NSString stringWithFormat:@"X: %f",a.x]];
            [self.yLabel setText:[NSString stringWithFormat:@"Y: %f",a.y]];
            [self.zLabel setText:[NSString stringWithFormat:@"Z: %f",a.z]];
            NSDictionary *message = @{@"watchAccData":[NSString stringWithFormat:@"%f,%f,%f",a.x,a.y,a.z]};
            [[WCSession defaultSession] sendMessage:message replyHandler:nil errorHandler:nil];
        }];
    }];
}

- (void)workoutSession:(HKWorkoutSession *)workoutSession didFailWithError:(NSError *)error {
    NSDictionary *message = @{@"error":error.localizedDescription};
    [[WCSession defaultSession] sendMessage:message replyHandler:nil errorHandler:nil];
}


#pragma mark - Button actions

- (IBAction)sessionButtonTapped:(id)sender {
    if (self.session) {
        [self.healthStore endWorkoutSession:self.session];
        self.session = nil;
        [self.sessionButton setTitle:@"Start Session"];
    } else {
        self.session = [[HKWorkoutSession alloc] initWithActivityType:HKWorkoutActivityTypeRunning locationType:HKWorkoutSessionLocationTypeIndoor];
        self.session.delegate = self;
        [self.healthStore startWorkoutSession:self.session];
        [self.sessionButton setTitle:@"Stop Session"];
    }
}

@end



