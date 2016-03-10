
#import <UIKit/UIKit.h>
#import <libARDiscovery/ARDISCOVERY_BonjourDiscovery.h>

@interface CellData : NSObject

@property (nonatomic, strong) ARService* service;
@property (nonatomic, strong) NSString* name;

@end

@interface DroneListViewController : UIViewController

@end