#include "PlayLayer.hpp"
#include "CheckpointObject.hpp"
#include "Geode/binding/PlayLayer.hpp"
#include "domain/CheckpointGameObjectReference.hpp"
#include <filesystem>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

PSPlayLayer* s_currentPlayLayer = nullptr;
char s_psfMagicAndVer[] = "PSF v0.0.7";

// overrides

bool PSPlayLayer::init(GJGameLevel* i_level, bool i_useReplay, bool i_dontCreateObjects) {
	// for processing objects asynchronously every time
	s_currentPlayLayer = this;
	m_fields->m_signalForAsyncLoad = !i_dontCreateObjects;
	if (m_fields->m_signalForAsyncLoad) {
		m_loadingProgress = 1.0f;
	}
	m_fields->m_normalModeCheckpoints = cocos2d::CCArray::create();

	if (!PlayLayer::init(i_level, i_useReplay, i_dontCreateObjects)) return false;

	// for processing objects asynchronously every time
	if (m_fields->m_signalForAsyncLoad) {
		m_loadingProgress = 0.0f;
	}
	setupKeybinds();
	setupSavingIcon();

	return true;
}

void PSPlayLayer::createObjectsFromSetupFinished() {
	if (m_fields->m_loadingState == LoadingState::Setup) {
		PlayLayer::createObjectsFromSetupFinished();
	}
}

void PSPlayLayer::setupHasCompleted() {
	//log::info("[setupHasCompleted] begin");
	if (!m_isPlatformer) {
		m_fields->m_loadingState = LoadingState::Ready;
	}
	else if (!savesEnabled() && m_fields->m_loadingState != LoadingState::WaitingForPopup) {
		if (!Mod::get()->getSavedValue<bool>("has-seen-editor-notice")) {
			CCEGLView::get()->showCursor(true);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Editor Level Saves",
				"Saving the game is <cr>disabled</c> by default for editor levels since it can be <cr>unstable</c>. You can change this behavior in the mod settings page.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					Mod::get()->setSavedValue<bool>("has-seen-editor-notice", true);
					m_fields->m_loadingState = LoadingState::Ready;
					CCEGLView::get()->showCursor(false);
					bool l_lockCursor = GameManager::get()->getGameVariable("0128");
					if (l_lockCursor) {
						CCEGLView::get()->toggleLockCursor(true);
					}
				}
			);
		} else {
			m_fields->m_loadingState = LoadingState::Ready;
		}
	}
	if (m_fields->m_loadingState != LoadingState::Ready) {
		//log::info("[setupHasCompleted] hasnt finished loading checkpoints");

		loadGame();
		
		//m_fields->m_loadingProgress += (m_fields->m_loadingProgress/8.0f);
		//log::info("[setupHasCompleted] m_bytesRead: {}", m_fields->m_bytesRead);
		//log::info("[setupHasCompleted] m_bytesToRead: {}", m_fields->m_bytesToRead);
		if (m_fields->m_bytesToRead > 0) {
			m_fields->m_loadingProgress = (static_cast<float>(m_fields->m_bytesRead)/static_cast<float>(m_fields->m_bytesToRead));
		}
		// if (m_fields->m_loadingProgress != 1.0f) {
		m_loadingProgress = m_fields->m_loadingProgress;
		// } else {
		// 	m_loadingProgress = 0.99f;
		// }
		//log::info("[setupHasCompleted] m_loadingProgress: {}", m_loadingProgress);
	}
	if (m_fields->m_loadingState == LoadingState::Ready && !m_fields->m_cancelLevelLoad) {
		m_loadingProgress = 1.0f;

		// now reset the order of arrival that we're actually going into the level
		CCNode::resetGlobalOrderOfArrival();

		m_fields->m_inSetupHasCompleted = true;
		PlayLayer::setupHasCompleted();
		m_fields->m_inSetupHasCompleted = false;

		m_fields->m_loadingProgress = 0.0f;
		m_fields->m_bytesToRead = 0;
		m_fields->m_bytesRead = 0;
	}
}

void PSPlayLayer::postUpdate(float i_unkFloat) {
	m_fields->m_inPostUpdate = true;
	m_fields->m_triedPlacingCheckpoint = m_tryPlaceCheckpoint;

	PlayLayer::postUpdate(i_unkFloat);
	
	m_fields->m_inPostUpdate = false;
	m_fields->m_triedPlacingCheckpoint = false;
}

CheckpointObject* PSPlayLayer::markCheckpoint() {
	PSCheckpointObject* l_checkpointObject = static_cast<PSCheckpointObject*>(PlayLayer::markCheckpoint());
	
	if (savesEnabled() && m_fields->m_inPostUpdate && !m_isPracticeMode) {
		if (m_fields->m_triedPlacingCheckpoint) {
			m_fields->m_triedPlacingCheckpoint = false;
		} else if (m_triggeredCheckpointGameObject != nullptr) {
			l_checkpointObject->m_fields->m_timePlayed = m_timePlayed;
			l_checkpointObject->m_fields->m_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			m_fields->m_normalModeCheckpoints->addObject(l_checkpointObject);
			m_fields->m_triggeredCheckpointGameObjects.push_back(CheckpointGameObjectReference(m_triggeredCheckpointGameObject));
			// autosave
			if (Mod::get()->getSettingValue<bool>("auto-save")) startSaveGame();
		}
	}

	return l_checkpointObject;
}

void PSPlayLayer::resetLevel() {
	m_fields->m_inResetLevel = true;
	PlayLayer::resetLevel();
	if (savesEnabled() && m_isPlatformer) {
		toggleMGVisibility(true); // it's actually toggleHideAttempts but virtuals are off for some reason IF IT BREAKS CHANGE IT BACK
	}
	m_fields->m_inResetLevel = false;
}

void PSPlayLayer::prepareMusic(bool i_unkBool) {
	if (savesEnabled() && m_fields->m_inSetupHasCompleted && m_isPlatformer && m_fields->m_normalModeCheckpoints->count() > 0 && !m_fields->m_inResetLevel) {
		return;
	}
	PlayLayer::prepareMusic(i_unkBool);
}

void PSPlayLayer::startMusic() {
	if (savesEnabled() && m_isPlatformer && m_fields->m_normalModeCheckpoints->count() > 0) {
		return;
	}
	PlayLayer::startMusic();
}

void PSPlayLayer::togglePracticeMode(bool i_value) {
	m_fields->m_inTogglePracticeMode = true;
	PlayLayer::togglePracticeMode(i_value);
	m_fields->m_inTogglePracticeMode = false;
}

void PSPlayLayer::resetLevelFromStart() {
	if (savesEnabled() && m_fields->m_inTogglePracticeMode && m_isPlatformer && !m_isPracticeMode && m_fields->m_normalModeCheckpoints->count() > 0) {
		PlayLayer::removeAllCheckpoints();
		registerCheckpointsAndTriggeredCheckpointGameObjects();
		PlayLayer::resetLevel();
		return;
	}
	PlayLayer::resetLevelFromStart();
}

void PSPlayLayer::onQuit() {
	s_currentPlayLayer = nullptr;
	PlayLayer::onQuit();
}

// custom methods

void PSPlayLayer::registerCheckpointsAndTriggeredCheckpointGameObjects() {
	PSCheckpointObject* l_checkpoint;
	for (int i = 0; i < m_fields->m_normalModeCheckpoints->count(); i++) {
		l_checkpoint = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->objectAtIndex(i));
		m_checkpointArray->addObject(l_checkpoint);
		PlayLayer::addToSection(l_checkpoint->m_physicalCheckpointObject);
		l_checkpoint->m_physicalCheckpointObject->activateObject();
		m_timePlayed = l_checkpoint->m_fields->m_timePlayed;
	}
	for (int i = 0; i < m_fields->m_triggeredCheckpointGameObjects.size(); i++) {
		if (m_fields->m_triggeredCheckpointGameObjects[i].m_reference != nullptr) m_fields->m_triggeredCheckpointGameObjects[i].m_reference->triggerActivated(0.0f);
	}
}

std::string PSPlayLayer::getSaveFilePath(bool i_checkExists, int i_slot) {
	if (i_slot == -1) {
		i_slot = m_fields->m_saveSlot;
	}
	std::string l_filePath = Mod::get()->getSaveDir().generic_string();
	std::string l_cleanLevelName = m_level->m_levelName;
	l_cleanLevelName.erase(std::remove(l_cleanLevelName.begin(), l_cleanLevelName.end(), '.'), l_cleanLevelName.end());
	l_cleanLevelName.erase(std::remove(l_cleanLevelName.begin(), l_cleanLevelName.end(), '/'), l_cleanLevelName.end());
	l_cleanLevelName.erase(std::remove(l_cleanLevelName.begin(), l_cleanLevelName.end(), '\\'), l_cleanLevelName.end());

	switch(m_level->m_levelType) {
		case GJLevelType::Local:
			l_filePath.append(std::format("/saves/local/{}/slot{}{}", m_level->m_levelID.value(), i_slot, PSF_EXT));
			break;
		case GJLevelType::Editor:
			l_filePath.append(std::format("/saves/editor/{}_rev{}/slot{}{}", l_cleanLevelName.c_str(), m_level->m_levelRev, i_slot, PSF_EXT));
			break;
		case GJLevelType::Saved:
		default:
			l_filePath.append(std::format("/saves/online/{}/slot{}{}", m_level->m_levelID.value(), i_slot, PSF_EXT));
			break;
	}
	//log::info("Filepath: \"{}\"", l_filePath);

	if (i_checkExists && !std::filesystem::exists(l_filePath)) {
		//log::info("File doesnt exist: {}", l_filePath);
		return "";
	}
	return l_filePath;
}

bool PSPlayLayer::validSaveExists() {
	std::string l_filePath;
	for (int i = 0; i < 4; i++) {
		l_filePath = getSaveFilePath(true, i);
		if (l_filePath != "") {
			return true;
		}
	}
	return false;
}

void PSPlayLayer::setupKeybinds() {
	// TodoRemove
	addEventListener<keybinds::InvokeBindFilter>(
		[this](keybinds::InvokeBindEvent* event) {
			if (event->isDown()) {
				startSaveGame();
			}
			return ListenerResult::Propagate;
		},
		"test-key-2"_spr
	);
	// EndTodo
}

void PSPlayLayer::setupSavingIcon() {
	CCSize l_winSize = CCDirector::sharedDirector()->getWinSize();
	float l_separation = l_winSize.height/10;

	m_fields->m_savingIcon = CCSprite::create("loadingCircle.png");
	m_fields->m_savingIcon->setBlendFunc({ GL_ONE, GL_ONE });
	m_fields->m_savingIcon->setPosition({l_winSize.width-l_separation,l_separation});
	m_fields->m_savingIcon->setZOrder(100);
	m_fields->m_savingIcon->setScale(0.5f);

	m_fields->m_savingIcon->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.f)));
	m_fields->m_savingIcon->pauseSchedulerAndActions();
	//m_fields->m_savingIcon->setVisible(false);
}

void PSPlayLayer::showSavingIcon(bool i_show) {
	CCScene* l_currentScene = CCScene::get();
	if (!l_currentScene || !m_fields->m_savingIcon) return;
	if (i_show) {
		if (!l_currentScene->getChildren()->containsObject(m_fields->m_savingIcon)) {
			l_currentScene->addChild(m_fields->m_savingIcon);
		}
		m_fields->m_savingIcon->resumeSchedulerAndActions();
		m_fields->m_savingIcon->setVisible(true);
		return;	
	}
	m_fields->m_savingIcon->pauseSchedulerAndActions();
	m_fields->m_savingIcon->setVisible(false);
}

bool PSPlayLayer::canSave() {
	if (!savesEnabled()) {
		return false;
	}
	if (m_fields->m_normalModeCheckpoints->count() > 0) {
		PSCheckpointObject* l_lastCheckpoint = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->lastObject());
		return l_lastCheckpoint->m_fields->m_timestamp > m_fields->m_lastSavedCheckpointTimestamp;
	}
	return false;
}

bool PSPlayLayer::savesEnabled() {
	log::info("savesEnabled: {}", Mod::get()->getSettingValue<bool>("editor-saves") || m_level->m_levelType != GJLevelType::Editor);
	return Mod::get()->getSettingValue<bool>("editor-saves") || m_level->m_levelType != GJLevelType::Editor;
}

void PSPlayLayer::removeSaveFile() {
	endInputStream();
	endOutputStream();
	std::string l_filePath = getSaveFilePath(true);
	if (l_filePath != "") {
		std::filesystem::remove(l_filePath);
	}
}