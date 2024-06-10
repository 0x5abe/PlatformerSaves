#pragma pattern_limit 16777216

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
    u8 pad;
};

struct ccHSVValue {
    float h;
    float s;
    float v;
    bool absoluteSaturation;
    bool absoluteBrightness;
    u8 pad[2];
};

struct CCArray<T> {
    u32 count;
    T elements[count];
};

struct CCObject {
    s32 m_eObjType;
    s32 m_nZOrder;
    s32 m_uOrderOfArrival;
};

struct GameObjectPtr {
   u32 m_index;
};

struct GJValueTween {
    u8 m_unk[40];
};

struct GameObjectPhysics {
    u8 m_unk[40];
};

struct EnhancedGameObjectPtr {
    u32 m_index;
};

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
    FallSpeedHigh = 78
};

struct EventTriggerInstance {
    u8 m_unk[16];
    vector<s32> m_unkVecInt;
};

struct EnterEffectAnimValue {
    u8 m_unk[32];
};

struct EnterEffectInstance {
    map<s32,EnterEffectAnimValue> m_enterEffectAnimMap;
    u8 m_unk[140];
    GameObjectPtr m_gameObject;
    u8 m_unk2[28];
    vector<s32> m_unkVecInt;
    u8 m_unk3[4];
};

struct AdvancedFollowInstance {
    GameObjectPtr m_gameObject;
    u8 m_unk[24];
};

struct DynamicObjectAction {
    u8 m_unk[60];
};

struct SFXTriggerGameObjectPtr {
    u32 m_index;
}; 

struct SFXTriggerInstance {
    u8 m_unk[16];
	SFXTriggerGameObjectPtr m_sfxTriggerGameObject;
};

struct SongTriggerGameObjectPtr {
    u32 m_index;
};

struct SongChannelState {
    u8 m_unk[4];
    SongTriggerGameObjectPtr m_songTriggerGameObject1;
    u8 m_unk2[12];
    SongTriggerGameObjectPtr m_songTriggerGameObject2;
    u8 m_unk3[12];
};

struct SongTriggerState {
    SongTriggerGameObjectPtr m_songTriggerGameObject;    
    u8 m_unk[12];
};

struct SFXTriggerState {
    SFXTriggerGameObjectPtr m_sfxTriggerGameObject;
    u8 m_unk[156];
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
    float m_zoom;
    float m_unkFloat1;
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
    u32 m_unkUint1;
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
    bool m_unk184;
    float m_timeWarp;
    float m_timeWarpRelated;
    s32 m_currentChannel;
    s32 m_unkInt17;
    unordered_map<s32, s32> m_spawnChannelRelated0;
    unordered_map<s32, bool> m_spawnChannelRelated1;
    double m_unkDouble1;
    double m_unkDouble2;
    double m_unkDouble3;
    u32 m_unkUint2;
    u32 m_unkUint3;
    s32 m_currentProgress;
    u32 m_unkUint4;
    u32 m_unkUint5;
    u32 m_unkUint6;
    u32 m_unkUint7;
    GameObjectPtr m_unkGameObjPtr1;
    GameObjectPtr m_unkGameObjPtr2;
    CCPoint m_cameraPosition;
    bool m_unkBool10;
    u32 m_unkUint8;
    bool m_unkBool11;
    bool m_unkBool12;
    bool m_isDualMode;
    float m_unkFloat9;
    unordered_map<s32, GJValueTween> m_tweenActions;
    s32 m_cameraEdgeValue0;
    s32 m_cameraEdgeValue1;
    s32 m_cameraEdgeValue2;
    s32 m_cameraEdgeValue3;
    unordered_map<s32, GameObjectPhysics> m_gameObjectPhysics;
    vector<float> m_unkVecFloat1;
    u32 m_unkUint10;
    u32 m_unkUint11;
    u32 m_unkUint12;
    CCPoint m_unkPoint31;
    float m_unkFloat10;
    u32 m_timeModRelated;
    bool m_timeModRelated2;
    map<pair<s32, s32>, s32> m_unkMapPairIntIntInt;
    u32 m_unkUint13;
    CCPoint m_unkPoint32;
    CCPoint m_unkPoint33;
    bool m_unkBool20;
    bool m_unkBool21;
    bool m_unkBool22;
    u32 m_unkUint14;
    bool m_unkBool26;
    bool m_cameraShakeEnabled;
    float m_cameraShakeFactor;
    u32 m_unkUint15;
    u32 m_unkUint16;
    u64 m_unkUint64_1;
    CCPoint m_unkPoint34;
    u32 dualRelated;
    unordered_map<s32, EnhancedGameObjectPtr> m_stateObjects;
    map<pair<GJGameEvent, s32>, vector<EventTriggerInstance>> m_unkMapPairGJGameEventIntVectorEventTriggerInstance;
    map<pair<GJGameEvent, s32>, s32> m_unkMapPairGJGameEventIntInt;
    unordered_map<s32, vector<EnterEffectInstance>> m_unorderedMapEnterEffectInstanceVectors1;
    unordered_map<s32, vector<EnterEffectInstance>> m_unorderedMapEnterEffectInstanceVectors2;
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
    u32 m_unkUint17;
    unordered_map<s32, vector<s32>> unkUMap8;
    map<pair<s32,s32>, SFXTriggerInstance> proximityVolumeRelated;
    unordered_map<s32, SongChannelState> m_songChannelStates;
    unordered_map<s32, vector<SongTriggerState>> m_songTriggerStateVectors;
    vector<SFXTriggerState> m_sfxTriggerStates;
    bool m_unkBool30;
    u32 m_unkUint18;
    s32 m_ground;
    u32 m_unkUint19;
    bool m_unkBool31;
    u32 m_unkUint20;
    bool m_unkBool32;
    u32 m_unkUint21;
    u32 m_unkUint22;
};

struct GJShaderState {
    unordered_map<u32, GJValueTween> m_someIntToValueTweenMap;
    unordered_map<u32, double> m_someIntToDoubleMap;
    u8 m_unk[584];
};

struct FMODSoundTween {
    u8 m_unk[40];
};

struct FMODQueuedMusic {
    string m_unkString;
    u8 m_unk[56];
};

struct FMODSoundState {
    string m_unkString;
    u8 m_unk2[164];
};

struct FMODAudioState {
    float m_unkFloat1;
    float m_unkFloat2;
    map<pair<s32,s32>,FMODSoundTween> m_unkMapPairIntIntFMODSoundTween1;
    map<pair<s32,s32>,FMODSoundTween> m_unkMapPairIntIntFMODSoundTween2;
    map<pair<s32,s32>,FMODSoundTween> m_unkMapPairIntIntFMODSoundTween3;
    unordered_map<s32,float> m_unkMapIntFloat1;
    unordered_map<s32,float> m_unkMapIntFloat2;
    unordered_map<s32,float> m_unkMapIntFloat3;
    unordered_map<s32,float> m_unkMapIntFloat4;
    unordered_map<s32,float> m_unkMapIntFloat5;
    unordered_map<s32,float> m_unkMapIntFloat6;
    unordered_map<s32,float> m_unkMapIntFloat7;
    unordered_map<s32,float> m_unkMapIntFloat8;
    unordered_map<s32,float> m_unkMapIntFloat9;
    unordered_map<s32,float> m_unkMapIntFloat10;
    unordered_map<s32,FMODQueuedMusic> m_unkMapIntFMODQueuedMusic1;
    unordered_map<s32,FMODQueuedMusic> m_unkMapIntFMODQueuedMusic2;
    unordered_map<s32,FMODSoundState> m_unkMapIntFMODSoundState;
    s32 m_unkInt1;
    s32 m_unkInt2;
};

struct PlayerCheckpoint {
    CCNode super;
    CCPoint m_position;
    CCPoint m_lastPosition;
    s32 m_unkInt1;
    bool m_isUpsideDown;
    bool m_unk7b3;
    bool m_isShip;
    bool m_isBall;
    bool m_isBird;
    bool m_isSwing;
    bool m_isDart;
    bool m_isRobot;
    bool m_isSpider;
    bool m_isOnGround;
    s32 m_hasGhostTrail;
    u8 m_unkBytes1[4];
    float m_speed;
    bool m_isHidden;
    bool m_isGoingLeft;
    u8 m_unkBytes2[27];
    u64 m_maybeAPointer;
    bool m_unkBool;
    float m_unkFloat1;
    s32 m_possiblyFlags;
    s32 m_timeOrPercentRelated;
    u8 m_unkBytes3[4];
    vector<float> m_yPositionVector;
    u8 m_unkBytes4[8];
};

struct DynamicSaveObject {
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

struct ActiveSaveObject1 {
    GameObjectPtr m_gameObject;
    bool m_unkBool1;
    bool m_unkBool2;
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

struct ActiveSaveObject2 {
    GameObjectPtr m_gameObject;
    EasingType m_easingType;
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
    u8 m_unk[72];
};

struct CountTriggerAction {
    u8 m_unk[36];
    vector<s32> m_unkVecInt;
};

struct OpacityEffectAction {
    u8 m_unk[44];
};

struct TouchToggleAction {
    u8 m_unk[32];
    vector<s32> m_unkVecInt;
};

struct CollisionTriggerAction {
    u8 m_unk[32];
    vector<s32> m_unkVecInt;
};

struct ToggleTriggerAction {
    u8 m_unk[20];
    vector<s32> m_unkVecInt;
};

struct SpawnTriggerAction {
    u8 m_unk[40];
    GameObjectPtr m_gameObject;
    vector<s32> m_unkVecInt;
};

struct KeyframeObject {
    u8 m_unk[32];
    vector<double> m_unkVecDouble1;
    vector<double> m_unkVecDouble2;
    vector<double> m_unkVecDouble3;
    vector<double> m_unkVecDouble4;
    vector<double> m_unkVecDouble5;
    u8 m_unk2[52];
    vector<double> m_unkVecDouble6;
    vector<double> m_unkVecDouble7;
    vector<double> m_unkVecDouble8;
    vector<double> m_unkVecDouble9;
    vector<double> m_unkVecDouble10;
    u8 m_unk3[132];
};

struct GroupCommandObject2 {
    u8 m_unk[436];
    vector<KeyframeObject> m_unkVecKeyframeObject;
    u8 m_unk2[16];
    vector<s32> m_unkVecInt;
    u8 m_unk3[12];
};

struct TimerItem {
    u8 m_unk[56];
    vector<s32> m_unkVecInt;
    u8 m_unk2[4];
};

struct TimerTriggerAction {
    u8 m_unk[32];
    vector<s32> m_unkVecInt;
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
    unordered_map<s32,s32> m_unorderedMapInt_int;
    unordered_map<s32,bool> m_unorderedMapInt_bool;
    vector<GroupCommandObject2> m_vectorGroupCommandObject2;
    unordered_map<s32,pair<double,double>> m_unorderedMapInt_pair_double_double;
    unordered_set<s32> m_unorderedSet_int2;
    unordered_map<s32,TimerItem> m_unorderedMapInt_TimerItem;
    unordered_map<s32,vector<TimerTriggerAction>> m_unorderedMapInt_vectorTimerTriggerAction;
};

struct GradientTriggerObjectPtr {
    u32 m_index;
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
    s32 m_unkInt1;
    s32 m_unkInt2;
    s32 m_unkInt3;
    s16 m_unkShort1;
    s32 m_unkInt4;
    s32 m_unkInt5;
    vector<DynamicSaveObject> m_vectorDynamicSaveObjects;
    vector<ActiveSaveObject1> m_vectorActiveSaveObjects1;
    vector<ActiveSaveObject2> m_vectorActiveSaveObjects2;
    EffectManagerState m_effectManagerState;
    bool m_hasGradientTriggerObjectArray;
    if (m_hasGradientTriggerObjectArray) {
         CCArray<GradientTriggerObjectPtr> m_gradientTriggerObjectArray;
    }
    bool m_unkBool1;
    unordered_map<s32,SequenceTriggerState> m_sequenceTriggerStateUnorderedMap;
    s32 m_unkGetsCopiedFromGameState;
};

struct PSF {
    u8 psfHeader[11];
    bool finishedSaving;
    padding[4];
    u32 levelStringChecksum;
    u32 checkpointObjectCount;
    CheckpointObject checkpointObjects[checkpointObjectCount];
};

PSF file @ 0x0;