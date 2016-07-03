////////////////////////////////////////////////////////////////////////////////
// UltimateFishbot - A simple World of Warcraft fishbot.
//
// Original Work (VB port)
//   Copyright (c) 2016, Gordon (LaBlazer).  All rights reserved.
// Modified Work (C# port)
//   Copyright (c) 2016, Pierre Ghiot (Mograine). All rights reserved.
// Modified Work (C++ port)
//   Copyright (c) 2016, Dustin Hopper (trotlinebeercan). All rights reserved.
//
// Copyrights licensed under an unknown license.
// https://github.com/UltimateFishbot/UltimateFishbot
////////////////////////////////////////////////////////////////////////////////

#include "Helpers/OS.h"

#ifndef __APPLE__
#  ifdef UNIX
#    error Although this is an xplat port, it is only for OSX
#  else
#    error Don't compile an obj-c file on anything but OSX
#  endif // UNIX
#else

#  import <ApplicationServices/ApplicationServices.h>
#  import <Carbon/Carbon.h>
#  import <Cocoa/Cocoa.h>
#  import <Foundation/Foundation.h>

///////////////////////////////////////////////////////////////////////////////
// OSX-only objective-c++ code declarations
///////////////////////////////////////////////////////////////////////////////
namespace osx
{
    void sendMouseMoveEventOSX(int32_t x, int32_t y);
    void sendMouseClickEventOSX();
    void sendKeyPressEventOSX(uint8_t key);
    void activateWarcraftOSX();

    UF::Image8Bit getCursorOSX();

    UF::Image8Bit convertCGImageRefToImage8Bit(CGImageRef image_ref);
    CGKeyCode keyCodeFromKeyString(NSString *key_string);
    CGImageRef convertNSImageToCGImageRef(NSImage *image);
    NSImage *resizeNSImage(NSImage *image, int32_t new_width, int32_t new_height);
};

///////////////////////////////////////////////////////////////////////////////
// inline bridge functions to prevent obj-c++ code being linked
///////////////////////////////////////////////////////////////////////////////
void
UF::OS::moveMouse(int32_t x, int32_t y)
{
    osx::sendMouseMoveEventOSX(x, y);
}

void
UF::OS::sendKeyPress(uint8_t key)
{
    osx::sendKeyPressEventOSX(key);
}

void
UF::OS::sendClick()
{
    osx::sendMouseClickEventOSX();
}

void
UF::OS::activateWarcraft()
{
    osx::activateWarcraftOSX();
}

UF::Image8Bit
UF::OS::getCursor()
{
    return osx::getCursorOSX();
}

///////////////////////////////////////////////////////////////////////////////
// OSX-only objective-c++ code implementation
///////////////////////////////////////////////////////////////////////////////
void
osx::sendMouseMoveEventOSX(int32_t x, int32_t y)
{
    CGEventRef move = CGEventCreateMouseEvent(
        NULL, kCGEventMouseMoved,
        CGPointMake(x, y),
        kCGMouseButtonLeft
    );
    CGEventPost(kCGHIDEventTap, move);
    usleep(250);
    CFRelease(move);
}

void
osx::sendMouseClickEventOSX()
{
    CGEventRef mouseDown = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseDown,
        CGPointMake(0, 0),
        kCGMouseButtonLeft
    );
    CGEventRef mouseUp   = CGEventCreateMouseEvent(
        NULL, kCGEventLeftMouseUp,
        CGPointMake(0, 0),
        kCGMouseButtonLeft
    );

    CGEventPost(kCGHIDEventTap, mouseDown);
    usleep(250);
    CGEventPost(kCGHIDEventTap, mouseUp);
    usleep(250);
    CFRelease(mouseDown);
    CFRelease(mouseUp);
}

void
osx::sendKeyPressEventOSX(uint8_t key)
{
    CGKeyCode code = keyCodeFromKeyString([NSString stringWithFormat:@"%c", key]);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(NULL, code, true);
    CGEventRef keyDn = CGEventCreateKeyboardEvent(NULL, code, false);

    CGEventPost(kCGHIDEventTap, keyUp);
    usleep(250);
    CGEventPost(kCGHIDEventTap, keyDn);
    usleep(250);

    CFRelease(keyDn);
    CFRelease(keyUp);
}

void
osx::activateWarcraftOSX()
{
    NSArray *apps = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"com.blizzard.worldofwarcraft"];
    [(NSRunningApplication *)[apps objectAtIndex:0] activateWithOptions:NSApplicationActivateIgnoringOtherApps];
}

UF::Image8Bit
osx::getCursorOSX()
{
    @autoreleasepool
    {
        NSCursor *cursor = [NSCursor currentSystemCursor];
        NSImage *cursorImage = [cursor image];
        CGImageRef imageRef = osx::convertNSImageToCGImageRef(osx::resizeNSImage(cursorImage, 35, 35));
        UF::Image8Bit image = osx::convertCGImageRefToImage8Bit(imageRef);
        return image;
    }
}

UF::Image8Bit
osx::convertCGImageRefToImage8Bit(CGImageRef image_ref)
{
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image_ref);
    const int32_t imageWidth  = CGImageGetWidth(image_ref);
    const int32_t imageHeight = CGImageGetHeight(image_ref);
    const int32_t imageBPP    = CGImageGetBitsPerPixel(image_ref) / 8;

    UF::Image8Bit image(imageWidth, imageHeight, 4);

    CGContextRef contextRef = CGBitmapContextCreate(
        *(image.m_data),
        imageWidth,
        imageHeight,
        8,
        0,
        colorSpace,
        kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault
    );

    CGContextDrawImage(contextRef, CGRectMake(0, 0, imageWidth, imageHeight), image_ref);
    CGContextRelease(contextRef);

    return image;
}

CGImageRef
osx::convertNSImageToCGImageRef(NSImage *image)
{
    NSGraphicsContext *context = [NSGraphicsContext currentContext];
    CGRect imageCGRect = CGRectMake(0, 0, image.size.width, image.size.height);
    NSRect imageRect = NSRectFromCGRect(imageCGRect);
    CGImageRef imageRef = [image CGImageForProposedRect:&imageRect context:context hints:nil];
    return imageRef;
}

NSImage *
osx::resizeNSImage(NSImage *image, int32_t new_width, int32_t new_height)
{
    const CGSize newSize = CGSizeMake(new_width, new_height);
    const CGRect newRect = CGRectMake(0, 0, newSize.width, newSize.height);

    NSImage *newImage = [[NSImage alloc] initWithSize: newSize];
    [newImage lockFocus];
    [image setSize:newSize];
    [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];
    [image drawAtPoint:NSZeroPoint fromRect:newRect operation:NSCompositeCopy fraction:1.0];
    [newImage unlockFocus];
    return newImage;
}

CGKeyCode
osx::keyCodeFromKeyString(NSString *key_string)
{
    if ([key_string isEqualToString:@"a"]) return 0;
    if ([key_string isEqualToString:@"s"]) return 1;
    if ([key_string isEqualToString:@"d"]) return 2;
    if ([key_string isEqualToString:@"f"]) return 3;
    if ([key_string isEqualToString:@"h"]) return 4;
    if ([key_string isEqualToString:@"g"]) return 5;
    if ([key_string isEqualToString:@"z"]) return 6;
    if ([key_string isEqualToString:@"x"]) return 7;
    if ([key_string isEqualToString:@"c"]) return 8;
    if ([key_string isEqualToString:@"v"]) return 9;
    // WHAT IS 10 ???
    if ([key_string isEqualToString:@"b"]) return 11;
    if ([key_string isEqualToString:@"q"]) return 12;
    if ([key_string isEqualToString:@"w"]) return 13;
    if ([key_string isEqualToString:@"e"]) return 14;
    if ([key_string isEqualToString:@"r"]) return 15;
    if ([key_string isEqualToString:@"y"]) return 16;
    if ([key_string isEqualToString:@"t"]) return 17;
    if ([key_string isEqualToString:@"1"]) return 18;
    if ([key_string isEqualToString:@"2"]) return 19;
    if ([key_string isEqualToString:@"3"]) return 20;
    if ([key_string isEqualToString:@"4"]) return 21;
    if ([key_string isEqualToString:@"6"]) return 22;
    if ([key_string isEqualToString:@"5"]) return 23;
    if ([key_string isEqualToString:@"="]) return 24;
    if ([key_string isEqualToString:@"9"]) return 25;
    if ([key_string isEqualToString:@"7"]) return 26;
    if ([key_string isEqualToString:@"-"]) return 27;
    if ([key_string isEqualToString:@"8"]) return 28;
    if ([key_string isEqualToString:@"0"]) return 29;
    if ([key_string isEqualToString:@"]"]) return 30;
    if ([key_string isEqualToString:@"o"]) return 31;
    if ([key_string isEqualToString:@"u"]) return 32;
    if ([key_string isEqualToString:@"["]) return 33;
    if ([key_string isEqualToString:@"i"]) return 34;
    if ([key_string isEqualToString:@"p"]) return 35;
    if ([key_string isEqualToString:@"RETURN"]) return 36;
    if ([key_string isEqualToString:@"l"]) return 37;
    if ([key_string isEqualToString:@"j"]) return 38;
    if ([key_string isEqualToString:@"'"]) return 39;
    if ([key_string isEqualToString:@"k"]) return 40;
    if ([key_string isEqualToString:@";"]) return 41;
    if ([key_string isEqualToString:@"\\"]) return 42;
    if ([key_string isEqualToString:@","]) return 43;
    if ([key_string isEqualToString:@"/"]) return 44;
    if ([key_string isEqualToString:@"n"]) return 45;
    if ([key_string isEqualToString:@"m"]) return 46;
    if ([key_string isEqualToString:@"."]) return 47;
    if ([key_string isEqualToString:@"TAB"]) return 48;
    if ([key_string isEqualToString:@"SPACE"]) return 49;
    if ([key_string isEqualToString:@"`"]) return 50;
    if ([key_string isEqualToString:@"DELETE"]) return 51;
    if ([key_string isEqualToString:@"ENTER"]) return 52;
    if ([key_string isEqualToString:@"ESCAPE"]) return 53;
    if ([key_string isEqualToString:@"."]) return 65;
    if ([key_string isEqualToString:@"*"]) return 67;
    if ([key_string isEqualToString:@"+"]) return 69;
    if ([key_string isEqualToString:@"CLEAR"]) return 71;
    if ([key_string isEqualToString:@"/"]) return 75;
    if ([key_string isEqualToString:@"ENTER"]) return 76;
    if ([key_string isEqualToString:@"="]) return 78;
    if ([key_string isEqualToString:@"="]) return 81;
    if ([key_string isEqualToString:@"0"]) return 82;
    if ([key_string isEqualToString:@"1"]) return 83;
    if ([key_string isEqualToString:@"2"]) return 84;
    if ([key_string isEqualToString:@"3"]) return 85;
    if ([key_string isEqualToString:@"4"]) return 86;
    if ([key_string isEqualToString:@"5"]) return 87;
    if ([key_string isEqualToString:@"6"]) return 88;
    if ([key_string isEqualToString:@"7"]) return 89;
    if ([key_string isEqualToString:@"8"]) return 91;
    if ([key_string isEqualToString:@"9"]) return 92;
    if ([key_string isEqualToString:@"F5"]) return 96;
    if ([key_string isEqualToString:@"F6"]) return 97;
    if ([key_string isEqualToString:@"F7"]) return 98;
    if ([key_string isEqualToString:@"F3"]) return 99;
    if ([key_string isEqualToString:@"F8"]) return 100;
    if ([key_string isEqualToString:@"F9"]) return 101;
    if ([key_string isEqualToString:@"F11"]) return 103;
    if ([key_string isEqualToString:@"F13"]) return 105;
    if ([key_string isEqualToString:@"F14"]) return 107;
    if ([key_string isEqualToString:@"F10"]) return 109;
    if ([key_string isEqualToString:@"F12"]) return 111;
    if ([key_string isEqualToString:@"F15"]) return 113;
    if ([key_string isEqualToString:@"HELP"]) return 114;
    if ([key_string isEqualToString:@"HOME"]) return 115;
    if ([key_string isEqualToString:@"PGUP"]) return 116;
    if ([key_string isEqualToString:@"DELETE"]) return 117;
    if ([key_string isEqualToString:@"F4"]) return 118;
    if ([key_string isEqualToString:@"END"]) return 119;
    if ([key_string isEqualToString:@"F2"]) return 120;
    if ([key_string isEqualToString:@"PGDN"]) return 121;
    if ([key_string isEqualToString:@"F1"]) return 122;
    if ([key_string isEqualToString:@"LEFT"]) return 123;
    if ([key_string isEqualToString:@"RIGHT"]) return 124;
    if ([key_string isEqualToString:@"DOWN"]) return 125;
    if ([key_string isEqualToString:@"UP"]) return 126;

    return 0;
}

#endif // !defined(__APPLE__)