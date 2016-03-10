

#import "JRMWatchToDroneGestureRecognizer.h"

#define kAccelerometerPrecision 0.25
typedef NS_ENUM(NSInteger, Region) {
    RegionNegative,
    RegionZero,
    RegionPositive
};

@implementation JRMWatchToDroneGestureRecognizer

- (DroneState)detectStateWithX:(double)x y:(double)y z:(double)z {
    if ([self value:x isNear:1] && [self value:y isNear:0] && [self value:z isNear:0]) {
        return DroneStateDescend;
    } else if ([self value:x isNear:0] && [self value:y isNear:0] && [self value:z isNear:-1]) {
        return DroneStateHover;
    } else if ([self value:x isNear:-1] && [self value:y isNear:0] && [self value:z isNear:0]) {
        return DroneStateClimb;
    } else {
        Region xRegion = [self regionForValue:x withPrecision:kAccelerometerPrecision];
        Region yRegion = [self regionForValue:y withPrecision:2*kAccelerometerPrecision];
        Region zRegion = [self regionForValue:z withPrecision:kAccelerometerPrecision];
        if (xRegion == RegionNegative && yRegion == RegionZero) {
            return DroneStateBackward;
        } else if (xRegion == RegionPositive && yRegion == RegionZero && zRegion != RegionPositive) {
            return DroneStateForward;
        } else if (xRegion == RegionZero && yRegion == RegionNegative && zRegion != RegionPositive) {
            return DroneStateRightRoll;
        } else if (xRegion == RegionZero && yRegion == RegionPositive && zRegion != RegionPositive) {
            return DroneStateLeftRoll;
        } else if (zRegion == RegionPositive) {
            return DroneStateTakeOffOrLand;
        } else {
            return DroneStateUnknown;
        }
    }
}

- (BOOL)value:(double)value isNear:(double)nearValue {
    return ((value > (nearValue-kAccelerometerPrecision)) && (value < (nearValue+kAccelerometerPrecision)));
}

- (Region)regionForValue:(double)value withPrecision:(double)precision {
    if (value <= precision && value >= -precision) {
        return RegionZero;
    } else if (value < -precision) {
        return RegionNegative;
    } else {
        return RegionPositive;
    }
}

@end
