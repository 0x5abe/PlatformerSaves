#pragma pattern_limit 16777216
#include <std/sys.pat>
#include <std/mem.pat>

enum GJGameEvent : u32 {
    None = 0,
    TinyLanding = 1,
    FeatherLanding = 2,
    SoftLanding = 3,
    NormalLanding = 4,
    HardLanding = 5,
    HitHead = 6,
    OrbTouched = 7,
    OrbActivated = 8,
    PadActivated = 9,
    GravityInverted = 10,
    GravityRestored = 11,
    NormalJump = 12,
    RobotBoostStart = 13,
    RobotBoostStop = 14,
    UFOJump = 15,
    ShipBoostStart = 16,
    ShipBoostEnd = 17,
    SpiderTeleport = 18,
    BallSwitch = 19,
    SwingSwitch = 20,
    WavePush = 21,
    WaveRelease = 22,
    DashStart = 23,
    DashStop = 24,
    Teleported = 25,
    PortalNormal = 26,
    PortalShip = 27,
    PortalBall = 28,
    PortalUFO = 29,
    PortalWave = 30,
    PortalRobot = 31,
    PortalSpider = 32,
    PortalSwing = 33,
    YellowOrb = 34,
    PinkOrb = 35,
    RedOrb = 36,
    GravityOrb = 37,
    GreenOrb = 38,
    DropOrb = 39,
    CustomOrb = 40,
    DashOrb = 41,
    GravityDashOrb = 42,
    SpiderOrb = 43,
    TeleportOrb = 44,
    YellowPad = 45,
    PinkPad = 46,
    RedPad = 47,
    GravityPad = 48,
    SpiderPad = 49,
    PortalGravityFlip = 50,
    PortalGravityNormal = 51,
    PortalGravityInvert = 52,
    PortalFlip = 53,
    PortalUnFlip = 54,
    PortalNormalScale = 55,
    PortalMiniScale = 56,
    PortalDualOn = 57,
    PortalDualOff = 58,
    PortalTeleport = 59,
    Checkpoint = 60,
    DestroyBlock = 61,
    UserCoin = 62,
    PickupItem = 63,
    CheckpointRespawn = 64,
    FallLow = 65,
    FallMed = 66,
    FallHigh = 67,
    FallVHigh = 68,
    JumpPush = 69,
    JumpRelease = 70,
    LeftPush = 71,
    LeftRelease = 72,
    RightPush = 73,
    RightRelease = 74,
    PlayerReversed = 75,
    FallSpeedLow = 76,
    FallSpeedMed = 77,
    FallSpeedHigh = 78,
};

enum EasingType : u32 {
    None = 0,
    EaseInOut = 1,
    EaseIn = 2,
    EaseOut = 3,
    ElasticInOut = 4,
    ElasticIn = 5,
    ElasticOut = 6,
    BounceInOut = 7,
    BounceIn = 8,
    BounceOut = 9,
    ExponentialInOut = 10,
    ExponentialIn = 11,
    ExponentialOut = 12,
    SineInOut = 13,
    SineIn = 14,
    SineOut = 15,
    BackInOut = 16,
    BackIn = 17,
    BackOut = 18,
};

enum GhostType : u32 {
    Disabled = 0,
    Enabled = 1,
};

enum TouchTriggerType : u32 {
    Unk = 0,
};

enum TouchTriggerControl : u32 {
    Unk = 0,
};

enum PulseEffectType : u32 {
    Default = 0,
    Hsv = 1,
    Color = 2,
};

struct pair<K,V> {
    K first;
    V second;
};

struct unordered_map<K,V> {
    u32 size;
    pair<K,V> elements[size];
};

struct map<K,V> {
    u32 size;
    pair<K,V> elements[size];
};

struct set<T> {
    u32 size;
    T elements[size];
};

struct unordered_set<T> {
    u32 size;
    T elements[size];
};

struct vector<T> {
    u32 size;
    T elements[size];
};

struct string {
    u32 size;
    char string[size];
};

struct CCAffineTransform {
    float a;
    float b;
    float c;
    float d;
    float tx;
    float ty;
};

struct CCPoint {
    float x;
    float y;
};

struct CCSize {
    float width;
    float height;
};

struct ccColor3B {
    u8 r;
    u8 g;
    u8 b;
};

struct ccHSVValue {
    float h;
    float s;
    float v;
    bool absoluteSaturation;
    bool absoluteBrightness;
    padding[2];
};

struct CCArray<T> {
    u32 count;
    T elements[count];
};

struct GameObjectPtr {
   u32 m_index;
};

struct EnhancedGameObjectPtr {
    u32 m_index;
};

struct EnterEffectObjectPtr {
    u32 m_index;
};

struct SFXTriggerGameObjectPtr {
    u32 m_index;
};

struct DashRingObjectPtr {
    u32 m_index;
};

struct GradientTriggerObjectPtr {
    u32 m_index;
};

struct CheckpointGameObjectPtr {
   u32 m_index;
};

struct SoundPtr {
    s32 m_value; // If it's -1 then we create a sound, if -2 it was nullptr
};

struct GJValueTween {
    float m_fromValue;
    float m_toValue;
    float m_duration;
    float m_deltaTime;
    float m_currentValue;
    s32 m_easingType;
    float m_easingRate;
    bool m_finished;
    bool m_disabled;
    s32 m_unkInt1;
    s32 m_unkInt2;
};

struct GameObjectPhysics {
    GameObjectPtr m_gameObject;
    CCPoint m_unkPoint1;
    CCPoint m_unkPoint2;
    float m_unkFloat1;
    float m_unkFloat2;
    s32 m_unkInt1;
    s32 m_unkInt2;
    s32 m_unkInt3;
};

struct EventTriggerInstance {
    s32 m_targetID;
    s32 m_uniqueID;
    s32 m_controlID;
    bool m_inactive;
    vector<s32> m_remapKeys;
};

struct EnterEffectAnimValue {
    s32 m_key;
    float m_value;
    float m_distance;
    float m_duration;
    float m_elapsed;
    EasingType m_easingType;
    float m_easingRate;
    s32 m_easingBuffer;
};

struct EnterEffectInstance {
    map<s32,EnterEffectAnimValue> m_enterEffectAnimMap;
    float m_length;
    float m_lengthVariance;
    float m_offset;
    float m_offsetVariance;
    float m_offsetY;
    float m_offsetYVariance;
    float m_modFront;
    float m_modBack;
    float m_deadzone;
    float m_moveDistance;
    float m_moveDistanceVariance;
    float m_moveAngle;
    float m_moveAngleVariance;
    float m_moveX;
    float m_moveXVariance;
    float m_moveY;
    float m_moveYVariance;
    float m_relativeFade;
    float m_scaleX;
    float m_scaleXVariance;
    float m_scaleY;
    float m_scaleYVariance;
    float m_rotation;
    float m_rotationVariance;
    float m_tint;
    float m_unk074;
    float m_toOpacity;
    float m_fromOpacity;
    ccHSVValue m_hsvValue;
    float m_hue;
    float m_saturation;
    float m_value;
    EnterEffectObjectPtr m_gameObject;
    bool m_unkBool1;
    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    float m_unkFloat4;
    bool m_unkBool2;
    bool m_unkBool3;
    bool m_unkBool4;
    float m_unkFloat5;
    vector<s32> m_unkVecInt;
    float m_unkFloat6;
};

struct AdvancedFollowInstance {
    GameObjectPtr m_gameObject;
    s32 m_group;
    s32 m_objectKey;
    s32 m_controlId;
    s32 m_otherObjectKey;
    s32 m_relatedToGJGameStateUnkUint7;
    bool m_finished;
    bool m_doStart;
    bool m_started;
    bool m_processed;
};

struct DynamicObjectAction {
    GameObjectPtr m_gameObject1;
    GameObjectPtr m_gameObject2;
    GameObjectPtr m_gameObject3;
    GameObjectPtr m_gameObject4;
    GameObjectPtr m_gameObject5;
    GameObjectPtr m_gameObject6;
    GameObjectPtr m_gameObject7;
    GameObjectPtr m_gameObject8;
    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    bool m_unkBool1;
    bool m_unkBool2;
    bool m_unkBool3;
    bool m_unkBool4;
    float m_unkFloat4;
    float m_unkFloat5;
    float m_unkFloat6;
};

struct SFXTriggerInstance {
    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    SFXTriggerGameObjectPtr m_sfxTriggerGameObject;
};

struct SongTriggerGameObjectPtr {
    u32 m_index;
};

struct SongChannelState {
    SongTriggerGameObjectPtr m_songTriggerGameObject1;
    double m_unkDouble1;
    SongTriggerGameObjectPtr m_songTriggerGameObject2;
    double m_unkDouble2;
};

struct SongTriggerState {
    SongTriggerGameObjectPtr m_songTriggerGameObject;    
    double m_unkDouble;
};

struct SFXStateContainer {
    double m_unkDouble1;
    double m_unkDouble2;
    float m_unkFloat1;
    float m_unkFloat2;
    bool m_unkBool;
};

struct SFXTriggerState {
    SFXTriggerGameObjectPtr m_sfxTriggerGameObject;
    s32 m_unkInt1;
    double m_unkDouble1;
    double m_unkDouble2;
    double m_unkDouble3;
    double m_unkDouble4;
    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    float m_unkFloat4;
    s32 m_unkInt2;
    bool m_processed;
    bool m_unkBool1;
    SFXStateContainer m_sfxStateContainers[3];
};

struct CCObject {
    s32 m_eObjType;
    s32 m_nZOrder;
    s32 m_uOrderOfArrival;
};

struct CCNode {
    CCObject super;
    float m_fRotationX;
    float m_fRotationY;
    float m_fScaleX;
    float m_fScaleY;
    float m_fVertexZ;
    CCPoint m_obPosition;
    float m_fSkewX;
    float m_fSkewY;
    CCPoint m_obAnchorPointInPoints;
    CCPoint m_obAnchorPoint;
    CCSize m_obContentSize;
    CCAffineTransform m_sAdditionalTransform;
    CCAffineTransform m_sTransform;
    CCAffineTransform m_sInverse;
    bool m_bRunning;
    bool m_bTransformDirty;
    bool m_bInverseDirty;
    bool m_bAdditionalTransformDirty;
    bool m_bVisible;
    bool m_bIgnoreAnchorPointForPosition;
    bool m_bReorderChildDirty;
    bool m_bUnkBool1;
    bool m_bUnkBool2;
};

struct GJGameState {
    float m_cameraZoom;
    float m_targetCameraZoom;
    CCPoint m_cameraOffset;
    CCPoint m_unkPoint1;
    CCPoint m_unkPoint2;
    CCPoint m_unkPoint3;
    CCPoint m_unkPoint4;
    CCPoint m_unkPoint5;
    CCPoint m_unkPoint6;
    CCPoint m_unkPoint7;
    CCPoint m_unkPoint8;
    CCPoint m_unkPoint9;
    CCPoint m_unkPoint10;
    CCPoint m_unkPoint11;
    CCPoint m_unkPoint12;
    CCPoint m_unkPoint13;
    CCPoint m_unkPoint14;
    CCPoint m_unkPoint15;
    CCPoint m_unkPoint16;
    CCPoint m_unkPoint17;
    CCPoint m_unkPoint18;
    CCPoint m_unkPoint19;
    CCPoint m_unkPoint20;
    CCPoint m_unkPoint21;
    CCPoint m_unkPoint22;
    CCPoint m_unkPoint23;
    CCPoint m_unkPoint24;
    CCPoint m_unkPoint25;
    CCPoint m_unkPoint26;
    CCPoint m_unkPoint27;
    CCPoint m_unkPoint28;
    CCPoint m_unkPoint29;
    bool m_unkBool1;
    s32 m_unkInt1;
    bool m_unkBool2;
    s32 m_unkInt2;
    bool m_unkBool3;
    CCPoint m_unkPoint30;
    float m_middleGroundOffsetY;
    s32 m_unkInt3;
    s32 m_unkInt4;
    bool m_unkBool4;
    bool m_unkBool5;
    float m_unkFloat2;
    float m_unkFloat3;
    s32 m_unkInt5;
    s32 m_unkInt6;
    s32 m_unkInt7;
    s32 m_unkInt8;
    s32 m_unkInt9;
    s32 m_unkInt10;
    s32 m_unkInt11;
    float m_unkFloat4;
    float m_unkUint1;
    float m_portalY;
    bool m_unkBool6;
    bool m_gravityRelated;
    s32 m_unkInt12;
    s32 m_unkInt13;
    s32 m_unkInt14;
    s32 m_unkInt15;
    bool m_unkBool7;
    bool m_unkBool8;
    bool m_unkBool9;
    float m_unkFloat5;
    float m_unkFloat6;
    float m_unkFloat7;
    float m_unkFloat8;
    float m_cameraAngle;
    float m_targetCameraAngle;
    bool m_playerStreakBlend;
    float m_timeWarp;
    float m_timeWarpRelated;
    s32 m_currentChannel;
    s32 m_unkInt17;
    unordered_map<s32,s32> m_spawnChannelRelated0;
    unordered_map<s32,bool> m_spawnChannelRelated1;
    double m_totalTime;
    double m_levelTime;
    double m_unkDouble3;
    s32 m_unkIntRelatedCurrentProgress1;
    s32 m_unkIntRelatedCurrentProgress2;
    s32 m_currentProgress;
    s32 m_unkIntRelatedCurrentProgress3;
    s32 m_unkIntRelatedCurrentProgress4;
    float m_unkUint6;
    float m_unkUint7;
    GameObjectPtr m_unkGameObjPtr1;
    GameObjectPtr m_unkGameObjPtr2;
    CCPoint m_cameraPosition;
    bool m_unkBool10;
    float m_levelFlipping;
    bool m_unkBool11;
    bool m_unkBool12;
    bool m_isDualMode;
    float m_unkFloat9;
    unordered_map<s32,GJValueTween> m_tweenActions;
    s32 m_cameraEdgeValue0;
    s32 m_cameraEdgeValue1;
    s32 m_cameraEdgeValue2;
    s32 m_cameraEdgeValue3;
    unordered_map<s32,GameObjectPhysics> m_gameObjectPhysics;
    vector<float> m_unkVecFloat1;
    float m_unkUint10;
    float m_unkUint11;
    float m_unkUint12;
    CCPoint m_unkPoint31;
    float m_unkFloat10;
    u32 m_timeModRelated;
    bool m_timeModRelated2;
    map<pair<s32,s32>,s32> m_unkMapPairIntIntInt;
    float m_unkUint13;
    CCPoint m_unkPoint32;
    CCPoint m_unkPoint33;
    bool m_unkBool20;
    bool m_unkBool21;
    bool m_unkBool22;
    float m_unkUint14;
    bool m_unkBool26;
    bool m_cameraShakeEnabled;
    float m_cameraShakeFactor;
    float m_unkUint15;
    float m_unkUint16;
    u64 m_unkUint64_1;
    CCPoint m_unkPoint34;
    u32 m_dualRelated;
    unordered_map<s32,EnhancedGameObjectPtr> m_stateObjects;
    map<pair<GJGameEvent,s32>,vector<EventTriggerInstance>> m_unkMapPairGJGameEventIntVectorEventTriggerInstance;
    map<pair<GJGameEvent,s32>,s32> m_unkMapPairGJGameEventIntInt;
    unordered_map<s32,vector<EnterEffectInstance>> m_unorderedMapEnterEffectInstanceVectors1;
    unordered_map<s32,vector<EnterEffectInstance>> m_unorderedMapEnterEffectInstanceVectors2;
    vector<s32> m_unkVecInt1;
    vector<s32> m_unkVecInt2;
    vector<EnterEffectInstance> m_enterEffectInstances1;
    vector<EnterEffectInstance> m_enterEffectInstances2;
    vector<EnterEffectInstance> m_enterEffectInstances3;
    vector<EnterEffectInstance> m_enterEffectInstances4;
    vector<EnterEffectInstance> m_enterEffectInstances5;
    unordered_set<s32> m_unkUnorderedSet1;
    bool m_unkBool27;
    vector<AdvancedFollowInstance> m_advanceFollowInstances;
    vector<DynamicObjectAction> m_dynamicObjActions1;
    vector<DynamicObjectAction> m_dynamicObjActions2;
    bool m_unkBool28;
    bool m_unkBool29;
    float m_unkUint17;
    unordered_map<s32,vector<s32>> m_unkUMap8;
    map<pair<s32,s32>,SFXTriggerInstance> m_proximityVolumeRelated;
    unordered_map<s32,SongChannelState> m_songChannelStates;
    unordered_map<s32,vector<SongTriggerState>> m_songTriggerStateVectors;
    vector<SFXTriggerState> m_sfxTriggerStates;
    bool m_unkBool30;
    float m_unkUint18;
    s32 m_ground;
    float m_unkUint19;
    bool m_unkBool31;
    float m_unkUint20;
    bool m_unkBool32;
    u32 m_pauseCounter;
    u32 m_pauseBufferTimer;
};

struct GJShaderState {
    unordered_map<u32,GJValueTween> m_someIntToValueTweenMap;
    unordered_map<u32,double> m_someIntToDoubleMap;
    double m_time;
    double m_prevTime;
    double m_startTime;
    float m_textureScaleX;
    float m_textureScaleY;
    ccColor3B m_blurRefColor;
    float m_blurIntensity;
    s32 m_blurUnk60;
    bool m_blurOnlyEmpty;
    float m_shockWaveUnk68;
    float m_shockWaveUnk6c;
    float m_shockWaveUnk70;
    float m_shockWaveUnk74;
    float m_shockWaveUnk78;
    float m_shockWaveUnk7c;
    float m_shockWaveUnk80;
    float m_shockWaveUnk84;
    float m_shockWaveUnk88;
    bool m_shockWaveInvert;
    float m_shockWaveUnk90;
    float m_shockWaveUnk94;
    s32 m_shockWaveUnk98;
    bool m_shockWaveUnk9c;
    bool m_shockWaveCenterMoving;
    bool m_shockWaveUnk9e;
    float m_shockWaveUnka0;
    CCPoint m_shockWaveUnka4;
    bool m_shockWaveCenterDirty;
    CCPoint m_shockWaveCenter;
    double m_shockLineUnkb8;
    float m_shockLineUnkc0;
    float m_shockLineUnkc4;
    float m_shockLineUnkc8;
    bool m_shockLineAxis;
    bool m_shockLineDirection;
    bool m_shockLineDual;
    bool m_shockLineUnkcf;
    float m_shockLineUnkd0;
    float m_shockLineUnkd4;
    float m_shockLineUnkd8;
    float m_shockLineUnkdc;
    float m_shockLineUnke0;
    float m_shockLineUnke4;
    s32 m_shockLineUnke8;
    bool m_shockLineUnkec;
    bool m_shockLineCenterMoving;
    bool m_shockLineUnkee;
    float m_shockLineUnkf0;
    CCPoint m_shockLineUnkf4;
    bool m_shockLineCenterDirty;
    CCPoint m_shockLineCenter;
    float m_glitchUnk108;
    float m_glitchUnk10c;
    float m_glitchUnk110;
    float m_glitchUnk114;
    float m_glitchUnk118;
    float m_glitchUnk11c;
    bool m_glitchUnk120;
    float m_chromaticUnk124;
    float m_chromaticUnk128;
    bool m_chromaticUnk12c;
    float m_cGUnk130;
    float m_cGUnk134;
    float m_cGUnk138;
    float m_cGUnk13c;
    float m_cGUnk140;
    float m_cGUnk144;
    float m_cGUnk148;
    float m_cGUnk14c;
    bool m_cGUnk150;
    bool m_cGUnk151;
    bool m_cGUnk152;
    bool m_cGUnk153;
    float m_pixelateTargetX;
    float m_pixelateTargetY;
    bool m_pixelateSnapGrid;
    bool m_pixelatePixelating;
    bool m_pixelateRelative;
    bool m_pixelateHardEdges;
    float m_lensCircleUnk160;
    float m_lensCircleUnk164;
    float m_lensCircleStrength;
    s32 m_lensCircleUnk16c;
    s32 m_lensCircleUnk170;
    bool m_lensCircleUnk174;
    bool m_lensCircleAdditive;
    char m_lensCircleTintR;
    char m_lensCircleTintG;
    char m_lensCircleTintB;
    CCPoint m_lensCircleUnk17c;
    CCPoint m_lensCircleUnk184;
    float m_radialBlurUnk18c;
    float m_radialBlurUnk190;
    bool m_radialBlurUnk194;
    s32 m_radialBlurUnk198;
    CCPoint m_radialBlurUnk19c;
    CCPoint m_radialBlurUnk1a4;
    float m_motionBlurUnk1ac;
    float m_motionBlurUnk1b0;
    float m_motionBlurSpeedX;
    float m_motionBlurSpeedY;
    float m_motionBlurUnk1bc;
    float m_motionBlurUnk1c0;
    s32 m_motionBlurUnk1c4;
    s32 m_motionBlurUnk1c8;
    bool m_motionBlurDual;
    bool m_motionBlurUnk1cd;
    float m_bulgeValue;
    bool m_bulgeUnk1d4;
    s32 m_bulgeUnk1d8;
    float m_bulgeRadius;
    CCPoint m_bulgeUnk1e0;
    CCPoint m_bulgeUnk1e8;
    float m_pinchUnk1f0;
    float m_pinchUnk1f4;
    bool m_pinchUnk1f8;
    bool m_pinchUnk1f9;
    s32 m_pinchUnk1fc;
    s32 m_pinchUnk200;
    CCPoint m_pinchUnk204;
    CCPoint m_pinchUnk20c;
    bool m_pinchUnk214;
    float m_pinchUnk218;
    CCPoint m_pinchUnk21c;
    float m_grayscaleValue;
    bool m_grayscaleUseLum;
    s32 m_grayscaleUnk22c;
    ccColor3B m_grayscaleTint;
    float m_sepiaValue;
    float m_invertColorEditRGB;
    float m_invertColorR;
    float m_invertColorG;
    float m_invertColorB;
    bool m_invertColorClampRGB;
    float m_hueShiftDegrees;
    float m_colorChangeCR;
    float m_colorChangeCG;
    float m_colorChangeCB;
    float m_colorChangeBR;
    float m_colorChangeBG;
    float m_colorChangeBB;
    float m_splitUnk268;
    float m_splitUnk26c;
    bool m_splitUnk270;
    s32 m_minBlendingLayer;
    s32 m_maxBlendingLayer;
    bool m_zLayerDirty;
    bool m_somethingZLayerUnk27d;
    bool m_usesShaders;
};

struct FMODSoundTween {
    double m_unkDouble1;
    double m_unkDouble2;
    double m_unkDouble3;
};

struct FMODQueuedMusic {
    string m_filePath;
    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    s32 m_unkInt1;
    s32 m_unkInt2;
    s32 m_unkInt3;
    s32 m_unkInt4;
    bool m_unkBool1;
    s32 m_keyForFMODMusicMap1;
    bool m_unkBool2;
    s32 m_keyForFMODMusicMap2;
    bool m_unkBool3;
    float m_unkFloat4;
    SoundPtr m_sound;
    bool m_dontReset;
};

struct SoundStateContainer {
    s32 m_fadePointCount;
    float m_fadePointVolumes[4];
    u64 m_fadePointOffsetsp[4];
    u64 m_currentOffset;
    s32 m_loopStartMs;
    s32 m_loopEndMs;
    s32 m_currentMs;
    u64 m_playStartOffset;
    u64 m_playEndOffset;
    s32 m_usePlayOffsets;
};

struct FMODSoundState {
    string m_filePath;
    float m_speed;
    float m_unkFloat1;
    float m_volume;
    bool m_shouldLoop;
    s32 m_channelID;
    SoundStateContainer m_soundStateContainer;
    s32 m_uniqueID;
    s32 m_sfxGroup;
    float m_pitch;
    bool m_fastFourierTransform;
    bool m_reverb;
    s32 m_unkInt1;
    bool m_unkBool1;
    s32 m_keyForFMODMusicMap;
    bool m_unkBool2;
};

struct FMODAudioState {
    float m_unkFloat1;
    float m_unkFloat2;
    map<pair<s32,s32>,FMODSoundTween> m_unkMapPairIntIntFMODSoundTween1;
    map<pair<s32,s32>,FMODSoundTween> m_unkMapPairIntIntFMODSoundTween2;
    map<pair<s32,s32>,FMODSoundTween> m_unkMapPairIntIntFMODSoundTween3;
    unordered_map<s32,float> m_volumeForChannels1;
    unordered_map<s32,float> m_volumeForChannels2;
    unordered_map<s32,float> m_pitchForChannels1;
    unordered_map<s32,float> m_volumeForChannels3;
    unordered_map<s32,float> m_volumeForChannels4;
    unordered_map<s32,float> m_pitchForChannels2;
    unordered_map<s32,float> m_unkMapIntFloat1;
    unordered_map<s32,float> m_unkMapIntFloat2;
    unordered_map<s32,float> m_unkMapIntFloat3;
    unordered_map<s32,float> m_unkMapIntFloat4;
    unordered_map<s32,FMODQueuedMusic> m_unkMapIntFMODQueuedMusic1;
    unordered_map<s32,FMODQueuedMusic> m_unkMapIntFMODQueuedMusic2;
    unordered_map<s32,FMODSoundState> m_unkMapIntFMODSoundState;
    u64 m_unkUint64_1;
};

struct PlayerCheckpoint {
    CCNode super;
    CCPoint m_position;
    CCPoint m_lastPosition;
    float m_yVelocity;
    bool m_isUpsideDown;
    bool m_isSideways;
    bool m_isShip;
    bool m_isBall;
    bool m_isBird;
    bool m_isSwing;
    bool m_isDart;
    bool m_isRobot;
    bool m_isSpider;
    bool m_isOnGround;
    GhostType m_ghostType;
    bool m_miniMode;
    float m_speed;
    bool m_hidden;
    bool m_goingLeft;
    float m_reverseSpeed;
    bool m_dashing;
    float m_dashX;
    float m_dashY;
    float m_dashAngle;
    float m_dashStartTime;
    DashRingObjectPtr m_dashRingObject;
    bool m_platformerCheckpoint;
    double m_lastFlipTime;
    float m_gravityMod;
    bool m_decreaseBoostSlide;
    s32 m_followRelated;
    vector<float> m_playerFollowFloats;
    float m_followRelated2;
};

struct SavedObjectStateRef {
    GameObjectPtr m_gameObject;
    double m_unkDouble1;
    double m_unkDouble2;
    float m_unkFloat1;
    float m_unkFloat2;
    float m_unkFloat3;
    float m_unkFloat4;
    float m_unkFloat5;
    float m_unkFloat6;
};

struct SavedActiveObjectState {
    GameObjectPtr m_gameObject;
    bool m_unkBool1;
    bool m_unkBool2;
};

struct SavedSpecialObjectState {
    GameObjectPtr m_gameObject;
    s32 m_animationID;
};

struct CAState {
    ccColor3B m_fromColor;
    ccColor3B m_toColor;
    ccColor3B m_color;
    bool m_property19;
    bool m_blending;
    bool m_copyOpacity;
    bool m_unknown;
    s32 m_playerColor;
    s32 m_colorID;
    s32 m_copyID;
    s32 m_unknown2;
    float m_duration;
    float m_fromOpacity;
    float m_toOpacity;
    float m_deltaTime;
    float m_unknown3;
    ccHSVValue m_copyHSV;
};

struct PulseEffectAction {
    bool m_disabled;
    float m_fadeInTime;
    float m_holdTime;
    float m_fadeOutTime;
    float m_deltaTime;
    s32 m_targetGroupID;
    float m_currentValue;
    ccColor3B m_color;
    PulseEffectType m_pulseEffectType;
    ccHSVValue m_hsv;
    s32 m_colorIndex;
    bool m_mainOnly;
    bool m_detailOnly;
    bool m_isDynamicHsv;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    float m_startTime;
};

struct CountTriggerAction {
    bool m_disabled;
    s32 m_previousCount;
    s32 m_targetCount;
    s32 m_targetGroupID;
    bool m_activateGroup;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    s32 m_itemID;
    bool m_multiActivate;
    vector<s32> m_remapKeys;
};

struct OpacityEffectAction {
    float m_duration;
    float m_fromValue;
    float m_toValue;
    bool m_finished;
    bool m_disabled;
    float m_deltaTime;
    s32 m_targetGroupID;
    float m_currentValue;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    float m_deltaTimeRelated;
    float m_durationRelated;
};

struct TouchToggleAction {
    bool m_disabled;
    s32 m_targetGroupID;
    bool m_holdMode;
    TouchTriggerType m_touchTriggerType;
    TouchTriggerControl m_touchTriggerControl;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    bool m_dualMode;
    vector<s32> m_remapKeys;
};

struct CollisionTriggerAction {
    bool m_disabled;
    s32 m_blockAID;
    s32 m_blockBID;
    s32 m_targetGroupID;
    s32 m_triggerOnExit;
    bool m_activateGroup;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    vector<s32> m_remapKeys;
};

struct ToggleTriggerAction {
    bool m_disabled;
    s32 m_targetGroupID;
    bool m_activateGroup;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    vector<s32> m_unkVecInt;
};

struct SpawnTriggerAction {
    bool m_finished;
    bool m_disabled;
    double m_duration;
    double m_deltaTime;
    s32 m_targetGroupID;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    bool m_spawnOrdered;
    GameObjectPtr m_gameObject;
    vector<s32> m_unkVecInt;
};

struct tk_spline {
    vector<double> m_x;
    vector<double> m_y;
    vector<double> m_b;
    vector<double> m_c;
    vector<double> m_d;
    double m_c0;
    s32 m_type;
    s32 m_left;
    s32 m_right;
    double m_leftValue;
    double m_rightValue;
    bool m_madeMonotonic;
};

struct KeyframeObject {
    double m_unk000;
    s32 m_unk008;
    float m_unk00c;
    bool m_unk010;
    s32 m_unk014;
    float m_unk018;
    bool m_unk01c;
    bool m_unk01d;
    bool m_unk01e;
    tk_spline m_spline1;
    tk_spline m_spline2;
    double m_unk170;
    double m_unk178;
    s32 m_unk180;
    s32 m_unk184;
    CCPoint m_unk188;
    CCPoint m_unk190;
    double m_unk198;
    double m_unk1a0;
    double m_unk1a8;
    double m_unk1b0;
    float m_unk1b8;
};

struct GroupCommandObject2 {
    s32 m_groupCommandUniqueID;
    CCPoint m_moveOffset;
    EasingType m_easingType;
    double m_easingRate;
    double m_duration;
    double m_deltaTime;
    s32 m_targetGroupID;
    s32 m_centerGroupID;
    double m_currentXOffset;
    double m_currentYOffset;
    double m_deltaX;
    double m_deltaY;
    double m_oldDeltaX;
    double m_oldDeltaY;
    double m_lockedCurrentXOffset;
    double m_lockedCurrentYOffset;
    bool m_finished;
    bool m_disabled;
    bool m_finishRelated;
    bool m_lockToPlayerX;
    bool m_lockToPlayerY;
    bool m_lockToCameraX;
    bool m_lockToCameraY;
    bool m_lockedInX;
    bool m_lockedInY;
    double m_moveModX;
    double m_moveModY;
    double m_currentRotateOrTransformValue;
    double m_currentRotateOrTransformDelta;
    double m_someInterpValue1RelatedOne;
    double m_someInterpValue2RelatedOne;
    double m_rotationOffset;
    bool m_lockObjectRotation;
    s32 m_targetPlayer;
    double m_followXMod;
    double m_followYMod;
    s32 m_commandType;
    double m_someInterpValue1;
    double m_someInterpValue2;
    double m_keyframeRelated;
    double m_targetScaleX;
    double m_targetScaleY;
    double m_transformTriggerProperty450;
    double m_transformTriggerProperty451;
    double m_someInterpValue1RelatedZero;
    double m_someInterpValue2RelatedZero;
    bool m_onlyMove;
    bool m_transformRelatedFalse;
    bool m_relativeRotation;
    double m_someInterpValue1Related;
    double m_someInterpValue2Related;
    double m_followYSpeed;
    double m_followYDelay;
    s32 m_followYOffset;
    double m_followYMaxSpeed;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    double m_deltaX_3;
    double m_deltaY_3;
    double m_oldDeltaX_3;
    double m_oldDeltaY_3;
    double m_Delta_3_Related;
    double m_unkDoubleMaybeUnused;
    s32 m_actionType1;
    s32 m_actionType2;
    double m_actionValue1;
    double m_actionValue2;
    bool m_someInterpValue1RelatedFalse;
    float m_deltaTimeInFloat;
    bool m_alreadyUpdated;
    bool m_doUpdate;
    vector<KeyframeObject> m_keyframes;
    CCPoint m_splineRelated;
    GameObjectPtr m_gameObject;
    float m_gameObjectRotation;
    vector<s32> m_remapKeys;
    bool m_someInterpValue2RelatedTrue;
    float m_unkFloat204;
};

struct TimerItem {
    s32 m_itemID;
    double m_time;
    bool m_paused;
    float m_timeMod;
    bool m_ignoreTimeWarp;
    double m_targetTime;
    bool m_stopTimeEnabled;
    s32 m_targetGroupID;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    vector<s32> m_remapKeys;
    bool m_disabled;
};

struct TimerTriggerAction {
    bool m_disabled;
    float m_time;
    float m_targetTime;
    s32 m_targetGroupID;
    s32 m_triggerUniqueID;
    s32 m_controlID;
    s32 m_itemID;
    bool m_multiActivate;
    vector<s32> m_remapKeys;
};

struct EffectManagerState {
    vector<CAState> m_unkVecCAState;
    vector<PulseEffectAction> m_unkVecPulseEffectAction;
    unordered_map<s32,vector<PulseEffectAction>> m_unorderedMapInt_vectorPulseEffectAction;
    unordered_map<s32,vector<CountTriggerAction>> m_unorderedMapInt_vectorCountTriggerAction;
    unordered_set<s32> m_unorderedSet_int1;
    map<s32,s32> m_mapInt_Int;
    unordered_map<s32,OpacityEffectAction> m_unorderedMapInt_OpacityEffectAction;
    vector<TouchToggleAction> m_vectorTouchToggleAction;
    vector<CollisionTriggerAction> m_vectorCollisionTriggerAction;
    vector<ToggleTriggerAction> m_vectorToggleTriggerAction;
    vector<SpawnTriggerAction> m_vectorSpawnTriggerAction;
    unordered_map<s32,s32> m_itemCountMap;
    unordered_map<s32,bool> m_unorderedMapInt_bool;
    vector<GroupCommandObject2> m_vectorGroupCommandObject2;
    unordered_map<s32,pair<double,double>> m_unorderedMapInt_pair_double_double;
    unordered_set<s32> m_unorderedSet_int2;
    unordered_map<s32,TimerItem> m_timerItemMap;
    unordered_map<s32,vector<TimerTriggerAction>> m_unorderedMapInt_vectorTimerTriggerAction;
};

struct SequenceTriggerState {
    unordered_map<s32,float> m_unkUnorderedMap1;
    unordered_map<s32,s32> m_unkUnorderedMap2;
};

struct CheckpointObject {
    CCNode super;
    GJGameState m_gameState;
    GJShaderState m_shaderState;
    FMODAudioState m_audioState;
    CCPoint m_physicalCheckpointObjectStartPos;
    PlayerCheckpoint m_player1Checkpoint;
    bool m_hasPlayer2;
    if (m_hasPlayer2) {
         PlayerCheckpoint m_player2Checkpoint;
    }
    s32 m_unke78;
    s32 m_unke7c;
    float m_unke80;
    bool m_ground2Invisible;
    bool m_streakBlend;
    s32 m_uniqueID;
    s32 m_respawnID;
    vector<SavedObjectStateRef> m_vectorSavedObjectStateRef;
    vector<SavedActiveObjectState> m_vectorActiveSaveObjectState;
    vector<SavedSpecialObjectState> m_vectorSpecialSaveObjectState;
    EffectManagerState m_effectManagerState;
    bool m_hasGradientTriggerObjectArray;
    if (m_hasGradientTriggerObjectArray) {
        CCArray<GradientTriggerObjectPtr> m_gradientTriggerObjectArray;
    }
    bool m_unk11e8;
    unordered_map<s32,SequenceTriggerState> m_sequenceTriggerStateUnorderedMap;
    s32 m_commandIndex;
    double timePlayed;
    u64 timestamp;
};

bitfield PSFData {
    unsigned m_originalVersion : 5;
    unsigned m_platform : 3;
    bool m_updatedFromPreviousLevelVersion : 1;
    bool m_lowDetailMode : 1;
    padding : 6;
};

struct PSF {
    u8 psfHeader[11];
    std::assert(psfHeader[5] == '0' && psfHeader[7] == '1' && psfHeader[9] == '1', "Incorrect PSF version");
    bool finishedSaving;
    PSFData psfData;
    padding[2];
    u32 levelStringChecksum;
    u32 checkpointObjectCount;
    CheckpointObject checkpointObjects[checkpointObjectCount];
    u32 activatedCheckpointCount;
    CheckpointGameObjectPtr activatedCheckpoints[activatedCheckpointCount];
    unordered_map<s32,s32> m_persistentItemCountMap;
    unordered_set<s32> m_persistentTimerItemSet;
    s32 m_attempts;
};

PSF file @ 0x0;

std::assert(std::mem::eof(), "Whole input not consumed");