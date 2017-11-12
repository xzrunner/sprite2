#ifndef _SPRITE2_PROXY_ACTOR_H_
#define _SPRITE2_PROXY_ACTOR_H_

#include "sprite2/Actor.h"

namespace s2
{

class ProxyActor : public Actor
{
public:
	ProxyActor(const SprConstPtr& spr, const ActorConstPtr& parent);

#ifndef S2_DISABLE_FLATTEN
	virtual void SetFlatten(const std::shared_ptr<ft::FTList>& ft, int pos) override;
	virtual void SetFlattenDirty() override;
	virtual void CreateFlatten() override;
	virtual void BuildFlatten() override;
	virtual bool HasFlatten() const override;
	virtual void FlattenUpdate(bool force) override;
	virtual void FlattenDraw(const RenderParams& rp) const override;
	virtual void FlattenSetFrame(int frame) override;
#endif // S2_DISABLE_FLATTEN

}; // ProxyActor

}

#endif // _SPRITE2_PROXY_ACTOR_H_