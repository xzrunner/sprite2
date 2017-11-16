#include "sprite2/ActorProxy.h"
#include "sprite2/Actor.h"
#include "sprite2/Sprite.h"

#include "sprite2/Symbol.h"

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

bool ActorProxyPool::Create(const ActorPtr& actor, ActorProxy*& out_proxy)
{
	auto itr = m_hash.find(actor.get());
	if (itr != m_hash.end()) {
		out_proxy = itr->second.get();
		return false;
	} else {
		auto new_p = CU_MAKE_SHARED<ActorProxy>(actor);
		m_hash.insert(std::make_pair(actor.get(), new_p));
		out_proxy = new_p.get();
		return true;
	}
}

void ActorProxyPool::Delete(const ActorPtr& actor)
{
	auto itr = m_hash.find(actor.get());
	assert(itr != m_hash.end());
	m_hash.erase(itr);
}

}