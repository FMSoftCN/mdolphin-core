//
//  BrowserStatisticsWindowController.m
//  MiniBrowser
//
//  Created by Sam Weinig on 4/21/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import "BrowserStatisticsWindowController.h"

#import <WebKit2/WKContextPrivate.h>

@implementation BrowserStatisticsWindowController

- (id)initWithThreadedWKContextRef:(WKContextRef)threadContext processWKContextRef:(WKContextRef)processContext
{
    if (self = [super initWithWindowNibName:@"BrowserStatisticsWindow"]) {
        _threadContext = WKContextRetain(threadContext);
        _processContext = WKContextRetain(processContext);
    }

    return self;
}

- (void)dealloc
{
    WKContextRelease(_threadContext);
    _threadContext = 0;

    WKContextRelease(_processContext);
    _processContext = 0;
    
    [super dealloc];
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    [self refreshStatistics:nil];
}

- (IBAction)refreshStatistics:(id)sender
{
    WKContextStatistics threadStats;
    WKContextGetStatistics(_threadContext, &threadStats);

    WKContextStatistics processStats;
    WKContextGetStatistics(_processContext, &processStats);

    [[_basicStatsMatrix cellWithTag:11] setIntValue:processStats.numberOfWKPages];
    [[_basicStatsMatrix cellWithTag:12] setIntValue:processStats.numberOfWKFrames];

    [[_basicStatsMatrix cellWithTag:21] setIntValue:threadStats.numberOfWKPages];
    [[_basicStatsMatrix cellWithTag:22] setIntValue:threadStats.numberOfWKFrames];

}

@end
