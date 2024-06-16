#pragma once
#include <sabe.persistenceutils/include/PersistenceUtils.hpp>

class CheckpointGameObjectReference {
public:
	CheckpointGameObject* m_reference;	

	CheckpointGameObjectReference() {
		m_reference = nullptr;
	};
	CheckpointGameObjectReference(CheckpointGameObject* i_reference) {
		m_reference = i_reference;
	}

	void load(persistenceUtils::InputStream& i_stream);
	void save(persistenceUtils::OutputStream& o_stream);
};