#pragma once
#include <sabe.persistenceapi/include/PersistenceAPI.hpp>

class CheckpointGameObjectReference {
public:
	CheckpointGameObject* m_reference;	

	CheckpointGameObjectReference() {
		m_reference = nullptr;
	};
	CheckpointGameObjectReference(CheckpointGameObject* i_reference) {
		m_reference = i_reference;
	}

	void load(persistenceAPI::Stream& i_stream);
	void save(persistenceAPI::Stream& o_stream);
};