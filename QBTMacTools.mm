#include "QBtMacTools.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/NSArray.h>
#include <Cocoa/Cocoa.h>


bool isMacSpecialURL(QUrl url)
{
   return url.url().startsWith("file:///.file/id=");
}


QUrl resolveMacSpecialURL(QUrl url)
{
    CFURLRef cfurl = url.toCFURL();
    CFErrorRef error = 0;
    CFURLRef absurl = CFURLCreateFilePathURL(kCFAllocatorDefault, cfurl, &error);
    url = QUrl::fromCFURL(absurl);
    CFRelease(cfurl);
    CFRelease(absurl);
    return url;
}

void activateAnotherInstance()
{
//    // check if abc.app is running
//    NSArray *apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"com.youApplication.abc"];
//    if ([apps count] == 0)
//    {
//        // not running, launch it
//        [[NSWorkspace sharedWorkspace] launchApplicationAtURL:[NSURL fileURLWithPath:@"abc.app" isDirectory:NO] options:NSWorkspaceLaunchDefault  configuration:nil error:NULL];
//    }



    NSArray *apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"com.yourcompany.bsc"];
    int count = [apps count];
    if ([apps count])
    {
        NSRunningApplication* app = [apps objectAtIndex:0];
        // abc.app is running, check if active
        if (![app isActive])
        {
            // not active, activate it
            [app activateWithOptions: NSApplicationActivateIgnoringOtherApps];
        }
    }
}

void moveToTrash(QString path)
{
    NSString *objcPath = [NSString stringWithCString:path.toUtf8() encoding:NSUTF8StringEncoding];
    NSError * error = nil;
    [[NSFileManager defaultManager] trashItemAtURL:[NSURL fileURLWithPath:objcPath] resultingItemURL:nil error:&error];
}
