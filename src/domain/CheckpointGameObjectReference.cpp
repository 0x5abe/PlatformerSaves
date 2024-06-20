#include "Geode/binding/CheckpointGameObject.hpp"
#include "domain/CheckpointGameObjectReference.hpp"

using namespace geode::prelude;
using namespace persistenceAPI;

void CheckpointGameObjectReference::load(InputStream& i_stream) {
	int l_objectIndex;
	i_stream >> l_objectIndex;
	persistenceAPI::PAPlayLayer* l_playLayer = static_cast<persistenceAPI::PAPlayLayer*>(PlayLayer::get());
	if (l_playLayer) m_reference = static_cast<CheckpointGameObject*>(l_playLayer->getGameObject(l_objectIndex));
}

void CheckpointGameObjectReference::save(OutputStream& o_stream) {
	int l_objectIndex = -1;
	persistenceAPI::PAPlayLayer* l_playLayer = static_cast<persistenceAPI::PAPlayLayer*>(PlayLayer::get());
	if (l_playLayer) l_objectIndex = l_playLayer->getGameObjectIndex(m_reference);
	o_stream << l_objectIndex;
}