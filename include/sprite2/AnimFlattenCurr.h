#ifndef _SPRITE2_ANIM_FLATTEN_CURR_H_
#define _SPRITE2_ANIM_FLATTEN_CURR_H_

#include "AnimCurr.h"

namespace s2
{

class AnimFlattenCurr : public AnimCurr
{
public:
	AnimFlattenCurr(int max_frame_idx);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimFlattenCurr* Clone() const;

	/**
	 *  @interface
	 *    AnimCurr
	 */
	virtual bool Update(const UpdateParams& up, const Symbol* sym, const Sprite* spr,
		bool loop = true, float interval = 0, int fps = 30);	
	virtual void SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps);
	virtual void Start(const UpdateParams& up, const Sprite* spr);
	virtual void OnMessage(const UpdateParams& up, const Sprite* spr, Message msg) {}
	virtual Sprite* FetchChildByName(int name, const Actor* actor) const { return NULL; }
	virtual Sprite* FetchChildByIdx(int idx) const { return NULL; }
	virtual VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const { return VISIT_OVER; }
	virtual int GetSlotSize() const { return 0; }
	virtual RenderReturn Draw(const RenderParams& rp) const { return RENDER_OK; }
	virtual RenderReturn DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const { return RENDER_OK; }
	virtual void Clear() {}
	virtual sm::rect CalcAABB(const Actor* actor) const { return sm::rect(); }

private:
	int UpdateFrameCursor(bool loop, float interval, int fps);

private:
	int m_max_frame_idx;

}; // AnimFlattenCurr

}

#endif // _SPRITE2_ANIM_FLATTEN_CURR_H_