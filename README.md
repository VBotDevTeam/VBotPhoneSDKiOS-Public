# VBotPhoneSDKiOS-Public

VBot Phone SDK – Cho khách hàng khả năng tùy biến giao diện cuộc gọi.

## Code demo

[https://github.com/VBotDevTeam/VBot-iOS-SDK-Example](https://github.com/VBotDevTeam/VBot-iOS-SDK-Example)

## Yêu cầu

iOS 12.0 trở lên

## Cài đặt SDK

### Cocoapod

Thêm **VBotPhoneSDK** vào Podfile

```swift
platform :ios, '13.5'

target 'Runner' do
  use_frameworks! :linkage => :static

  pod 'VBotPhoneSDKiOS-Public', '1.1.6'

  target 'RunnerTests' do
    inherit! :search_paths
    # Pods for testing
  end

end

post_install do |installer|
  installer.pods_project.targets.each do |target|
   target.build_configurations.each do |config|
     # Bắt buộc
     config.build_settings['BUILD_LIBRARY_FOR_DISTRIBUTION'] = 'YES'
    end
  end
end
```

Trong đó phần **config.build_settings['BUILD_LIBRARY_FOR_DISTRIBUTION'] = 'YES'** là bắt buộc để VBotPhoneSDK hoạt động với các phiên bản Swift mới hơn

## Cấu hình dự án

### Bật Voip trong dự án Xcode

Chọn **Xcode Project → Capabilities**

Thêm **Background Modes** và **Push Notifications**

Ở **Background Modes,** Bật **Audio, AirPlay, and Picture in Picture |** **Voice over IP | Background Fetch | Remote Notifications**

Mở tệp **info.plist** và thêm key sau

```swift
<key>NSMicrophoneUsageDescription</key>
<string>Microphone access is necessary to be able to make calls.</string>
```

Lưu ý:

Khi khởi chạy dự án mà Xcode trả về lỗi

“Sandbox: rsync.samba (13105) deny(1) file-write-create”

Thực hiện chỉnh sửa sau:

Trong **Build Settings**, tìm **User Script Sandboxing**: Chọn **No**

## Sử dụng SDK

### Khởi tạo

File **AppDelegate.swift**

1. Trong hàm **application didFinishLaunchingWithOptions**, khởi tạo cấu hình `VBotConfig` và thiết lập `VBotPhone`:

**Khởi tạo cơ bản:**

```swift
let config = VBotConfig(
    iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData()
)

VBotPhone.sharedInstance.setup(with: config)
```

**Khởi tạo đầy đủ với các tùy chọn cấu hình:**

```swift
let config = VBotConfig(
    supportPopupCall: false,            // Mặc định: false. Cho phép hiển thị popup cuộc gọi.
    includesCallsInRecents: false,      // Mặc định: false. Cho phép lưu lịch sử cuộc gọi vào nhật ký cuộc gọi hệ thống qua CallKit.
    iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData(), // Ảnh icon hiển thị trên giao diện CallKit.
    environment: .production,           // Môi trường kết nối API. Mặc định là .production.
    customBaseUrl: nil                  // URL API tùy chỉnh nếu muốn cấu hình thủ công (ghi đè cấu hình môi trường).
)

VBotPhone.sharedInstance.setup(with: config)
```

---

### Gọi đi

Để thực hiện cuộc gọi đi, sử dụng hàm `startOutgoingCall`:

```swift
VBotPhone.sharedInstance.startOutgoingCall(
    displayName: "Nguyễn Văn A",
    number: "0901234567",
    hotline: "1900xxxx",
    externalCallId: "ext-call-123"  // Mã định danh cuộc gọi từ hệ thống ngoài (Tùy chọn)
) { success, error in
    if success {
        print("Bắt đầu cuộc gọi đi thành công")
    } else {
        print("Lỗi khởi tạo cuộc gọi đi: \(error?.localizedDescription ?? "")")
    }
}
```

---

### Gọi đến

Luồng cuộc gọi đến sẽ do SDK xử lý thông qua dịch vụ VoIP PushKit của Apple. Để nhận cuộc gọi, bạn cần thiết lập đăng ký PushKit trong `AppDelegate` và chuyển giao payload nhận được cho VBotPhone SDK.

Ví dụ:

```swift
import PushKit
import VBotPhoneSDK

@main
class AppDelegate: UIResponder, UIApplicationDelegate, PKPushRegistryDelegate {
    var window: UIWindow?
    var voipRegistry: PKPushRegistry!

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // ... Khởi tạo VBotConfig và setup VBotPhone ...

        // Khởi tạo và đăng ký PKPushRegistry nhận cuộc gọi VoIP
        voipRegistry = PKPushRegistry(queue: .main)
        voipRegistry!.desiredPushTypes = [.voIP]
        voipRegistry!.delegate = self

        return true
    }

    // Nhận VoIP Push Token và gửi lên hệ thống của bạn để cấu hình nhận cuộc gọi
    func pushRegistry(_ registry: PKPushRegistry, didUpdate pushCredentials: PKPushCredentials, for type: PKPushType) {
        guard let token = registry.pushToken(for: .voIP) else { return }
        let pushToken = token.map { String(format: "%.2hhx", $0) }.joined()
        print("VoIP Push Token: \(pushToken)")
    }

    // Nhận cuộc gọi VoIP đến và chuyển giao payload cho SDK
    func pushRegistry(_ registry: PKPushRegistry, didReceiveIncomingPushWith payload: PKPushPayload, for type: PKPushType, completion: @escaping () -> Void) {
        if type == .voIP {
            VBotPhone.sharedInstance.startIncomingCall(
                payload: payload,
                completion: completion
            )
        } else {
            completion()
        }
    }
}
```

---

### Lắng nghe các sự kiện (Delegate)

Đăng ký nhận các sự kiện cuộc gọi:

```swift
// Đăng ký nhận delegate
VBotPhone.sharedInstance.addDelegate(self)

// Hủy đăng ký nhận delegate
deinit {
    VBotPhone.sharedInstance.removeDelegate(self)
}
```

Các delegate method được cung cấp bởi `VBotPhoneDelegate`:

```swift
protocol VBotPhoneDelegate {
    // Trạng thái cuộc gọi thay đổi
    func callStateChanged(state: VBotCallState)

    // Cuộc gọi đi đã bắt đầu
    func callStarted()

    // Cuộc gọi đến được chấp nhận (Khi user chọn chấp nhận cuộc gọi)
    func callAccepted()

    // Cuộc gọi kết thúc, đi kèm nguyên nhân kết thúc cuộc gọi
    func callEnded(reason: VBotEndCallReason)

    // Trạng thái quyền truy cập microphone
    func microphonePermission(status: AVAudioSession.RecordPermission)

    // Trạng thái tắt/mở âm microphone thay đổi
    func callMuteStateDidChange(muted: Bool)

    // Nhận externalCallId (chỉ gọi 1 lần duy nhất khi bắt đầu cuộc gọi có chứa ID này, nếu không nil hoặc không rỗng)
    func didReceiveExternalCallId(_ externalCallId: String)

    // Yêu cầu hiển thị giao diện cuộc gọi
    func showCallVC()

    // Yêu cầu quay lại giao diện cuộc gọi
    func returnToCallVC()

    // Yêu cầu ẩn giao diện cuộc gọi
    func hideCallVC()

    // Mất kết nối mạng
    func networkIsUnreachable()

    // Kết nối mạng thay đổi
    func internetConnectionChanged()
}
```

---

### Sử dụng với Objective-C

SDK tương thích hoàn toàn để sử dụng từ dự án Objective-C.

#### 1. Import Module

Import module trong tệp `.m` hoặc `.mm` của bạn:

```objc
@import VBotPhoneSDK;
```

#### 2. Khởi tạo SDK trong AppDelegate

```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSData *iconData = UIImagePNGRepresentation([UIImage imageNamed:@"callkit-icon"]);

    VBotConfig *config = [[VBotConfig alloc] initWithSupportPopupCall:NO
                                               includesCallsInRecents:YES
                                                iconTemplateImageData:iconData
                                                          environment:VBotEnvironmentProduction
                                                        customBaseUrl:nil];

    [[VBotPhone sharedInstance] setupWith:config];
    return YES;
}
```

#### 3. Thực hiện cuộc gọi đi (Outgoing Call)

```objc
[[VBotPhone sharedInstance] startOutgoingCallWithDisplayName:@"Nguyễn Văn A"
                                                      number:@"0901234567"
                                                     hotline:@"1900xxxx"
                                              externalCallId:@"ext-call-123" // nil nếu không sử dụng
                                                  completion:^(BOOL success, NSError * _Nullable error) {
        if (success) {
            NSLog(@"Gọi đi thành công");
        } else {
            NSLog(@"Gọi đi thất bại với lỗi: %@", error.localizedDescription);
        }
    }];
```

#### 4. Nhận sự kiện cuộc gọi qua Delegate

```objc
@interface ViewController () <VBotPhoneDelegate>
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [[VBotPhone sharedInstance] addDelegate:self];
}

- (void)dealloc {
    [[VBotPhone sharedInstance] removeDelegate:self];
}

#pragma mark - VBotPhoneDelegate

- (void)callStateChangedWithState:(enum VBotCallState)state {
    NSLog(@"Trạng thái cuộc gọi thay đổi: %ld", (long)state);
}

- (void)callStarted {
    NSLog(@"Cuộc gọi đi đã bắt đầu");
}

- (void)callAccepted {
    NSLog(@"Cuộc gọi đã được chấp nhận");
}

- (void)callEndedWithReason:(enum VBotEndCallReason)reason {
    NSLog(@"Cuộc gọi kết thúc với nguyên nhân: %ld", (long)reason);
}

- (void)didReceiveExternalCallId:(NSString *)externalCallId {
    NSLog(@"Nhận được External Call ID: %@", externalCallId);
}

@end
```

---

### Xem thêm

#### VBotEndCallReason và VBotError

```
    // Timeout
    case timeOut = -1001

    // Khởi tạo không thành công
    case initiationFailed = 1001

    case initiationFailed_1 = 1002

    // Chưa cấp truyền mic
    case microphonePermissionDenied = 1003

    case invalidPhoneNumber = 1004

    // Không có dữ liệu từ máy chủ
    case noDataFromServer = 1005

    case initiationFailed_2 = 1006

    case initiationFailed_3 = 1007

    // Dữ liệu không hợp lệ
    case dataInvalid = 1008

    case initiationFailed_4 = 1009

    // Xác thực thất bại
    case authenticatedFailed = 1010

    // Đang có cuộc gọi khác
    case anotherCallInProgress = 1011

    // Cuộc gọi kết thúc
    case normal = 1012

    // Từ chối cuộc gọi
    case decline = 1013

    // Không liên lạc được
    case temporarilyUnavailable = 1014

    // Máy bận
    case busy = 1015

    // reportNewIncomingCall lỗi
    case reportNewIncomingCallFailed = 1016

    // Lỗi chưa xác định
    case unknownError = 1999
```
