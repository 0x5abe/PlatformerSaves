#pragma once
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <domain/CheckpointGameObjectReference.hpp>
#include <hooks/CheckpointObject.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class PSPlayLayer;

#define PSF_EXT ".psf"
extern PSPlayLayer* s_currentPlayLayer;
extern char s_psfMagicAndVer[11];

enum class LoadingState {
	Ready,
	Setup,
	WaitingForPlayLevelMenuPopup,
	SetupFileRead,
	HandleFileError,
	ReadVersion,
	HandleIncorrectVersion,
	UpdateVersion,
	ReadFinishedSaving,
	HandleDidNotFinishSaving,
	ReadHash,
	HandleIncorrectHash,
	ReadCheckpointCount,
	ReadCheckpoint,
	ReadTriggeredCheckpointGameObjects,
	ReadTimePlayed,
	WaitingForPopup,
	CancelLevelLoad
};

enum class SavingState {
	Ready,
	Setup,
	SaveCheckpoint,
	SaveTriggeredCheckpointGameObjects,
	SaveTimePlayed
};

class $modify(PSPlayLayer, PlayLayer) {
public:
	struct Fields {
		persistenceAPI::InputStream m_inputStream;
		persistenceAPI::OutputStream m_outputStream;
		int m_saveSlot = -1;
		int m_uniqueIdBase = 12;
		bool m_onQuitCalled = false;
		LoadingState m_loadingState = LoadingState::Setup;
		float m_loadingProgress = 0.0f;
		unsigned int m_remainingCheckpointLoadCount = 0;
		unsigned int m_remainingCheckpointSaveCount = 0;
		SavingState m_savingState = SavingState::Ready;
		unsigned int m_bytesToRead = 0;
		unsigned int m_bytesRead = 0;
		bool m_signalForAsyncLoad = false;
		cocos2d::CCScene* m_transitionFadeScene = nullptr;
		bool m_cancelLevelLoad = false;
		geode::Ref<cocos2d::CCSprite> m_savingIcon = nullptr;
		geode::Ref<cocos2d::CCArray> m_normalModeCheckpoints = nullptr;
		std::vector<CheckpointGameObjectReference> m_triggeredCheckpointGameObjects;
		bool m_triedPlacingCheckpoint = false;
		bool m_inPostUpdate = false;
		bool m_inSetupHasCompleted = false;
		bool m_inResetLevel = false;
		bool m_inTogglePracticeMode = false;
		long long m_lastSavedCheckpointTimestamp = 0;
	};

	// overrides

	$override
	bool init(GJGameLevel* i_level, bool i_useReplay, bool i_dontCreateObjects);

	$override 
	void createObjectsFromSetupFinished();

	$override
	void setupHasCompleted();

	$override
	void onQuit();

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
	
	// custom methods

	void registerCheckpointsAndTriggeredCheckpointGameObjects();

	bool readPsfLevelStringHash();

	bool readPsfVersion();

	bool readPsfFinishedSaving();

	void showPlayLevelMenu();

	bool validSaveExists();
	
	std::string getSaveFilePath(bool i_checkExists = false, int i_slot = -1);

	void loadGame();

	void loadCheckpointFromStream();

	void loadTriggeredCheckpointGameObjectsFromStream();

	void updateAsyncProcessCreateObjectsFromSetup();

	void endAsyncProcessCreateObjectsFromSetup();

	void writePsfHeader();

	bool startSaveGame();

	void saveGame();

	void saveCheckpointToStream(unsigned int i_index);
	
	inline int getGameObjectIndex(GameObject* i_object) {
		return i_object->m_uniqueID-m_fields->m_uniqueIdBase;
	}

	void setupKeybinds();

	void setupSavingIcon();

	void showSavingIcon(bool i_show);

	void endOutputStream();

	bool canSave();
};