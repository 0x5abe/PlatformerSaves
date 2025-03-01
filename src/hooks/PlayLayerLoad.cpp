#include "PlayLayer.hpp"
#include <filesystem>
#include <ui/PlayLevelMenuPopup.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>
#include <util/platform.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

bool PSPlayLayer::readPsfLevelStringHash() {
	unsigned int l_savedLevelStringHash;

	m_fields->m_stream >> l_savedLevelStringHash;
	
	if (l_savedLevelStringHash != util::algorithm::hash_string(m_level->m_levelString.c_str())) {
		//log::info("[readPsfLevelStringHash] different levelstring hash");
		return false;
	}
	//log::info("[readPsfLevelStringHash] same levelstring hash");
	return true;
}

bool PSPlayLayer::readPsfVersionAndUpdateIfNecessary() {
	std::string l_psfMagicAndVer(sizeof(s_psfMagicAndVer), ' ');

	m_fields->m_stream.read(l_psfMagicAndVer.data(), sizeof(s_psfMagicAndVer));
	l_psfMagicAndVer = l_psfMagicAndVer.substr(5, 5);
	l_psfMagicAndVer.erase(std::remove(l_psfMagicAndVer.begin(), l_psfMagicAndVer.end(), '.'), l_psfMagicAndVer.end());
	m_fields->m_readPsfVersion = std::stoi(l_psfMagicAndVer);
	//log::info("[readPsfVersionAndUpdateIfNecessary] Read PSF Version: {}", m_fields->m_readPsfVersion);
	if (s_psfVersion != m_fields->m_readPsfVersion) {
		if (!makeBackup()) {
			return false;
		}
		return updatePsfFormat();
	}
	m_fields->m_stream.setPAVersion(2);
	return true;
}

bool PSPlayLayer::readPsfFinishedSaving() {
	bool l_params[16-sizeof(s_psfMagicAndVer)];

	m_fields->m_stream.read(reinterpret_cast<char*>(l_params), 16-sizeof(s_psfMagicAndVer));

	if (l_params[0] == false) {
		return false;
	}

	return true;
}

void PSPlayLayer::loadGame() {
	switch (m_fields->m_loadingState) {
		case LoadingState::Setup: {
			m_fields->m_loadingProgress = 0.0f;

			PlayLevelMenuPopup* l_playLevelMenuPopup = PlayLevelMenuPopup::create();
			l_playLevelMenuPopup->setID("play-level-menu-popup"_spr);
			l_playLevelMenuPopup->show();
			m_fields->m_loadingState = LoadingState::WaitingForPlayLevelMenuPopup;
			// falls through
		}
		case LoadingState::WaitingForPlayLevelMenuPopup: {
			// TODO: fix this so it's not ugly
			if (m_fields->m_saveSlot == -1) {
				break;
			}
			else if (m_fields->m_saveSlot == -2) {
				m_fields->m_saveSlot = 0;
				m_fields->m_loadingState = LoadingState::CancelLevelLoad;
				break;
			}
			else if (m_fields->m_saveSlot == -3) {
				m_fields->m_saveSlot = 0;
				m_fields->m_loadingState = LoadingState::Ready;
				break;
			}
			else {
				m_fields->m_loadingState = LoadingState::SetupFileRead;
			}
			// falls through
		}
		case LoadingState::SetupFileRead: {
			std::string l_filePath = getSaveFilePath(-1, true);
			if (l_filePath == "") {
				m_fields->m_loadingState = LoadingState::Ready;
				m_fields->m_loadingProgress = 0.99f;
				break;
			}

			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_progressTimer->setColor(ccColor3B(220, 32, 64));
			}

			m_fields->m_bytesToRead = std::filesystem::file_size(l_filePath);
			m_fields->m_bytesRead = 0;
			if(m_fields->m_bytesToRead == 0 || !m_fields->m_stream.setFile(l_filePath, &m_fields->m_bytesRead, 2)) {
				m_fields->m_loadingState = LoadingState::HandleFileError;
				break;
			}

			m_fields->m_loadingState = LoadingState::ReadVersion;
			// falls through
		}
		case LoadingState::ReadVersion: {
			if (!readPsfVersionAndUpdateIfNecessary()) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectVersion;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadFinishedSaving;
			// falls through
		}
		case LoadingState::ReadFinishedSaving: {
			if (!readPsfFinishedSaving()) {
				m_fields->m_loadingState = LoadingState::HandleDidNotFinishSaving;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadHash;
			// falls through
		}
		case LoadingState::ReadHash: {
			if (!readPsfLevelStringHash()) {
				m_fields->m_loadingState = LoadingState::HandleIncorrectHash;
				break;
			}
			m_fields->m_loadingState = LoadingState::ReadCheckpointCount;
			// falls through
		}
		case LoadingState::ReadCheckpointCount: {
			m_fields->m_stream >> m_fields->m_remainingCheckpointLoadCount;
			m_fields->m_loadingState = LoadingState::ReadCheckpoint;
			// falls through
		}
		case LoadingState::ReadCheckpoint: {
			if (m_fields->m_remainingCheckpointLoadCount > 0) {
				loadCheckpointFromStream();
				m_fields->m_remainingCheckpointLoadCount--;
			}
			if (m_fields->m_remainingCheckpointLoadCount == 0) {
				m_fields->m_loadingState = LoadingState::ReadActivatedCheckpoints;
			}
			// falls through
		}
		case LoadingState::ReadActivatedCheckpoints: {
			loadActivatedCheckpointsFromStream();
			m_fields->m_loadingState = LoadingState::ReadExtraData;
			// falls through
		}
		case LoadingState::ReadExtraData: {
			m_fields->m_stream >> m_effectManager->m_persistentItemCountMap;

			m_fields->m_stream >> m_fields->m_loadedPersistentTimerItemSet;

			m_fields->m_stream >> m_fields->m_loadedAttempts;
			
			m_fields->m_updateExtraData = true;
			m_fields->m_loadingState = LoadingState::Ready;
			// falls through
		}
		case LoadingState::Ready: {
			endStream();
			if (m_fields->m_normalModeCheckpoints->count() > 0) {
				m_fields->m_lastSavedCheckpointTimestamp = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->lastObject())->m_fields->m_timestamp;
			}
			registerCheckpointsAndActivatedCheckpoints();
			break;
		}
		case LoadingState::HandleFileError: {
			util::platform::hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The save file for this level <cr>could not be opened</c>.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					util::platform::hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectVersion: {
			util::platform::hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"Updating the save file <cr>failed</c>.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					util::platform::hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleDidNotFinishSaving: {
			util::platform::hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			createQuickPopup("Error loading game",
				"The save file for this level appears to be <cr>corrupted</c>.",
				"Ok",
				nullptr,
				[&](FLAlertLayer*, bool i_btn2) {
					m_fields->m_loadingState = LoadingState::CancelLevelLoad;
					util::platform::hideAndLockCursor(true);
				}
			);
			break;
		}
		case LoadingState::HandleIncorrectHash: {
			util::platform::hideAndLockCursor(false);
			m_fields->m_loadingState = LoadingState::WaitingForPopup;
			if (m_level->m_levelType == GJLevelType::Editor) {
				createQuickPopup("Error loading game",
					"The version of the level in the save file does not match the current one. <cy>A new game will be started</c>.",
					"Cancel",
					"Ok",
					[&](FLAlertLayer*, bool i_btn2) {
						if (i_btn2) {
							removeSaveFile();
							m_fields->m_loadingState = LoadingState::Ready;
						} else {
							m_fields->m_loadingState = LoadingState::CancelLevelLoad;
						}
						util::platform::hideAndLockCursor(true);
					}
				);
			} else {
				createQuickPopup("Error loading game",
					"The version of the level in the save file does not match the current one. <cy>Try to load it anyways</c>? (<cr>this might be unstable or crash the game</c>).",
					"Cancel",
					"Ok",
					[&](FLAlertLayer*, bool i_btn2) {
						if (i_btn2) {
							m_fields->m_loadingState = LoadingState::ReadCheckpointCount;
						} else {
							m_fields->m_loadingState = LoadingState::CancelLevelLoad;
						}
						util::platform::hideAndLockCursor(true);
					}
				);
			}
			break;
		}
		case LoadingState::WaitingForPopup: {
			break;
		}
		case LoadingState::CancelLevelLoad: {
			m_fields->m_cancelLevelLoad = true;
			LevelInfoLayer* l_levelInfoLayer = static_cast<LevelInfoLayer*>(CCScene::get()->getChildByID("LevelInfoLayer"));
			if (l_levelInfoLayer) {
				l_levelInfoLayer->m_progressTimer->setVisible(false);
			}
			m_fields->m_loadingState = LoadingState::Ready;
			endStream();
			break;
		}
	}
}

void PSPlayLayer::loadActivatedCheckpointsFromStream() {
	unsigned int l_size;
	m_fields->m_stream.read(reinterpret_cast<char*>(&l_size), 4);
	if (l_size != 0) {
		m_fields->m_activatedCheckpoints.resize(l_size);
		for (int i = 0; i < l_size; i++) {
			m_fields->m_activatedCheckpoints[i].load(m_fields->m_stream);
		}
	}
}

void PSPlayLayer::loadCheckpointFromStream() {
	PSCheckpointObject* l_checkpoint = reinterpret_cast<PSCheckpointObject*>(CheckpointObject::create());
	l_checkpoint->load(m_fields->m_stream); 

	GameObject* l_newPhysicalCPO = GameObject::createWithFrame("square_01_001.png");
	CC_SAFE_RETAIN(l_newPhysicalCPO);
	l_newPhysicalCPO->m_objectID = 0x2c;
	l_newPhysicalCPO->m_objectType = GameObjectType::Decoration;
	l_newPhysicalCPO->m_glowSprite = nullptr;
	l_newPhysicalCPO->m_isDisabled2 = true; // who knows
	l_newPhysicalCPO->m_isInvisible = true; // who knows
	l_newPhysicalCPO->setOpacity(0);

	// This never seemed to matter
	//int* l_unkField1 = reinterpret_cast<int*>(reinterpret_cast<size_t>(l_newPhysicalCPO)+0x3d4);
	//*l_unkField1 = 3;

	CC_SAFE_RELEASE(l_checkpoint->m_physicalCheckpointObject);
	l_checkpoint->m_physicalCheckpointObject = l_newPhysicalCPO;
	
	l_checkpoint->m_physicalCheckpointObject->setStartPos(l_checkpoint->m_fields->m_position);

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
	l_checkpoint->describe();
#endif

	m_fields->m_normalModeCheckpoints->addObject(l_checkpoint);
}

void PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup() {
	cocos2d::SEL_CallFunc l_sel = callfunc_selector(PSPlayLayer::updateAsyncProcessCreateObjectsFromSetup);
	PSPlayLayer::processCreateObjectsFromSetup();
	if (m_loadingProgress >= 1.0) {
		l_sel = callfunc_selector(PSPlayLayer::endAsyncProcessCreateObjectsFromSetup);
	}
	 
	CCScene* l_currentScene = CCScene::get();
	if (l_currentScene) {
		l_currentScene->runAction(
			CCSequence::create(
				CCDelayTime::create(0.0f),
				CCCallFunc::create(
					this,
					l_sel
				),
				nullptr
			)
		);
	}
}

void PSPlayLayer::endAsyncProcessCreateObjectsFromSetup() {
	if (m_fields->m_transitionFadeScene) {
		CCDirector::get()->replaceScene(m_fields->m_transitionFadeScene);
		CC_SAFE_RELEASE(m_fields->m_transitionFadeScene);
		m_fields->m_transitionFadeScene = nullptr;
	}
}