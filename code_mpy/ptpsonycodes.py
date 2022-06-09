import ptpcodes

OPCODE_SDIOConnect            = 0x9201
OPCODE_SDIOGetExtDeviceInfo   = 0x9202
OPCODE_SonyGetDevicePropDesc  = 0x9203
OPCODE_SonyGetDevicePropValue = 0x9204
OPCODE_SetControlDeviceA      = 0x9205
OPCODE_GetControlDeviceDesc   = 0x9206
OPCODE_SetControlDeviceB      = 0x9207
OPCODE_GetAllDevicePropData   = 0x9209

EVENTCODE_ObjectAdded       = 0xC201
EVENTCODE_ObjectRemoved     = 0xC202
EVENTCODE_PropertyChanged   = 0xC203

PROPCODE_DPCCompensation    = 0xD200
PROPCODE_DRangeOptimize     = 0xD201
PROPCODE_SonyImageSize      = 0xD203
PROPCODE_ShutterSpeed       = 0xD20D
PROPCODE_Unknown_0E         = 0xD20E
PROPCODE_ColorTemp          = 0xD20F
PROPCODE_CCFilter           = 0xD210
PROPCODE_AspectRatio        = 0xD211
PROPCODE_FocusFound         = 0xD213
PROPCODE_ObjectInMemory     = 0xD215
PROPCODE_ExposeIndex        = 0xD216
PROPCODE_SonyBatteryLevel   = 0xD218
PROPCODE_PictureEffect      = 0xD21B
PROPCODE_ABFilter           = 0xD21C
PROPCODE_Recording          = 0xD21D
PROPCODE_ISO                = 0xD21E
PROPCODE_AutoFocus          = 0xD2C1
PROPCODE_FocusMode          = ptpcodes.PROPCODE_FocusMode
PROPCODE_Capture            = 0xD2C2
PROPCODE_Movie              = 0xD2C8
PROPCODE_StillImage         = 0xD2C7
PROPCODE_DriveMode          = ptpcodes.PROPCODE_StillCaptureMode
PROPCODE_Aperture           = ptpcodes.PROPCODE_Aperture
PROPCODE_ExpoComp           = ptpcodes.PROPCODE_ExposureBiasCompensation
PROPCODE_FocusArea          = 0xD22C
PROPCODE_FocusPointSet1     = 0xD232
PROPCODE_FocusPointSet2     = 0xD2DC
PROPCODE_ManualFocusMode    = 0xD2D2
PROPCODE_ManualFocusStep    = 0xD2D1
PROPCODE_FileFormat         = ptpcodes.PROPCODE_CompressionSetting
PROPCODE_MovieTime          = 0xD261
PROPCODE_ManualFocusDist    = 0xD24C
PROPCODE_FocusAssistWindow  = 0xD254
PROPCODE_MemoryRemaining_Card1 = 0xD249
PROPCODE_MemoryRemaining_Card2 = 0xD257

DRIVEMODE_Single                        = 0x0001
DRIVEMODE_ContHi                        = 0x0002
DRIVEMODE_ContHiPlus                    = 0x8010
DRIVEMODE_ContLo                        = 0x8012
DRIVEMODE_ContMid                       = 0x8015
DRIVEMODE_SelfTimer10s                  = 0x8004
DRIVEMODE_SelfTimer2s                   = 0x8005
DRIVEMODE_SelfTimer5s                   = 0x8003
DRIVEMODE_SelfTimer10sContinuous3Images = 0x8008
DRIVEMODE_SelfTimer10sContinuous5Images = 0x8009
DRIVEMODE_SelfTimer2sContinuous3Images  = 0x800E
DRIVEMODE_SelfTimer2sContinuous5Images  = 0x800F
DRIVEMODE_SelfTimer5sContinuous3Images  = 0x800C
DRIVEMODE_SelfTimer5sContinuous5Images  = 0x800D
DRIVEMODE_Continuous                    = 0x8013
DRIVEMODE_ContinuousSpeedPriority       = 0x8014
DRIVEMODE_WhiteBalanceBracketLow        = 0x8018
DRIVEMODE_WhiteBalanceBracketHigh       = 0x8028
DRIVEMODE_DRangeOptimizerBracketLow     = 0x8019
DRIVEMODE_DRangoOptimizerBracketHigh    = 0x8029
DRIVEMODE_ContinuousBracket10EV3Image   = 0x8311
DRIVEMODE_ContinuousBracket20EV3Image   = 0x8321
DRIVEMODE_ContinuousBracket30EV3Image   = 0x8331
DRIVEMODE_ContinuousBracket03EV3Image   = 0x8337
DRIVEMODE_ContinuousBracket05EV3Image   = 0x8357
DRIVEMODE_ContinuousBracket07EV3Image   = 0x8377
DRIVEMODE_ContinuousBracket10EV5Image   = 0x8511
DRIVEMODE_ContinuousBracket20EV5Image   = 0x8521
DRIVEMODE_ContinuousBracket30EV5Image   = 0x8531
DRIVEMODE_ContinuousBracket03EV5Image   = 0x8537
DRIVEMODE_ContinuousBracket05EV5Image   = 0x8557
DRIVEMODE_ContinuousBracket07EV5Image   = 0x8577
DRIVEMODE_ContinuousBracket10EV9Image   = 0x8911
DRIVEMODE_ContinuousBracket03EV9Image   = 0x8937
DRIVEMODE_ContinuousBracket05EV9Image   = 0x8957
DRIVEMODE_ContinuousBracket07EV9Image   = 0x8977
DRIVEMODE_SingleBracket10EV3Image       = 0x8310
DRIVEMODE_SingleBracket20EV3Image       = 0x8320
DRIVEMODE_SingleBracket30EV3Image       = 0x8330
DRIVEMODE_SingleBracket03EV3Image       = 0x8336
DRIVEMODE_SingleBracket05EV3Image       = 0x8356
DRIVEMODE_SingleBracket07EV3Image       = 0x8376
DRIVEMODE_SingleBracket10EV5Image       = 0x8510
DRIVEMODE_SingleBracket20EV5Image       = 0x8520
DRIVEMODE_SingleBracket30EV5Image       = 0x8530
DRIVEMODE_SingleBracket03EV5Image       = 0x8536
DRIVEMODE_SingleBracket05EV5Image       = 0x8556
DRIVEMODE_SingleBracket07EV5Image       = 0x8576
DRIVEMODE_SingleBracket10EV9Image       = 0x8910
DRIVEMODE_SingleBracket03EV9Image       = 0x8936
DRIVEMODE_SingleBracket05EV9Image       = 0x8956
DRIVEMODE_SingleBracket07EV9Image       = 0x8976

EXPOMODE_IntelligentAuto   = 0x8000
EXPOMODE_SuperiorAuto      = 0x8001
EXPOMODE_P                 = 0x0002
EXPOMODE_A                 = 0x0003
EXPOMODE_S                 = 0x0004
EXPOMODE_M                 = 0x0001
EXPOMODE_MovieP            = 0x8050
EXPOMODE_MovieA            = 0x8051
EXPOMODE_MovieS            = 0x8052
EXPOMODE_MovieM            = 0x8053
EXPOMODE_Panoramic         = 0x8041
EXPOMODE_Portrait          = 0x0007
EXPOMODE_SportsAction      = 0x8011
EXPOMODE_Macro             = 0x8015
EXPOMODE_Landscape         = 0x8014
EXPOMODE_Sunset            = 0x8012
EXPOMODE_NightScene        = 0x8013
EXPOMODE_HandheldTwilight  = 0x8016
EXPOMODE_NightPortrait     = 0x8017
EXPOMODE_AntiMotionBlur    = 0x8018

FOCUSAREA_WIDE                      = 0x0001
FOCUSAREA_ZONE                      = 0x0002
FOCUSAREA_CENTER                    = 0x0003
FOCUSAREA_MOVEABLE_SMALL            = 0x0101
FOCUSAREA_MOVEABLE_MEDIUM           = 0x0102
FOCUSAREA_MOVEABLE_LARGE            = 0x0103
FOCUSAREA_MOVEABLE_EXPAND           = 0x0104
FOCUSAREA_TRACKING_WIDE             = 0x0201
FOCUSAREA_TRACKING_ZONE             = 0x0202
FOCUSAREA_TRACKING_CENTER           = 0x0203
FOCUSAREA_TRACKING_MOVEABLE_SMALL   = 0x0204
FOCUSAREA_TRACKING_MOVEABLE_MEDIUM  = 0x0205
FOCUSAREA_TRACKING_MOVEABLE_LARGE   = 0x0206
FOCUSAREA_TRACKING_MOVEABLE_EXPAND  = 0x0207

FILEFORMAT_RAW              = 0x0010
FILEFORMAT_RAW_JPEG_STD     = 0x0012
FILEFORMAT_RAW_JPEG_FINE    = 0x0013
FILEFORMAT_RAW_JPEG_XFINE   = 0x0014
FILEFORMAT_JPEG_STD         = 0x0002
FILEFORMAT_JPEG_FINE        = 0x0003
FILEFORMAT_JPEG_XFINE       = 0x0004

FOCUSPOINT_X_MAX = 639
FOCUSPOINT_Y_MAX = 480
FOCUSPOINT_X_MID = ((FOCUSPOINT_X_MAX + 1) // 2)
FOCUSPOINT_Y_MID = ((FOCUSPOINT_Y_MAX + 0) // 2)

# TODO: unsure if this is specific to the a6600 that I'm testing on maybe a bigger screen means bigger numbers?

FOCUSSTEP_FARTHER_SMALL   =  1
FOCUSSTEP_FARTHER_MEDIUM  =  3
FOCUSSTEP_FARTHER_LARGE   =  7
FOCUSSTEP_CLOSER_SMALL    = -1
FOCUSSTEP_CLOSER_MEDIUM   = -3
FOCUSSTEP_CLOSER_LARGE    = -7
# TODO: experiment with other step sizes

AFMODE_AFC = 0x8004
AFMODE_AFS = 0x0002
AFMODE_AFA = 0x8005
AFMODE_DMF = 0x8006
AFMODE_MF  = 0x0001

FOCUSSTATUS_NONE     = 0x01 # idle
FOCUSSTATUS_FOCUSED  = 0x06 # focused and tracking
FOCUSSTATUS_HUNTING  = 0x05 # still searching
FOCUSSTATUS_FAILED   = 0x03 # AF failed unable to acquire
FOCUSSTATUS_LOST     = 0x02 # single AF on target but then shift out of focus (normal for AF-S)

EXPOMETERMODE_MULTI      = 0x8001
EXPOMETERMODE_CENTER     = 0x8002
EXPOMETERMODE_AVERAGE    = 0x8003
EXPOMETERMODE_SPOT_STD   = 0x8004
EXPOMETERMODE_SPOT_LARGE = 0x8005
EXPOMETERMODE_HIGHLIGHT  = 0x8006