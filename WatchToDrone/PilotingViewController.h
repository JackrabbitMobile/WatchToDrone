
#import "ViewController.h"
#import <libARDiscovery/ARDISCOVERY_BonjourDiscovery.h>

@interface PilotingViewController : ViewController

/**
    The service we want to connect with (in this sample, the service is an ARBLEService).
 */
@property (nonatomic, strong) ARService* service;
@property (nonatomic, strong) IBOutlet UILabel *batteryLabel;

@end
