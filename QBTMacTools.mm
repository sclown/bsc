#include "QBTMacTools.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/NSArray.h>
#include <Cocoa/Cocoa.h>

#ifndef BSC_BUNDLE_ID
#error "BSC_BUNDLE_ID must be defined by the build (see CMakeLists.txt)"
#endif

bool isMacSpecialURL( const QUrl& url )
{
   return url.url().startsWith( "file:///.file/id=" );
}

QUrl resolveMacSpecialURL( const QUrl& url )
{
    CFURLRef cfurl = url.toCFURL();
    CFErrorRef error = 0;
    CFURLRef absurl = CFURLCreateFilePathURL( kCFAllocatorDefault, cfurl, &error );
    QUrl result = absurl ? QUrl::fromCFURL( absurl ) : url;
    CFRelease( cfurl );
    if( absurl ) CFRelease( absurl );
    return result;
}

bool activateAnotherInstance()
{
    NSString* bundleId = [NSString stringWithUTF8String:BSC_BUNDLE_ID];
    NSArray* apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:bundleId];
    if( [apps count] == 0 ) {
        return false;
    }
    NSRunningApplication* app = [apps objectAtIndex:0];
    if( ![app isActive] ) {
        [app activateWithOptions:0];
    }
    return true;
}
