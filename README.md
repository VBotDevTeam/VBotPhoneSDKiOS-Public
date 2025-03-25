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

  pod 'VBotPhoneSDKiOS-Public', '1.0.9'

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

1. Trong hàm **application didFinishLaunchingWithOptions**, gọi hàm khởi tạo VBotPhone

```swift
let config = VBotConfig(
            iconTemplateImageData: UIImage(named: "callkit-icon")?.pngData())
            
VBotPhone.sharedInstance.setup(with: config)
```

Trong đó:
- **config** là cấu hình tùy chọn cho SDK

### Gọi đi

Để thực hiện cuộc gọi đi, hãy sử dụng hàm **startOutgoingCall**

```swift
VBotPhone.sharedInstance.startOutgoingCall()
```

### Gọi đến
Luồng cuộc gọi đến sẽ do SDK xử lý


---


### Lắng nghe các sự kiện

Có 2 cách để lắng nghe các sự kiện trong VBot SDK

1. **Sử dụng Protocol delegate**

Đăng ký nhận sự kiện VBot:

```swift
  // Đăng ký nhận sự kiện cuộc gọi
  VBotPhone.sharedInstance.addDelegate(self)

  // Hủy đăng ký
   deinit {
       VBotPhone.sharedInstance.removeDelegate(self)
   }
```

Các delegate method bao gồm

```swift
protocol VBotPhoneDelegate {

    // Trạng thái cuộc gọi thay đổi
    func callStateChanged(state: VBotCallState)

    // Cuộc gọi đi đã bắt đầu
    func callStarted()

    // Cuộc gọi đến được chấp nhận (Khi user chọn chấp nhận cuộc gọi)
    func callAccepted()

    // Cuộc gọi kết thúc, cùng nguyên nhân
    func callEnded(reason: VBotEndCallReason)
    
    // Lấy quyền microphone
    func microphonePermission(status: AVAudioSession.RecordPermission)

    // Trạng thái Microphone thay đổi
    func callMuteStateDidChange(muted: Bool)

    // Nhấn vào nút nhắn tin
    func messageButtonTapped()

}
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




