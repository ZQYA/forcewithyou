//
//  main.m
//  force
//
//  Created by 权周 on 2017/6/29.
//  Copyright © 2017年 权周. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "broadcast.hpp"
#import <string>
#import <Foundation/Foundation.h>
NSString *server_ip;
int main(int argc, char * argv[]) {
    @autoreleasepool {
        bd_so::BroadcastCenter center(true);
        std::string serverip;
        center.startSend("hello",serverip);
        server_ip = [NSString stringWithUTF8String:serverip.c_str()];
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
