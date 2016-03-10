

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, DroneState) {
    DroneStateHover,
    DroneStateTakeOffOrLand,
    DroneStateForward,
    DroneStateBackward,
    DroneStateClimb,
    DroneStateDescend,
    DroneStateLeftRoll,
    DroneStateRightRoll,
    DroneStateUnknown
};
#define droneValueString(enum) [@[@"DroneStateHover",@"DroneStateTakeOffOrLand",@"DroneStateForward",@"DroneStateBackward",@"DroneStateClimb",@"DroneStateDescend",@"DroneStateLeftRoll",@"DroneStateRightRoll",@"DroneStateUnknown"] objectAtIndex:enum]

@interface JRMWatchToDroneGestureRecognizer : NSObject

- (DroneState)detectStateWithX:(double)x y:(double)y z:(double)z;

@end
