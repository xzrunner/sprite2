#ifndef _SPRITE2_ANIM_CURR_H_
#define _SPRITE2_ANIM_CURR_H_

#include "AnimCtrl.h"
#include "S2_Message.h"
#include "VisitResult.h"
#include "SpriteVisitor.h"
#include "RenderReturn.h"

#include <SM_Rect.h>

#include <CU_RefCountObj.h>

namespace s2
{

class UpdateParams;
class Symbol;
class Sprite;
class Actor;
class RenderParams;

class AnimCurr : public cu::RefCountObj
{
public:
	virtual ~AnimCurr() {}	

	virtual AnimCurr* Clone() const = 0;

	virtual bool Update(const UpdateParams& up, const Symbol* sym, const Sprite* spr,
		bool loop = true, float interval = 0, int fps = 30) = 0;
	virtual void SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps) = 0;
	virtual void Start(const UpdateParams& up, const Sprite* spr) = 0;
	virtual void OnMessage(const UpdateParams& up, const Sprite* spr, Message msg) = 0;
	virtual Sprite* FetchChildByName(int name, const Actor* actor) const = 0;
	virtual Sprite* FetchChildByIdx(int idx) const = 0;
	virtual VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const = 0;
	virtual int GetSlotSize() const = 0;
	virtual RenderReturn Draw(const RenderParams& rp) const = 0;
	virtual void Clear() = 0;
	virtual sm::rect CalcAABB(const Actor* actor) const = 0;

	int GetFrame() const { return m_ctrl.GetFrame(); }

	void SetTime(float time) { return m_ctrl.SetTime(time); }

	void SetActive(bool active) { m_ctrl.SetActive(active); }
	bool IsActive() const { return m_ctrl.IsActive(); }

protected:
	AnimCtrl m_ctrl;

}; // AnimCurr

}

#endif // _SPRITE2_ANIM_CURR_H_