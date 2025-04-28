Pod::Spec.new do |s|
  s.name         = 'VBotPhoneSDKiOS-Public'
  s.version      = '1.0.3'
  s.authors      = "VBotDevTeam"
  s.summary      = "VBotPhoneSDKiOS-Public"
  s.description  = "VBot Phone SDK for iOS."
  s.homepage     = "https://vbot.vn"
  s.license      = 'LICENSE.txt'
  s.xcconfig = { "VBotPhoneSDK_SDK_VERSION" => s.version }
  s.source   = {
    :git => "https://github.com/VBotDevTeam/VBotPhoneSDKiOS-Public.git",
    :tag => s.version.to_s
  }
  s.documentation_url = "https://vbot.vn"
  s.platform = :ios, '12.0'
  s.static_framework = true
  s.swift_versions = [4.2, 5.0, 5.3, 5.4]
	
  s.frameworks = [
    "Foundation",
    "UIKit",
    "AVFoundation",
    "CallKit",
    "PushKit"
  ]

  s.vendored_frameworks = [
    "iOS/VBotPJSIP.xcframework",
    "iOS/VBotPhonePublic.xcframework",
  ]

  s.resources =   ['iOS/resources/*.png']

  s.dependency 'Starscream', '~> 4.0.8'
  s.dependency 'CocoaLumberjack', '~> 3.8.5'
  s.dependency 'Reachability', '~> 3.7.6'
   
end