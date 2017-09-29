#ifndef _SPRITE2_PROXY_ACTOR_H_
#define _SPRITE2_PROXY_ACTOR_H_

#include "S2_Actor.h"

namespace s2
{

class ProxyActor : public Actor
{
public:
	ProxyActor(const Sprite* spr, const Actor* parent);

#ifndef S2_DISABLE_FLATTEN
	virtual void SetFlatten(const std::shared_ptr<ft::FTList>& ft, int pos);
	virtual void SetFlattenDirty();
	virtual void CreateFlatten();
	virtual void BuildFlatten();
	virtual bool HasFlatten() const;
	virtual void FlattenUpdate(bool force);
	virtual void FlattenDraw(const s2::RenderParams& rp) const;
	virtual void FlattenSetFrame(int frame);
#endif // S2_DISABLE_FLATTEN

}; // ProxyActor

}

#endif // _SPRITE2_PROXY_ACTOR_H_