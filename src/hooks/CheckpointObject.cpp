#include "CheckpointObject.hpp"
#include <Geode/binding/SavedActiveObjectState.hpp>
#include <Geode/binding/CheckpointObject.hpp>
#include <Geode/binding/SavedObjectStateRef.hpp>
#include <Geode/binding/GradientTriggerObject.hpp>
#include <Geode/binding/PlayerCheckpoint.hpp>
#include <Geode/binding/SequenceTriggerState.hpp>
#include <Geode/cocos/cocoa/CCArray.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <hooks/PlayLayer.hpp>
#include <util/debug.hpp>

using namespace geode::prelude;
using namespace persistenceAPI;

void PSCheckpointObject::load(Stream& i_stream) {
	reinterpret_cast<PACCNode*>(this)->load(i_stream);
	i_stream >> *this;
}

void PSCheckpointObject::save(Stream& o_stream) {
	//log::info("SAVING CHECKPOINT OBJECT");
	reinterpret_cast<PACCNode*>(this)->save(o_stream);
	o_stream << *this;
}

inline void operator>>(Stream& i_stream, PSCheckpointObject& o_value) {
	SEPARATOR_I_C(GAME)
	reinterpret_cast<PAGJGameState*>(&o_value.m_gameState)->load(i_stream);
	SEPARATOR_I_C(GAME)
	SEPARATOR_I_C(SHAD)
	reinterpret_cast<PAGJShaderState*>(&o_value.m_shaderState)->load(i_stream);
	SEPARATOR_I_C(SHAD)
	SEPARATOR_I_C(AUDI)
	reinterpret_cast<PAFMODAudioState*>(&o_value.m_audioState)->load(i_stream);
	SEPARATOR_I_C(AUDI)
	i_stream >> o_value.m_fields->m_position;
	SEPARATOR_I
	o_value.m_player1Checkpoint = PlayerCheckpoint::create();
	CC_SAFE_RETAIN(o_value.m_player1Checkpoint);
	reinterpret_cast<PAPlayerCheckpoint*>(o_value.m_player1Checkpoint)->load(i_stream);
	bool l_hasPlayer2;
	i_stream >> l_hasPlayer2;
	SEPARATOR_I
	if (l_hasPlayer2) {
		o_value.m_player2Checkpoint = PlayerCheckpoint::create();
		CC_SAFE_RETAIN(o_value.m_player2Checkpoint);
		reinterpret_cast<PAPlayerCheckpoint*>(o_value.m_player2Checkpoint)->load(i_stream);
	}
	i_stream >> o_value.m_unke78;
	SEPARATOR_I
	i_stream >> o_value.m_unke7c;
	SEPARATOR_I
	i_stream >> o_value.m_unke80;
	SEPARATOR_I
	if (i_stream.getPAVersion() > 1) {
		i_stream >> o_value.m_ground2Invisible;
		SEPARATOR_I
		i_stream >> o_value.m_streakBlend;
	} else {
		i_stream.read(reinterpret_cast<char*>(&o_value.m_ground2Invisible), 2);
	}
	SEPARATOR_I
	i_stream >> o_value.m_uniqueID;
	SEPARATOR_I
	i_stream >> o_value.m_respawnID;
	VEC_SEPARATOR_I
	i_stream >> o_value.m_vectorSavedObjectStateRef;
	VEC_SEPARATOR_I
	i_stream >> o_value.m_vectorActiveSaveObjectState;
	VEC_SEPARATOR_I
	i_stream >> o_value.m_vectorSpecialSaveObjectState;
	VEC_SEPARATOR_I
	SEPARATOR_I_C(EFFE)
	reinterpret_cast<PAEffectManagerState*>(&o_value.m_effectManagerState)->load(i_stream);
	SEPARATOR_I_C(EFFE)
	bool l_hasGradientTriggerObjectArray;
	i_stream >> l_hasGradientTriggerObjectArray;
	SEPARATOR_I
	if (l_hasGradientTriggerObjectArray) {
		o_value.m_gradientTriggerObjectArray = CCArray::create();
		CC_SAFE_RETAIN(o_value.m_gradientTriggerObjectArray);
		static_cast<PACCArray*>(o_value.m_gradientTriggerObjectArray)->load<GradientTriggerObject>(i_stream);
		ARR_SEPARATOR_I
	}
	i_stream >> o_value.m_unk11e8;
	UMAP_SEPARATOR_I
	i_stream >> o_value.m_sequenceTriggerStateUnorderedMap;
	UMAP_SEPARATOR_I
	if (i_stream.getPAVersion() > 1) {
		i_stream >> o_value.m_commandIndex;
	} else {
		i_stream.read(reinterpret_cast<char*>(&o_value.m_commandIndex), 8);
	}
	SEPARATOR_I

	// custom members

	i_stream >> o_value.m_fields->m_timePlayed;
	i_stream >> o_value.m_fields->m_timestamp;
	o_value.m_fields->m_wasLoaded = true;
}

inline void operator<<(Stream& o_stream, PSCheckpointObject& i_value) {
	SEPARATOR_O_C(GAME)
	reinterpret_cast<PAGJGameState*>(&i_value.m_gameState)->save(o_stream);
	SEPARATOR_O_C(GAME)
	SEPARATOR_O_C(SHAD)
	reinterpret_cast<PAGJShaderState*>(&i_value.m_shaderState)->save(o_stream);
	SEPARATOR_O_C(SHAD)
	SEPARATOR_O_C(AUDI)
	reinterpret_cast<PAFMODAudioState*>(&i_value.m_audioState)->save(o_stream);
	SEPARATOR_O_C(AUDI)
	o_stream << i_value.m_physicalCheckpointObject->m_startPosition;
	SEPARATOR_O
	reinterpret_cast<PAPlayerCheckpoint*>(i_value.m_player1Checkpoint)->save(o_stream);
	bool l_hasPlayer2 = false;
	if (i_value.m_player2Checkpoint) {
		l_hasPlayer2 = true;
	}
	o_stream << l_hasPlayer2;
	SEPARATOR_O
	if (l_hasPlayer2) {
		reinterpret_cast<PAPlayerCheckpoint*>(i_value.m_player2Checkpoint)->save(o_stream);
	}
	o_stream << i_value.m_unke78;
	SEPARATOR_O
	o_stream << i_value.m_unke7c;
	SEPARATOR_O
	o_stream << i_value.m_unke80;
	SEPARATOR_O
	o_stream << i_value.m_ground2Invisible;
	SEPARATOR_O
	o_stream << i_value.m_streakBlend;
	SEPARATOR_O
	o_stream << i_value.m_uniqueID;
	SEPARATOR_O
	o_stream << i_value.m_respawnID;
	VEC_SEPARATOR_O
	o_stream << i_value.m_vectorSavedObjectStateRef;
	VEC_SEPARATOR_O
	o_stream << i_value.m_vectorActiveSaveObjectState;
	VEC_SEPARATOR_O
	o_stream << i_value.m_vectorSpecialSaveObjectState;
	VEC_SEPARATOR_O
	SEPARATOR_O_C(EFFE)
	reinterpret_cast<PAEffectManagerState*>(&i_value.m_effectManagerState)->save(o_stream);
	SEPARATOR_O_C(EFFE)
	bool l_hasGradientTriggerObjectArray = false;
	if (i_value.m_gradientTriggerObjectArray) {
		l_hasGradientTriggerObjectArray = true;
	}
	o_stream << l_hasGradientTriggerObjectArray;
	SEPARATOR_O
	if (l_hasGradientTriggerObjectArray) {
		static_cast<PACCArray*>(i_value.m_gradientTriggerObjectArray)->save<GradientTriggerObject>(o_stream);
		ARR_SEPARATOR_O
	}
	o_stream << i_value.m_unk11e8;
	UMAP_SEPARATOR_O
	o_stream << i_value.m_sequenceTriggerStateUnorderedMap;
	UMAP_SEPARATOR_O
	o_stream << i_value.m_commandIndex;
	SEPARATOR_O

	// custom members

	o_stream << i_value.m_fields->m_timePlayed;
	o_stream << i_value.m_fields->m_timestamp;
}

void PSCheckpointObject::clean() {
	if (!m_fields->m_wasLoaded) return;

	reinterpret_cast<PAGJGameState*>(&m_gameState)->clean();

	reinterpret_cast<PAEffectManagerState*>(&m_effectManagerState)->clean();

	m_vectorSavedObjectStateRef.clear();
	gd::vector<SavedObjectStateRef>().swap(m_vectorSavedObjectStateRef);

	m_vectorActiveSaveObjectState.clear();
	gd::vector<SavedActiveObjectState>().swap(m_vectorActiveSaveObjectState);

	m_vectorSpecialSaveObjectState.clear();
	gd::vector<SavedSpecialObjectState>().swap(m_vectorSpecialSaveObjectState);
}

#if defined(PS_DEBUG) && defined(PS_DESCRIBE)
void PSCheckpointObject::describe() {
	log::info("[PSCheckpointObject - describe] start");
	reinterpret_cast<PACCObject*>(this)->describe();
	reinterpret_cast<PACCNode*>(this)->describe();
	reinterpret_cast<PAGJGameState*>(&m_gameState)->describe();
	reinterpret_cast<PAGJShaderState*>(&m_shaderState)->describe();
	reinterpret_cast<PAFMODAudioState*>(&m_audioState)->describe();
	log::info("[PSCheckpointObject - describe] m_physicalCheckpointObject->m_startPosition: {}", m_physicalCheckpointObject->m_startPosition);
	reinterpret_cast<PAPlayerCheckpoint*>(m_player1Checkpoint)->describe();
	if (m_player2Checkpoint) {
		reinterpret_cast<PAPlayerCheckpoint*>(m_player2Checkpoint)->describe();
	}
	log::info("[PSCheckpointObject - describe] m_unke78: {}", m_unke78);
	log::info("[PSCheckpointObject - describe] m_unke7c: {}", m_unke7c);
	log::info("[PSCheckpointObject - describe] m_unke80: {}", m_unke80);
	log::info("[PSCheckpointObject - describe] m_ground2Invisible: {}", m_ground2Invisible);
	log::info("[PSCheckpointObject - describe] m_streakBlend: {}", m_streakBlend);
	log::info("[PSCheckpointObject - describe] m_uniqueID: {}", m_uniqueID);
	log::info("[PSCheckpointObject - describe] m_respawnID: {}", m_respawnID);
	int l_size = m_vectorSavedObjectStateRef.size();
	log::info("[PSCheckpointObject - describe] m_vectorSavedObjectStateRef.size(): {}", l_size);
	for (int i = 0; i < l_size; i++) {
		log::info("[PSCheckpointObject - describe] m_vectorSavedObjectStateRef[{}]:", i);
		reinterpret_cast<PASavedObjectStateRef*>(&m_vectorSavedObjectStateRef[i])->describe();
	}
	l_size = m_vectorActiveSaveObjectState.size();
	log::info("[PSCheckpointObject - describe] m_vectorActiveSaveObjectState.size(): {}", l_size);
	for (int i = 0; i < l_size; i++) {
		log::info("[PSCheckpointObject - describe] m_vectorActiveSaveObjectState[{}]:", i);
		reinterpret_cast<PASavedActiveObjectState*>(&m_vectorActiveSaveObjectState[i])->describe();
	}
	l_size = m_vectorSpecialSaveObjectState.size();
	log::info("[PSCheckpointObject - describe] m_vectorSpecialSaveObjectState.size(): {}", l_size);
	for (int i = 0; i < l_size; i++) {
		log::info("[PSCheckpointObject - describe] m_vectorSpecialSaveObjectState[{}]:", i);
		reinterpret_cast<PASavedSpecialObjectState*>(&m_vectorSpecialSaveObjectState[i])->describe();
	}
	reinterpret_cast<PAEffectManagerState*>(&m_effectManagerState)->describe();
	if (m_gradientTriggerObjectArray) {
		reinterpret_cast<PACCArray*>(m_gradientTriggerObjectArray)->describe<GradientTriggerObject>();
	}
	log::info("[PSCheckpointObject - describe] m_unk11e8: {}", m_unk11e8);
	l_size = m_sequenceTriggerStateUnorderedMap.size();
	log::info("[PSCheckpointObject - describe] m_sequenceTriggerStateUnorderedMap.size(): {}", l_size);
	int i = 0;
	for (std::pair<int, SequenceTriggerState> l_pair : m_sequenceTriggerStateUnorderedMap) {
		log::info("[PSCheckpointObject - describe] m_sequenceTriggerStateUnorderedMap element {} key: {}", i, l_pair.first);
		log::info("[PSCheckpointObject - describe] m_sequenceTriggerStateUnorderedMap element {} value:", i);
		reinterpret_cast<PASequenceTriggerState*>(&l_pair.second)->describe();
		i++;
	}
	log::info("[PSCheckpointObject - describe] m_commandIndex: {}", m_commandIndex);
}
#endif