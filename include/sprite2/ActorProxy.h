#ifndef _SPRITE2_ACTOR_PROXY_H_
#define _SPRITE2_ACTOR_PROXY_H_

#include "s2_typedef.h"

#include <memmgr/Allocator.h>

#include <cu/cu_macro.h>

#include <unordered_map>

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
	ActorProxy* Create(const ActorPtr& actor);
	void Delete(const ActorPtr& actor);

private:
	std::unordered_map<void*, std::shared_ptr<ActorProxy>> m_hash;

	CU_SINGLETON_DECLARATION(ActorProxyPool)

}; // ActorProxyPool

}

#endif // _SPRITE2_ACTOR_PROXY_H_