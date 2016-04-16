# WatchToDrone
Sample code for controlling a Parrot mini drone via Apple Watch gestures.

####Requirements
- iOS 9.0+ device (need a real device for Parrot's ARDiscovery to successfully scan for BTLE devices).
- Apple Watch running WatchOS 2.0+ (required for access to the accelerometer :))

Tested on Airborne Cargo running 2.1.55 firmware. Should run on any Parrot mini drone. 
Check Parrot's FreeFlight 3 App on iOS or Android to check your firmware version or upgrade to the latest firmware over Bluetooth. 
Also, you can go to http://www.parrot.com/usa/support/ and select your Parrot Mini-Drone to initiate a faster firmware update over USB.

####Instructions

- Download and open the project.
- Create an App ID for both the iOS and Apple WatchKit extension in the iOS Developer Center.
  - For the Apple WatchKit extension target, ensure Health Kit entitlements are enabled.
  - Create any necessary development provisioning profiles and set these under your Code Signing settings.
  - Change your parent app and WatchKit extension bundle identifiers to match your new app ids, if necessary.
  - Also check that the Watch App's Info.plist has the correct WKCompanionAppBundleIdentifier and that the WatchKit extension's Info.plist has the correct WKAppBundleIdentifier.
- Run the app on an iOS 9 device with an Apple Watch running WatchOS 2.0+.

Assuming you're now up and running, you should see a blank table with WatchToDrone at the top:

![alt tag](http://i.imgur.com/vlou2dlm.png)

Turn on your Parrot mini drone, and make sure the front lights have changed to green:

![alt tag](http://i.imgur.com/dQrUVcQm.jpg)

Going back to the app, you should see your drone populated in the list, assuming no other device or app is currently connected to it:

![alt tag](http://i.imgur.com/aQUB2MLm.png)

Before tapping on it to connect, let's make sure the watch app is working. Open the WatchToDrone app on your Apple Watch and tap "Start Session":

![alt tag](http://i.imgur.com/df9E0YZ.png)

You should start to see accelerometer readings on both the watch screen and in the Xcode debug console for your iOS device, assuming you're running in debug mode. If you see this, then watch communication with the iOS device is working correctly.

![alt tag](http://i.imgur.com/BGBtIdh.png)

Now, tap your drone's name the list. Upon successful connection to the device, you'll now have on-screen controls - and, assuming your watch app is running, you can start to issue movement commands to the drone via the watch gestures outlined here:

[![Gestures](https://img.youtube.com/vi/IDrZdVHqQt0/0.jpg)](https://www.youtube.com/watch?v=IDrZdVHqQt0E)

When done, press "Stop Session" to stop accelerometer capture on your Apple Watch and end the HKWorkoutSession (unless you want it to drain your battery all day :)).
