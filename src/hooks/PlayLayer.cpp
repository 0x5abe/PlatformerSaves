#include "PlayLayer.hpp"
#include "CheckpointObject.hpp"
#include "Geode/binding/PlayLayer.hpp"
#include "domain/CheckpointGameObjectReference.hpp"
#include "hooks/PauseLayer.hpp"
#include "hooks/FMODAudioEngine.hpp"
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

PSPlayLayer* s_currentPlayLayer = nullptr;
char s_psfMagicAndVer[] = "PSF v0.0.9";
int s_psfVersion = 9;

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
	setupSavingProgressCircleSprite();

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
				"Saving the game is <cr>disabled</c> by default for editor levels since it can be <cr>unstable</c>. You can change this behavior in the PlatformerSaves <cy>mod settings</c> page.",
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

		//log::info("[setupHasCompleted] m_bytesRead: {}", m_fields->m_bytesRead);
		//log::info("[setupHasCompleted] m_bytesToRead: {}", m_fields->m_bytesToRead);
		if (m_fields->m_bytesToRead > 0) {
			m_fields->m_loadingProgress = (static_cast<float>(m_fields->m_bytesRead)/static_cast<float>(m_fields->m_bytesToRead));
		}

		m_loadingProgress = m_fields->m_loadingProgress;
		//log::info("[setupHasCompleted] m_loadingProgress: {}", m_loadingProgress);
	}
	if (m_fields->m_loadingState == LoadingState::Ready && !m_fields->m_cancelLevelLoad) {
		m_loadingProgress = 1.0f;

		// now reset the order of arrival that we're actually going into the level
		CCNode::resetGlobalOrderOfArrival();
		
		// restore loadMusic
		PSFMODAudioEngine* l_audioEngine = static_cast<PSFMODAudioEngine*>(FMODAudioEngine::get());
		l_audioEngine->m_fields->m_disableLoadMusic = false;

		m_fields->m_inSetupHasCompleted = true;
		PlayLayer::setupHasCompleted();
		m_fields->m_inSetupHasCompleted = false;

		if (m_fields->m_normalModeCheckpoints->count() > 0) {
			PSCheckpointObject* l_checkpoint = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->lastObject());
			m_player1->setStartPos(l_checkpoint->m_fields->m_position);
		}

		m_fields->m_loadingProgress = 0.0f;
		m_fields->m_bytesToRead = 0;
		m_fields->m_bytesRead = 0;
		s_currentPlayLayer = nullptr;
	}
}

void PSPlayLayer::postUpdate(float i_unkFloat) {
	m_fields->m_inPostUpdate = true;
	m_fields->m_triedPlacingCheckpoint = m_tryPlaceCheckpoint;

	PlayLayer::postUpdate(i_unkFloat);

	if (m_fields->m_updateExtraData) {
		m_fields->m_updateExtraData = false;
		m_effectManager->m_persistentTimerItemSet = m_fields->m_loadedPersistentTimerItemSet;
		m_attempts = m_fields->m_loadedAttempts;
	}
	
	m_fields->m_inPostUpdate = false;
	m_fields->m_triedPlacingCheckpoint = false;
}

CheckpointObject* PSPlayLayer::markCheckpoint() {
	PSCheckpointObject* l_checkpointObject = static_cast<PSCheckpointObject*>(PlayLayer::markCheckpoint());
	
	if (l_checkpointObject && savesEnabled() && m_fields->m_inPostUpdate && !m_isPracticeMode) {
		if (m_fields->m_triedPlacingCheckpoint) {
			m_fields->m_triedPlacingCheckpoint = false;
		} else if (m_activatedCheckpoint != nullptr) {
			l_checkpointObject->m_fields->m_timePlayed = m_timePlayed;
			l_checkpointObject->m_fields->m_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			m_fields->m_normalModeCheckpoints->addObject(l_checkpointObject);
			m_fields->m_activatedCheckpoints.push_back(CheckpointGameObjectReference(m_activatedCheckpoint));
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
		registerCheckpointsAndActivatedCheckpoints();
		PlayLayer::resetLevel();
		return;
	}
	PlayLayer::resetLevelFromStart();
}

void PSPlayLayer::onQuit() {
	s_currentPlayLayer = nullptr;
	PlayLayer::onQuit();
}

void PSPlayLayer::updateVisibility(float i_unkFloat) {
	PlayLayer::updateVisibility(i_unkFloat);

	for (int i = 0; i < m_fields->m_normalModeCheckpoints->count(); i++) {
		static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->objectAtIndex(i))->m_physicalCheckpointObject->setOpacity(0);
	}
}

// custom methods

void PSPlayLayer::registerCheckpointsAndActivatedCheckpoints() {
	PSCheckpointObject* l_checkpoint;
	for (int i = 0; i < m_fields->m_normalModeCheckpoints->count(); i++) {
		l_checkpoint = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->objectAtIndex(i));
		m_checkpointArray->addObject(l_checkpoint);
		PlayLayer::addToSection(l_checkpoint->m_physicalCheckpointObject);
		l_checkpoint->m_physicalCheckpointObject->activateObject();
		m_timePlayed = l_checkpoint->m_fields->m_timePlayed;
	}
	for (int i = 0; i < m_fields->m_activatedCheckpoints.size(); i++) {
		if (m_fields->m_activatedCheckpoints[i].m_reference != nullptr) m_fields->m_activatedCheckpoints[i].m_reference->triggerActivated(0.0f);
	}
}

std::string PSPlayLayer::getSaveFilePath(int i_slot, bool i_checkExists) {
	if (i_slot == -1) {
		i_slot = m_fields->m_saveSlot;
	}
	return util::filesystem::getSaveFilePath(m_level, i_slot, i_checkExists);
}

bool PSPlayLayer::validSaveExists() {
	return util::filesystem::validSaveExists(m_level);
}

void PSPlayLayer::setupKeybinds() {
	addEventListener<keybinds::InvokeBindFilter>(
		[this](keybinds::InvokeBindEvent* event) {
			if (event->isDown() && canSave() && startSaveGame()) {
				PSPauseLayer* l_pauseLayer = static_cast<PSPauseLayer*>(CCScene::get()->getChildByID("PauseLayer"));
				if (l_pauseLayer) {
					if (l_pauseLayer->m_fields->m_saveCheckpointsSprite != nullptr) l_pauseLayer->m_fields->m_saveCheckpointsSprite->setColor({127,127,127});
					if (l_pauseLayer->m_fields->m_saveCheckpointsSprite != nullptr && l_pauseLayer->m_fields->m_saveCheckpointsSprite->getChildren()->count() > 0) static_cast<CCSprite*>(l_pauseLayer->m_fields->m_saveCheckpointsSprite->getChildren()->objectAtIndex(0))->setColor({127,127,127});
					if (l_pauseLayer->m_fields->m_saveCheckpointsButton != nullptr) l_pauseLayer->m_fields->m_saveCheckpointsButton->m_bEnabled = false;
				}
			}
			return ListenerResult::Propagate;
		},
		"save-game"_spr
	);
}

void PSPlayLayer::setupSavingProgressCircleSprite() {
	CCSize l_winSize = CCDirector::sharedDirector()->getWinSize();
	float l_separation = l_winSize.height/10;

	m_fields->m_savingProgressCircleSprite = CCSprite::create("loadingCircle.png");
	m_fields->m_savingProgressCircleSprite->setBlendFunc({ GL_ONE, GL_ONE });
	m_fields->m_savingProgressCircleSprite->setPosition({l_winSize.width-l_separation,l_separation});
	m_fields->m_savingProgressCircleSprite->setZOrder(100);
	m_fields->m_savingProgressCircleSprite->setScale(0.5f);
	m_fields->m_savingProgressCircleSprite->setID("saving-progress-circle-sprite"_spr);

	m_fields->m_savingProgressCircleSprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.f)));
	m_fields->m_savingProgressCircleSprite->pauseSchedulerAndActions();
}

void PSPlayLayer::showSavingProgressCircleSprite(bool i_show) {
	CCScene* l_currentScene = CCScene::get();
	if (!l_currentScene || !m_fields->m_savingProgressCircleSprite) return;
	if (i_show) {
		if (!l_currentScene->getChildren()->containsObject(m_fields->m_savingProgressCircleSprite)) {
			l_currentScene->addChild(m_fields->m_savingProgressCircleSprite);
		}
		m_fields->m_savingProgressCircleSprite->resumeSchedulerAndActions();
		m_fields->m_savingProgressCircleSprite->setVisible(true);
		return;	
	}
	m_fields->m_savingProgressCircleSprite->pauseSchedulerAndActions();
	m_fields->m_savingProgressCircleSprite->setVisible(false);
}

void PSPlayLayer::endStream() {
	m_fields->m_stream.end();
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
	//log::info("savesEnabled: {}", Mod::get()->getSettingValue<bool>("editor-saves") || m_level->m_levelType != GJLevelType::Editor);
	return Mod::get()->getSettingValue<bool>("editor-saves") || m_level->m_levelType != GJLevelType::Editor;
}

void PSPlayLayer::removeSaveFile(int i_slot) {
	if (i_slot == -1) {
		i_slot = m_fields->m_saveSlot;
	}
	endStream();
	util::filesystem::removeSaveFile(m_level, i_slot);
}

bool PSPlayLayer::updatePsfFormat() {
	switch (m_fields->m_readPsfVersion) {
		case 7: {
			m_fields->m_stream.seek(0);
			m_fields->m_stream.write(s_psfMagicAndVer,sizeof(s_psfMagicAndVer));
			m_fields->m_stream.seek(0, true);
			m_fields->m_stream.clear();
			m_fields->m_stream.writeZero(3*sizeof(int));
			m_fields->m_stream.seek(sizeof(s_psfMagicAndVer));
			m_fields->m_readPsfVersion = s_psfVersion;
			return true;
		}
		case 8: {
			m_fields->m_stream.seek(0);
			m_fields->m_stream.write(s_psfMagicAndVer,sizeof(s_psfMagicAndVer));
			m_fields->m_stream.seek(0, true);
			m_fields->m_stream.clear();
			m_fields->m_stream.writeZero(sizeof(int));
			m_fields->m_stream.seek(sizeof(s_psfMagicAndVer));
			m_fields->m_readPsfVersion = s_psfVersion;
			return true;
		}
		default: {
			m_fields->m_stream.seek(sizeof(s_psfMagicAndVer));
			return false;
		}
	}
}

bool PSPlayLayer::makeBackup() {
	std::string l_filePath = getSaveFilePath(-1, true);
	if (l_filePath == "") {
		return false;
	}

	l_filePath.append(std::format(".{}.bak",m_fields->m_readPsfVersion));
	//log::info("Backup file path: {}", l_filePath);
	if (!m_fields->m_backupStream.setFile(l_filePath, true)) {
		return false;
	}

	std::vector<char> buf(m_fields->m_bytesToRead);
	m_fields->m_stream.seek(0);
	m_fields->m_stream.read(buf.data(), m_fields->m_bytesToRead);
	m_fields->m_backupStream.write(buf.data(), m_fields->m_bytesToRead);
	m_fields->m_backupStream.end();
	m_fields->m_stream.seek(sizeof(s_psfMagicAndVer));

	return true;
}