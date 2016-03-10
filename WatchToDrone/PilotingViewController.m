
#import "PilotingViewController.h"
#import <libARDiscovery/ARDiscovery.h>
#import <libARController/ARController.h>
#import <uthash/uthash.h>

@import WatchConnectivity;

void stateChanged (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData);
void onCommandReceived (eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);

@interface PilotingViewController () <WCSessionDelegate>

@property (nonatomic) ARCONTROLLER_Device_t *deviceController;
@property (nonatomic) dispatch_semaphore_t stateSem;
@property (nonatomic, strong) UIAlertController *alertController;

@end

@implementation PilotingViewController

#pragma mark - View lifecycle

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.deviceController = NULL;
    self.stateSem = dispatch_semaphore_create(0);
    
    self.alertController = [UIAlertController alertControllerWithTitle:[self.service name] message:@"Connecting.." preferredStyle:UIAlertControllerStyleAlert];
    
    if ([WCSession isSupported]) {
        WCSession *session = [WCSession defaultSession];
        session.delegate = self;
        [session activateSession];
    }
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [[UIApplication sharedApplication] setIdleTimerDisabled:NO];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    [self presentViewController:self.alertController animated:YES completion:nil];
    
    // create the device controller
    [self createDeviceControllerWithService:self.service];
}

- (void)viewDidDisappear:(BOOL)animated {
    if (self.alertController && !self.alertController.isBeingPresented) {
        [self dismissViewControllerAnimated:NO completion:nil];
    }
    self.alertController = [UIAlertController alertControllerWithTitle:[self.service name] message:@"Disconnecting.." preferredStyle:UIAlertControllerStyleAlert];
    [self presentViewController:self.alertController animated:YES completion:nil];
    
    // in background
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
        
        // if the device controller is not stopped, stop it
        eARCONTROLLER_DEVICE_STATE state = ARCONTROLLER_Device_GetState(_deviceController, &error);
        if ((error == ARCONTROLLER_OK) && (state != ARCONTROLLER_DEVICE_STATE_STOPPED)) {
            // after that, stateChanged should be called soon
            error = ARCONTROLLER_Device_Stop (_deviceController);
            
            if (error != ARCONTROLLER_OK) {
                NSLog(@"- error :%s", ARCONTROLLER_Error_ToString(error));
            }
            else {
                // wait for the state to change to stopped
                NSLog(@"- wait new state ... ");
                dispatch_semaphore_wait(_stateSem, DISPATCH_TIME_FOREVER);
            }
        }
        
        // once the device controller is stopped, we can delete it
        if (_deviceController != NULL) {
            ARCONTROLLER_Device_Delete(&_deviceController);
        }
        
        // dismiss the alert view in main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            [self dismissViewControllerAnimated:NO completion:nil];
        });
    });
}

#pragma mark WCSessionDelegate methods

- (void)session:(WCSession *)session didReceiveMessage:(NSDictionary<NSString *,id> *)message {
    if ((message[@"watchAccData"]) != nil) {
        
        // Incoming accelerometer data
        NSString *dataString = message[@"watchAccData"];
        NSArray *components = [dataString componentsSeparatedByString:@","];
        double x = [components[0] doubleValue];
        double y = [components[1] doubleValue];
        double z = [components[2] doubleValue];
        NSLog(@"X:%f Y:%f Z:%f",x,y,z);
        
        
    } else if (message[@"workoutState"]) {
        NSLog(@"New HKWorkoutSession state: %@",message[@"workoutState"]);
    } else if (message[@"error"]) {
        NSLog(@"Error: %@",message[@"error"]);
    }
//    if ((message[@"request"]) != nil) {
//        NSString *dataString = message[@"request"];
//        NSArray *components = [dataString componentsSeparatedByString:@","];
//        double x = [components[0] doubleValue];
//        double y = [components[1] doubleValue];
//        double z = [components[2] doubleValue];
//        NSLog(@"x:%f y:%f z:%f",x,y,z);
//        DroneState newState = [self.gestureRecognizer detectStateWithX:x y:y z:z];
//        if (newState == DroneStateTakeOffOrLand && self.currentState != DroneStateTakeOffOrLand) {
//            if (!self.hasTakenOff) {
//                [self takeOff];
//                self.hasTakenOff = YES;
//            } else {
//                [self land];
//                self.hasTakenOff = NO;
//            }
//            self.currentState = DroneStateTakeOffOrLand;
//        } else if (newState == DroneStateForward && self.currentState != DroneStateForward && self.hasTakenOff) {
//            [self goToHover];
//            [self pitchForwardTouchDown:nil];
//            NSLog(@"sent forward command");
//            self.currentState = DroneStateForward;
//        } else if (newState == DroneStateBackward && self.currentState != DroneStateBackward && self.hasTakenOff) {
//            [self goToHover];
//            [self pitchBackTouchDown:nil];
//            NSLog(@"sent backward command");
//            self.currentState = DroneStateBackward;
//        } else if (newState == DroneStateLeftRoll && self.currentState != DroneStateLeftRoll && self.hasTakenOff) {
//            [self goToHover];
//            [self rollLeftTouchDown:nil];
//            NSLog(@"sent left command");
//            self.currentState = DroneStateLeftRoll;
//        } else if (newState == DroneStateRightRoll && self.currentState != DroneStateRightRoll && self.hasTakenOff) {
//            [self goToHover];
//            [self rollRightTouchDown:nil];
//            NSLog(@"sent right command");
//            self.currentState = DroneStateRightRoll;
//        } else if (newState == DroneStateClimb && self.currentState != DroneStateClimb && self.hasTakenOff) {
//            [self goToHover];
//            [self gazUpTouchDown:nil];
//            NSLog(@"sent up command");
//            self.currentState = DroneStateClimb;
//        } else if (newState == DroneStateDescend && self.currentState != DroneStateDescend && self.hasTakenOff) {
//            [self goToHover];
//            NSLog(@"sent down command");
//            [self gazDownTouchDown:nil];
//            self.currentState = DroneStateDescend;
//        } else if ((newState == DroneStateHover || newState == DroneStateUnknown) && self.currentState != DroneStateHover && self.hasTakenOff) {
//            [self goToHover];
//            self.currentState = DroneStateHover;
//        } else {
//            self.currentState = DroneStateUnknown;
//        }
//    }
//    
}



#pragma mark Device controller callbacks

- (void)createDeviceControllerWithService:(ARService*)service {
    // first get a discovery device
    ARDISCOVERY_Device_t *discoveryDevice = [self createDiscoveryDeviceWithService:service];
    
    if (discoveryDevice != NULL) {
        eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
        
        // create the device controller
        NSLog(@"- ARCONTROLLER_Device_New ... ");
        _deviceController = ARCONTROLLER_Device_New (discoveryDevice, &error);
        
        if ((error != ARCONTROLLER_OK) || (_deviceController == NULL)) {
            NSLog(@"- error :%s", ARCONTROLLER_Error_ToString(error));
        }
        
        // add the state change callback to be informed when the device controller starts, stops...
        if (error == ARCONTROLLER_OK) {
            NSLog(@"- ARCONTROLLER_Device_AddStateChangedCallback ... ");
            error = ARCONTROLLER_Device_AddStateChangedCallback(_deviceController, stateChanged, (__bridge void *)(self));
            
            if (error != ARCONTROLLER_OK) {
                NSLog(@"- error :%s", ARCONTROLLER_Error_ToString(error));
            }
        }
        
        // add the command received callback to be informed when a command has been received from the device
        if (error == ARCONTROLLER_OK) {
            NSLog(@"- ARCONTROLLER_Device_AddCommandRecievedCallback ... ");
            error = ARCONTROLLER_Device_AddCommandReceivedCallback(_deviceController, onCommandReceived, (__bridge void *)(self));
            
            if (error != ARCONTROLLER_OK) {
                NSLog(@"- error :%s", ARCONTROLLER_Error_ToString(error));
            }
        }
        
        // start the device controller (the callback stateChanged should be called soon)
        if (error == ARCONTROLLER_OK) {
            NSLog(@"- ARCONTROLLER_Device_Start ... ");
            error = ARCONTROLLER_Device_Start (_deviceController);
            
            if (error != ARCONTROLLER_OK) {
                NSLog(@"- error :%s", ARCONTROLLER_Error_ToString(error));
            }
        }
        
        // we don't need the discovery device anymore
        ARDISCOVERY_Device_Delete (&discoveryDevice);
        
        // if an error occured, go back
        if (error != ARCONTROLLER_OK) {
            [self goBack];
        }
    }
}

- (ARDISCOVERY_Device_t *)createDiscoveryDeviceWithService:(ARService*)service {
    ARDISCOVERY_Device_t *device = NULL;
    
    eARDISCOVERY_ERROR errorDiscovery = ARDISCOVERY_OK;
    
    NSLog(@"- Init discovery device  ... ");
    
    device = ARDISCOVERY_Device_New (&errorDiscovery);
    if ((errorDiscovery != ARDISCOVERY_OK) || (device == NULL)) {
        NSLog(@"device : %p", device);
        NSLog(@"Discovery error :%s", ARDISCOVERY_Error_ToString(errorDiscovery));
    }
    
    if (errorDiscovery == ARDISCOVERY_OK) {
        // get the ble service from the ARService
        ARBLEService* bleService = service.service;
        
        // create a discovery device (ARDISCOVERY_PRODUCT_MINIDRONE)
        errorDiscovery = ARDISCOVERY_Device_InitBLE (device, ARDISCOVERY_PRODUCT_MINIDRONE, (__bridge ARNETWORKAL_BLEDeviceManager_t)(bleService.centralManager), (__bridge ARNETWORKAL_BLEDevice_t)(bleService.peripheral));
        
        if (errorDiscovery != ARDISCOVERY_OK) {
            NSLog(@"Discovery error :%s", ARDISCOVERY_Error_ToString(errorDiscovery));
        }
    }
    
    return device;
}

- (void)goBack {
    [self.navigationController popViewControllerAnimated:YES];
}

// called when the state of the device controller has changed
void stateChanged (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData) {
    PilotingViewController *pilotingViewController = (__bridge PilotingViewController *)customData;
    
    NSLog (@"newState: %d",newState);
    
    if (pilotingViewController != nil) {
        switch (newState) {
            case ARCONTROLLER_DEVICE_STATE_RUNNING: {
                // dismiss the alert view in main thread
                dispatch_async(dispatch_get_main_queue(), ^{
                    [pilotingViewController dismissViewControllerAnimated:YES completion:nil];
                });
                break;
            }
            case ARCONTROLLER_DEVICE_STATE_STOPPED: {
                dispatch_semaphore_signal(pilotingViewController.stateSem);
                
                // Go back
                dispatch_async(dispatch_get_main_queue(), ^{
                    [pilotingViewController goBack];
                });
                
                break;
            }
                
            case ARCONTROLLER_DEVICE_STATE_STARTING:
                break;
                
            case ARCONTROLLER_DEVICE_STATE_STOPPING:
                break;
                
            default:
                NSLog(@"new State : %d not known", newState);
                break;
        }
    }
}

// called when a command has been received from the drone
void onCommandReceived (eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData) {
    PilotingViewController *pilotingViewController = (__bridge PilotingViewController *)customData;
    
    // if the command received is a battery state changed
    if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED) && (elementDictionary != NULL)) {
        ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
        ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
        
        // get the command received in the device controller
        HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
        if (element != NULL) {
            // get the value
            HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED_PERCENT, arg);
            
            if (arg != NULL) {
                // update UI
                [pilotingViewController onUpdateBatteryLevel:arg->value.U8];
            }
        }
    }
}

#pragma mark UI updates from commands
- (void)onUpdateBatteryLevel:(uint8_t)percent;
{
    NSLog(@"onUpdateBattery ...");
    
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString *text = [[NSString alloc] initWithFormat:@"%d%%", percent];
        [self.batteryLabel setText:text];
    });
}

@end
