#pragma once
#include "Geode/modify/Modify.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <domain/CheckpointGameObjectReference.hpp>
#include <hooks/CheckpointObject.hpp>
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class PSPlayLayer;

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
	ReadActivatedCheckpoints,
	ReadTimePlayed,
	WaitingForPopup,
	CancelLevelLoad
};

enum class SavingState {
	Ready,
	Setup,
	SaveCheckpoint,
	SaveActivatedCheckpoints,
	SaveTimePlayed
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
		int m_saveSlot = -1;
		int m_uniqueIdBase = 12;
		unsigned int m_remainingCheckpointLoadCount = 0;
		unsigned int m_remainingCheckpointSaveCount = 0;
		unsigned int m_bytesToRead = 0;
		unsigned int m_bytesRead = 0;
		float m_loadingProgress = 0.0f;
		long long m_lastSavedCheckpointTimestamp = 0;
		persistenceAPI::InputStream m_inputStream;
		persistenceAPI::OutputStream m_outputStream;
		LoadingState m_loadingState = LoadingState::Setup;
		SavingState m_savingState = SavingState::Ready;
		cocos2d::CCScene* m_transitionFadeScene = nullptr;
		geode::Ref<cocos2d::CCSprite> m_savingProgressCircleSprite = nullptr;
		geode::Ref<cocos2d::CCArray> m_normalModeCheckpoints = nullptr;
		std::vector<CheckpointGameObjectReference> m_activatedCheckpoints;
	};

	// overrides

	$override
	bool init(GJGameLevel* i_level, bool i_useReplay, bool i_dontCreateObjects);

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
	
	// custom methods

	void registerCheckpointsAndActivatedCheckpoints();

	bool readPsfLevelStringHash();

	bool readPsfVersion();

	bool readPsfFinishedSaving();

	void showPlayLevelMenu();

	bool validSaveExists();
	
	std::string getSaveFilePath(int i_slot = -1, bool i_checkExists = false);

	void loadGame();

	void loadCheckpointFromStream();

	void loadActivatedCheckpointsFromStream();

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

	void setupSavingProgressCircleSprite();

	void showSavingProgressCircleSprite(bool i_show);

	void endOutputStream();

	void endInputStream();

	bool canSave();

	bool savesEnabled();

	void removeSaveFile(int i_slot = -1);
};