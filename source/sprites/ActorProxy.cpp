#include "ActorProxy.h"
#include "S2_Actor.h"
#include "S2_Sprite.h"

#include "S2_Symbol.h"

#include <assert.h>

namespace s2
{

CU_SINGLETON_DEFINITION(ActorProxyPool)

ActorProxy::ActorProxy(const ActorPtr& actor)
	: actor(actor)
	, spr(std::const_pointer_cast<Sprite>(actor->GetSpr()))
{
}

ActorProxyPool::ActorProxyPool()
{
}

ActorProxy* ActorProxyPool::Create(const ActorPtr& actor)
{
	auto itr = m_hash.find(actor.get());
	if (itr != m_hash.end()) {
		return itr->second.get();
	} else {
		auto new_p = mm::allocate_shared<ActorProxy>(actor);
		m_hash.insert(std::make_pair(actor.get(), new_p));
		return new_p.get();
	}
}

void ActorProxyPool::Delete(const ActorPtr& actor)
{
	auto itr = m_hash.find(actor.get());
	assert(itr != m_hash.end());
	m_hash.erase(itr);
}

}