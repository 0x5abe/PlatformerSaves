#include "PlayLayer.hpp"
#include <filesystem>
#include <hooks/PauseLayer.hpp>
#include <util/algorithm.hpp>
#include <util/filesystem.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

bool PSPlayLayer::startSaveGame() {
	if (m_fields->m_savingState != SavingState::Ready || m_isPracticeMode) return false;
	m_fields->m_savingState = SavingState::Setup;
	CCScene* l_currentScene = CCScene::get();
	if (l_currentScene) {
		l_currentScene->runAction(
			CCSequence::create(
				CCDelayTime::create(0.0f),
				CCCallFunc::create(
					this,
					callfunc_selector(PSPlayLayer::saveGame)
				),
				nullptr
			)
		);
	}
	return true;
}

void PSPlayLayer::writePsfHeader() {
	m_fields->m_stream.write(s_psfMagicAndVer,sizeof(s_psfMagicAndVer));
	m_fields->m_stream.writeZero(16-sizeof(s_psfMagicAndVer));
	unsigned int l_levelStringHash = util::algorithm::hash_string(m_level->m_levelString.c_str());
	m_fields->m_stream << l_levelStringHash;
}

void PSPlayLayer::saveGame() {
	//log::info("SaveGame Gets run");
	switch (m_fields->m_savingState) {
		case SavingState::Setup: {
			//log::info("Goes into beginning");
			
			if (m_fields->m_normalModeCheckpoints->count() == 0) {
				m_fields->m_savingState = SavingState::Ready;
				break;
			}
			m_fields->m_remainingCheckpointSaveCount = 1;
		
			std::string l_filePath = getSaveFilePath(-1);
			std::string l_fileDirectory = util::filesystem::getParentDirectoryFromPath(l_filePath);
			if (!std::filesystem::exists(l_fileDirectory) && !std::filesystem::create_directories(l_fileDirectory)) {
				m_fields->m_savingState = SavingState::Ready;
				break;
			}

			int l_PAVersion = 2;
			if (m_fields->m_readPsfVersion > 0 && m_fields->m_readPsfVersion < 10) {
				l_PAVersion = 1;
			}
			if (!m_fields->m_stream.setFile(l_filePath, l_PAVersion, true)) {
				m_fields->m_savingState = SavingState::Ready;
				break;
			}

			showSavingProgressCircleSprite(true);
			
			writePsfHeader();
			
			m_fields->m_stream << m_fields->m_remainingCheckpointSaveCount;

			m_fields->m_savingState = SavingState::SaveCheckpoint;
			// falls through
		}
		case SavingState::SaveCheckpoint: {
			if (m_fields->m_remainingCheckpointSaveCount > 0) {
				saveCheckpointToStream(m_fields->m_normalModeCheckpoints->count()-m_fields->m_remainingCheckpointSaveCount);
				m_fields->m_remainingCheckpointSaveCount--;
				//log::info("Remaining save count: {}", m_fields->m_remainingCheckpointSaveCount);
				CCScene* l_currentScene = CCScene::get();
				if (l_currentScene) {
					l_currentScene->runAction(
						CCSequence::create(
							CCDelayTime::create(0.0f),
							CCCallFunc::create(
								this,
								callfunc_selector(PSPlayLayer::saveGame)
							),
							nullptr
						)
					);
				}
				break;
			}
			if (m_fields->m_remainingCheckpointSaveCount == 0) {
				m_fields->m_savingState = SavingState::SaveActivatedCheckpoints;
			}
			// TODO: use tasks maybe
			// falls through
		}
		case SavingState::SaveActivatedCheckpoints: {
			unsigned int l_size = m_fields->m_activatedCheckpoints.size();
			m_fields->m_stream.write(reinterpret_cast<char*>(&l_size), 4);
			for (int i = 0; i < m_fields->m_activatedCheckpoints.size(); i++) {
				m_fields->m_activatedCheckpoints[i].save(m_fields->m_stream);
			}
			m_fields->m_savingState = SavingState::SaveExtraData;
			// falls through
		}
		case SavingState::SaveExtraData: {
			m_fields->m_stream << m_effectManager->m_persistentItemCountMap;

			m_fields->m_stream << m_effectManager->m_persistentTimerItemSet;

			m_fields->m_stream << m_attempts;

			m_fields->m_savingState = SavingState::Ready;
			// falls through
		}
		case SavingState::Ready: {
			if (m_fields->m_normalModeCheckpoints->count() > 0) {
				m_fields->m_lastSavedCheckpointTimestamp = static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->lastObject())->m_fields->m_timestamp;
			}
			m_fields->m_stream.seek(sizeof(s_psfMagicAndVer));
			bool o_finishedSaving = true;
			m_fields->m_stream.write((char*)&o_finishedSaving,sizeof(bool));
			endStream();
			showSavingProgressCircleSprite(false);
			if (m_fields->m_exitAfterSave) {
				//log::info("Goes into exitAfterSave");
				m_fields->m_exitAfterSave = false;
				PauseLayer* l_pauseLayer = static_cast<PauseLayer*>(CCScene::get()->getChildByID("PauseLayer"));
				if (l_pauseLayer) {
					l_pauseLayer->onQuit(this);
				}
			}
			break;
		}
	}
}

void PSPlayLayer::saveCheckpointToStream(unsigned int i_index) {
	//log::info("Saving Startpoints to stream");
#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
	static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->objectAtIndex(i_index))->describe();
#endif
	static_cast<PSCheckpointObject*>(m_fields->m_normalModeCheckpoints->objectAtIndex(i_index))->save(m_fields->m_stream);
	//log::info("Saved Startpoints to stream");
}