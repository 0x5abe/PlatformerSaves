#pragma once
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <domain/CheckpointGameObjectReference.hpp>
#include <hooks/CheckpointObject.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>
#include <util/platform.hpp>

class PSPlayLayer;

extern PSPlayLayer* s_currentPlayLayer;
extern char s_psfMagicAndVer[11];
extern int s_psfVersion;

enum class LoadingState {
	Ready,
	Setup,
	WaitingForPlayLevelMenuPopup,
	SetupFileRead,
	HandleFileError,
	ReadVersion,
	HandleIncorrectVersion,
	ReadFinishedSaving,
	HandleDidNotFinishSaving,
	ReadPSFData,
	ShowPlatformError,
	ShowPlatformWarning,
	ShowLevelVersionWarning,
	ShowPSFVersionWarning,
	ReadLowDetailMode,
	HandleIncorrectLowDetailMode,
	ReadHash,
	HandleIncorrectHash,
	ReadCheckpointCount,
	ReadCheckpoint,
	ReadActivatedCheckpoints,
	ReadExtraData,
	WaitingForPopup,
	CancelLevelLoad
};

enum class SavingState {
	Ready,
	Setup,
	SaveCheckpoint,
	SaveActivatedCheckpoints,
	SaveExtraData
};

union PSFData {
	struct {
		uint8_t m_originalVersion : 5;
		uint8_t m_platform : 2;
		uint8_t m_updatedFromPreviousLevelVersion : 1;
	};
	uint8_t data;
};

class $modify(PSPlayLayer, PlayLayer) {
public:
	struct Fields {
		bool m_onQuitCalled = false;
		bool m_signalForAsyncLoad = false;
		bool m_triedPlacingCheckpoint = false;
		bool m_inPostUpdate = false;
		bool m_inSetupHasCompleted = false;
		bool m_inResetLevel = false;
		bool m_inTogglePracticeMode = false;
		bool m_exitAfterSave = false;
		bool m_editorNoticeClosed = false;
		bool m_cancelLevelLoad = false;
		bool m_updateExtraData = false;
		bool m_startedLoadingObjects = false;
		uint8_t m_originalPSFVersion = 0;
		bool m_updatedFromPreviousLevelVersion = false;
		int m_saveSlot = -1;
		int m_readPSFVersion = -1;
		int m_loadedAttempts = 0;
		unsigned int m_remainingCheckpointLoadCount = 0;
		unsigned int m_remainingCheckpointSaveCount = 0;
		unsigned int m_bytesToRead = 0;
		unsigned int m_bytesRead = 0;
		float m_loadingProgress = 0.0f;
		util::platform::PSPlatform m_platform;
		util::platform::PSPlatform m_readPlatform;
		long long m_lastSavedCheckpointTimestamp = 0;
		persistenceAPI::Stream m_stream;
		persistenceAPI::Stream m_backupStream;
		LoadingState m_loadingState = LoadingState::Setup;
		SavingState m_savingState = SavingState::Ready;
		cocos2d::CCScene* m_transitionFadeScene = nullptr;
		geode::Ref<cocos2d::CCSprite> m_savingProgressCircleSprite = nullptr;
		geode::Ref<cocos2d::CCSprite> m_savingSuccessSprite = nullptr;
		geode::Ref<cocos2d::CCArray> m_normalModeCheckpoints = nullptr;
		std::vector<CheckpointGameObjectReference> m_activatedCheckpoints;
		gd::unordered_set<int> m_loadedPersistentTimerItemSet;
	};

	// overrides

	$override
	bool init(GJGameLevel* i_level, bool i_useReplay, bool i_dontCreateObjects);

	$override
	void processCreateObjectsFromSetup();

	$override 
	void createObjectsFromSetupFinished();

	$override
	void setupHasCompleted();

	$override
	void postUpdate(float i_unkFloat);

	$override
	CheckpointObject* markCheckpoint();
	
	$override
	void resetLevel();

	$override
	void prepareMusic(bool i_unkBool);

	$override
	void startMusic();

	$override
	void togglePracticeMode(bool i_value);

	$override
	void resetLevelFromStart();

	$override
	void onQuit();

	$override
	void updateVisibility(float i_unkFloat);
	
	// custom methods

	void registerCheckpointsAndActivatedCheckpoints();

	bool readPSFVersionAndUpdateIfNecessary();

	bool readPSFFinishedSaving();

	void readPSFData();

	bool readLowDetailMode();

	bool readPSFLevelStringHash();

	void showPlayLevelMenu();

	bool validSaveExists();
	
	std::string getSaveFilePath(int i_slot = -1, bool i_checkExists = false);

	void loadGame();

	void loadCheckpointFromStream();

	void loadActivatedCheckpointsFromStream();

	void updateAsyncProcessCreateObjectsFromSetup();

	void endAsyncProcessCreateObjectsFromSetup();

	void writePSFHeader();

	bool startSaveGame();

	void saveGame();

	void saveCheckpointToStream(unsigned int i_index);
	
	inline int getGameObjectIndex(GameObject* i_object) {
		int l_uniqueIDBase = reinterpret_cast<persistenceAPI::PAPlayLayer*>(this)->m_fields->m_uniqueIDBase;
		return i_object->m_uniqueID-l_uniqueIDBase;
	}

	void setupKeybinds();

	void setupSavingProgressCircleSprite();

	void showSavingProgressCircleSprite(bool i_show);

	void savingProgressCircleSpriteFadeOutEnd();

	void setupSavingSuccessSprite();

	void showSavingSuccessSprite();

	void endStream();

	bool canSave();

	bool savesEnabled();

	void removeSaveFile(int i_slot = -1);

	bool updatePSFFormat();

	bool makeBackup();
};