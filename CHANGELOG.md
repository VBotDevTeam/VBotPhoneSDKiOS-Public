# Changelog

Các thay đổi của VBot Phone SDK iOS.

## 1.1.9

_Ngày phát hành: 21/08/2026_

### Tính năng mới & Cải tiến

- Sửa lỗi nhận cuộc gọi Auto Call

### Cài đặt

```ruby
pod 'VBotPhoneSDKiOS-Public', :git => 'https://github.com/VBotDevTeam/VBotPhoneSDKiOS-Public.git', :tag => '1.1.9'
```

## 1.1.8

_Ngày phát hành: 03/08/2026_

### Tính năng mới

- Callback `callEnded(reason: VBotEndCallReason, endedBy: VBotCallEndParty)` cho biết nguyên nhân và bên kết thúc cuộc gọi. Callback `callEnded(reason:)` cũ vẫn hoạt động tương thích ngược.
- SDK đọc SIP final response/BYE để map nguyên nhân kết thúc cuộc gọi: ví dụ `486` → `busy` / `callee`, `487` → `callerCanceled` / `caller`, `500` → `connectionError` / `server`.
- `VBotEndCallReason` và `VBotCallEndParty` có `key` và `description` public, phục vụ hiển thị và analytics.

### VBotEndCallReason mới

| reason                     | SIP |
| -------------------------- | --- |
| `incorrectInformation`     | 400 |
| `unauthenticated`          | 401 |
| `insufficientBalance`      | 402 |
| `recipientBlocksCalls`     | 403 |
| `destinationNotFound`      | 404 |
| `callIntervalNotAllowed`   | 405 |
| `memberNotActivated`       | 406 |
| `memberNotInProject`       | 407 |
| `doNotDisturb`             | 409 |
| `destinationGone`          | 410 |
| `recipientAbsent`          | 411 |
| `packageExpired`           | 412 |
| `hotlineTelcoNotSupported` | 413 |
| `telcoNotFound`            | 414 |
| `invalidParameter`         | 415 |
| `projectExpired`           | 416 |
| `callerCanceled`           | 487 |
| `connectionError`          | 500 |
| `transmissionError`        | 502 |

### Cài đặt

CocoaPods clone trực tiếp repository theo Git tag, không cần package trên CocoaPods Specs/CDN:

```ruby
pod 'VBotPhoneSDKiOS-Public', :git => 'https://github.com/VBotDevTeam/VBotPhoneSDKiOS-Public.git', :tag => '1.1.8'
```
