// VBotLogger.h

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface VBotLoggerObjC : NSObject

@property (class, nonatomic, readonly, strong) VBotLoggerObjC *shared;

- (void)configureWithDebugMode:(BOOL)debugMode;
- (void)logVerbose:(nullable NSString *)filter message:(NSString *)message file:(NSString *)file function:(NSString *)function line:(NSUInteger)line;
- (void)logDebug:(nullable NSString *)filter message:(NSString *)message file:(NSString *)file function:(NSString *)function line:(NSUInteger)line;
- (void)logInfo:(nullable NSString *)filter message:(NSString *)message file:(NSString *)file function:(NSString *)function line:(NSUInteger)line;
- (void)logWarning:(nullable NSString *)filter message:(NSString *)message file:(NSString *)file function:(NSString *)function line:(NSUInteger)line;
- (void)logError:(nullable NSString *)filter message:(NSString *)message file:(NSString *)file function:(NSString *)function line:(NSUInteger)line;

@end

// Simple macro definitions
#define VBotLogInfo2(filter, message) [[VBotLoggerObjC shared] logInfo:filter message:message file:@(__FILE__) function:@(__PRETTY_FUNCTION__) line:__LINE__]
#define VBotLogDebug2(filter, message) [[VBotLoggerObjC shared] logDebug:filter message:message file:@(__FILE__) function:@(__PRETTY_FUNCTION__) line:__LINE__]
#define VBotLogError2(filter, message) [[VBotLoggerObjC shared] logError:filter message:message file:@(__FILE__) function:@(__PRETTY_FUNCTION__) line:__LINE__]
#define VBotLogWarning2(filter, message) [[VBotLoggerObjC shared] logWarning:filter message:message file:@(__FILE__) function:@(__PRETTY_FUNCTION__) line:__LINE__]
#define VBotLogVerbose2(filter, message) [[VBotLoggerObjC shared] logVerbose:filter message:message file:@(__FILE__) function:@(__PRETTY_FUNCTION__) line:__LINE__]

NS_ASSUME_NONNULL_END
