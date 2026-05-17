#!/bin/zsh

rm -r Mx.xcframework
rm -r build
#xcodebuild clean 

echo "Building for macOS"
xcodebuild archive \
-scheme MxmacOS \
-destination "platform=macOS" \
-archivePath "build/Mx.macOS.xcarchive" \
SKIP_INSTALL=NO

echo "Building for iOS devices (arm64)"
xcodebuild archive \
-scheme MxiOS \
-sdk iphoneos \
-destination "generic/platform=iOS" \
-archivePath "build/Mx.iOS.xcarchive" \
SKIP_INSTALL=NO

echo "Building for iOS simulator"
xcodebuild archive \
-scheme MxiOS \
-sdk iphonesimulator \
-archivePath "build/Mx.iOS-simulator.xcarchive" \
SKIP_INSTALL=NO

echo "Building for Mac Catalyst"
xcodebuild archive \
-scheme MxiOS \
-destination "platform=macOS,variant=Mac Catalyst" \
-archivePath "build/Mx.catalyst.xcarchive" \
SKIP_INSTALL=NO

xcodebuild -create-xcframework \
-framework "build/Mx.macOS.xcarchive/Products/Library/Frameworks/MxmacOS.framework" \
-framework "build/Mx.iOS.xcarchive/Products/Library/Frameworks/MxiOS.framework" \
-framework "build/Mx.iOS-simulator.xcarchive/Products/Library/Frameworks/MxiOS.framework" \
-framework "build/Mx.catalyst.xcarchive/Products/Library/Frameworks/MxiOS.framework" \
-output "Mx.xcframework"

#rm -r build

open .
