#ifndef _SPRITE2_ACTOR_PROXY_H_
#define _SPRITE2_ACTOR_PROXY_H_

#include "sprite2/typedef.h"

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

class ActorProxy
{
public:
	ActorProxy(const ActorPtr& actor);

	ActorPtr actor;
	SprPtr   spr;

private:	
	friend class ActorProxyPool;

}; // ActorProxy

class ActorProxyPool
{
public:
	// bool: if is new one
	bool Create(const ActorPtr& actor, ActorProxy*& out_proxy);
	void Delete(const ActorPtr& actor);

private:
	CU_UNORDERED_MAP<void*, std::shared_ptr<ActorProxy>> m_hash;

	CU_SINGLETON_DECLARATION(ActorProxyPool)

}; // ActorProxyPool

}

#endif // _SPRITE2_ACTOR_PROXY_H_