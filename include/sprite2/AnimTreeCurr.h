#ifndef _SPRITE2_ANIM_TREE_CURR_H_
#define _SPRITE2_ANIM_TREE_CURR_H_

#include "S2_Message.h"
#include "AnimCopy.h"
#include "VisitResult.h"
#include "AnimCtrl.h"
#include "RenderReturn.h"

#include <CU_RefCountObj.h>

#include <vector>
#include <string>

namespace s2
{

class Actor;
class AnimCopy;
class Symbol;
class Sprite;
class AnimSymbol;
class SpriteVisitor;
class SprVisitorParams;
class RenderParams;
class UpdateParams;

class AnimTreeCurr : public cu::RefCountObj
{
public:
	AnimTreeCurr();
	AnimTreeCurr(const AnimTreeCurr& curr);
	AnimTreeCurr& operator = (const AnimTreeCurr& curr);
	virtual ~AnimTreeCurr();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimTreeCurr* Clone() const;

	virtual bool Update(const UpdateParams& up, const Symbol* sym, const Sprite* spr,
		bool loop = true, float interval = 0, int fps = 30);
	virtual bool SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps);
	virtual void Start(const UpdateParams& up, const Sprite* spr);
	virtual void OnMessage(const UpdateParams& up, const Sprite* spr, Message msg);
	virtual Sprite* FetchChildByName(int name, const Actor* actor) const;
	virtual Sprite* FetchChildByIdx(int idx) const;
	virtual VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;
	virtual int GetSlotSize() const { return m_slots.size(); }
	virtual RenderReturn Draw(const RenderParams& rp) const;
	virtual void Clear();
	virtual sm::rect CalcAABB(const Actor* actor) const;

	void SetAnimCopy(const AnimCopy* copy);

	int GetFrame() const { return m_ctrl.GetFrame(); }

	void SetTime(float time) { return m_ctrl.SetTime(time); }

	void SetActive(bool active) { m_ctrl.SetActive(active); }
	bool IsActive() const { return m_ctrl.IsActive(); }

	static void LoadSprLerpData(Sprite* spr, const AnimCopy::Lerp& lerp, int time);

private:
	int  UpdateFrameCursor(bool loop, float interval, int fps, bool reset_cursor);

	void ResetLayerCursor();

	void LoadCurrSprites(const UpdateParams& up, const Sprite* spr);
	void UpdateCursor();
	void LoadCurrSpritesImpl(const UpdateParams& up, const Sprite* spr);
	bool UpdateChildren(const UpdateParams& up, const Sprite* spr);

	void SetChildrenFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps);
		
private:
	AnimCtrl m_ctrl;

	const AnimCopy* m_copy;

	std::vector<int> m_layer_cursor;
	std::vector<int> m_layer_cursor_update;

	std::vector<Sprite*> m_slots;

	int* m_curr;
	int  m_curr_num;

}; // AnimTreeCurr

}

#endif // _SPRITE2_ANIM_TREE_CURR_H_