
#import "DroneListViewController.h"
#import <WatchConnectivity/WatchConnectivity.h>
#import "PilotingViewController.h"
#import <libARDiscovery/ARDISCOVERY_BonjourDiscovery.h>
#import "JRMWatchToDroneGestureRecognizer.h"

@interface CellData ()
@end

@implementation CellData
@end

@interface DroneListViewController () <UITableViewDelegate, UITableViewDataSource,WCSessionDelegate>

@property (nonatomic, strong) IBOutlet UITableView *tableView;
@property (nonatomic, strong) ARService *serviceSelected;
@property (nonatomic, strong) NSArray *tableData;
@property (nonatomic, strong) JRMWatchToDroneGestureRecognizer *gestureRecognizer;

@end

@implementation DroneListViewController


#pragma mark - View lifecycle

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableData = [NSArray array];
    self.gestureRecognizer = [[JRMWatchToDroneGestureRecognizer alloc] init];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    [self registerApplicationNotifications];
    
    // Start Parrot discovery
    [[ARDiscovery sharedInstance] start];
    
    // Start listening for Watch messages
    if ([WCSession isSupported]) {
        WCSession *session = [WCSession defaultSession];
        session.delegate = self;
        [session activateSession];
    }
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
    
    [self unregisterApplicationNotifications];
    [[ARDiscovery sharedInstance] stop];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if(([segue.identifier isEqualToString:@"pilotingSegue"]) && (self.serviceSelected != nil)) {
        PilotingViewController *pilotingViewController = (PilotingViewController *)[segue destinationViewController];
        [pilotingViewController setService:self.serviceSelected];
    }
}

#pragma mark - WCSessionDelegate methods

- (void)session:(WCSession *)session didReceiveMessage:(NSDictionary<NSString *,id> *)message {
    // This WCSession implementation for debugging purposes only.
    // Must actually be connected to a drone in PilotingViewController to
    // send commands to the Parrot drone.
    
    
    if ((message[@"watchAccData"]) != nil) {
        // Incoming accelerometer data
        NSString *dataString = message[@"watchAccData"];
        NSArray *components = [dataString componentsSeparatedByString:@","];
        double x = [components[0] doubleValue];
        double y = [components[1] doubleValue];
        double z = [components[2] doubleValue];
        NSLog(@"X:%f Y:%f Z:%f",x,y,z);
        
        DroneState newState = [self.gestureRecognizer detectStateWithX:x y:y z:z];
        NSLog(@"New State: %@", droneValueString(newState));
        
    } else if (message[@"workoutState"]) {
        NSLog(@"New HKWorkoutSession state: %@",message[@"workoutState"]);
    } else if (message[@"error"]) {
        NSLog(@"Error: %@",message[@"error"]);
    }
}


#pragma mark - Notification methods

- (void)registerApplicationNotifications {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(enteredBackground:) name: UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(enterForeground:) name: UIApplicationWillEnterForegroundNotification object:nil];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(discoveryDidUpdateServices:) name:kARDiscoveryNotificationServicesDevicesListUpdated object:nil];
}

- (void)unregisterApplicationNotifications {
    [[NSNotificationCenter defaultCenter] removeObserver:self name: UIApplicationDidEnterBackgroundNotification object: nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name: UIApplicationWillEnterForegroundNotification object: nil];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kARDiscoveryNotificationServicesDevicesListUpdated object:nil];
}

- (void)enteredBackground:(NSNotification*)notification {
    [[ARDiscovery sharedInstance] stop];
}

- (void)enterForeground:(NSNotification*)notification {
    [[ARDiscovery sharedInstance] start];
}

- (void)discoveryDidUpdateServices:(NSNotification *)notification {
    // Called when the list of discovered services has changed
    dispatch_async(dispatch_get_main_queue(), ^{
        [self updateServicesList:[[notification userInfo] objectForKey:kARDiscoveryServicesList]];
    });
}

- (void)updateServicesList:(NSArray *)services {
    NSMutableArray *serviceArray = [NSMutableArray array];
    
    for (ARService *service in services) {
        // only display the ble services
        if ([service.service isKindOfClass:[ARBLEService class]]) {
            CellData *cellData = [[CellData alloc]init];
            
            [cellData setService:service];
            [cellData setName:service.name];
            [serviceArray addObject:cellData];
        }
    }
    
    self.tableData = serviceArray;
    [self.tableView reloadData];
}


#pragma mark - UITableViewDataSource methods

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self.tableData count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"Cell"];
    }
    cell.textLabel.text = [(CellData *)[self.tableData objectAtIndex:indexPath.row] name];
    return cell;
}

#pragma mark - UITableViewDelegate methods

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    self.serviceSelected = [(CellData *)[_tableData objectAtIndex:indexPath.row] service];
    [self performSegueWithIdentifier:@"pilotingSegue" sender:self];
}



@end
