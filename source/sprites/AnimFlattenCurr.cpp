#include "AnimFlattenCurr.h"
#include "AnimSymbol.h"
#include "AnimFlatten.h"
#include "S2_Sprite.h"

#include <assert.h>

namespace s2
{

AnimFlattenCurr::AnimFlattenCurr(int max_frame_idx)
	: m_max_frame_idx(max_frame_idx)
{
}

AnimFlattenCurr* AnimFlattenCurr::Clone() const
{
	return new AnimFlattenCurr(m_max_frame_idx);
}

bool AnimFlattenCurr::Update(const UpdateParams& up, const Symbol* sym, const Sprite* spr, 
							 bool loop, float interval, int fps)
{
	if (!m_ctrl.IsActive()) {
		return false;
	}

	if (!m_ctrl.Update()) {
		return false;
	}

	bool dirty = false;

	int curr_frame = UpdateFrameCursor(loop, interval, fps);
	if (curr_frame != m_ctrl.GetFrame()) {
		m_ctrl.SetFrame(curr_frame);
		dirty = true;
	}

	const AnimSymbol* anim_sym = VI_DOWNCASTING<const AnimSymbol*>(sym);
	AnimFlatten* flatten = const_cast<AnimSymbol*>(anim_sym)->GetFlatten();
	assert(flatten);
	if (flatten->Update(up, curr_frame)) {
		dirty = true;
	}

	return dirty;
}

bool AnimFlattenCurr::SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps)
{
	if (frame == m_ctrl.GetFrame()) {
		return false;
	}

	frame = frame % m_max_frame_idx;
	m_ctrl.SetFrame(frame, fps);

	const AnimSymbol* sym = VI_DOWNCASTING<const AnimSymbol*>(spr->GetSymbol());
	AnimFlatten* flatten = const_cast<AnimSymbol*>(sym)->GetFlatten();
	assert(flatten);
	flatten->SetFrame(up, frame);

	return true;
}

void AnimFlattenCurr::Start(const UpdateParams& up, const Sprite* spr)
{
	m_ctrl.Reset();
	m_ctrl.SetFrame(0);
}

int AnimFlattenCurr::UpdateFrameCursor(bool loop, float interval, int fps)
{
	int curr_frame = static_cast<int>((m_ctrl.GetCurrTime() - m_ctrl.GetStartTime()) * fps);
	int max_frame = m_max_frame_idx - 1;
	int loop_max_frame = static_cast<int>(max_frame + interval * fps);
	if (loop) {
		if (curr_frame <= max_frame) {
			;
		} else if (curr_frame > max_frame && curr_frame <= loop_max_frame) {
			curr_frame = 0;
			m_ctrl.SetFrame(0);
		} else {
			curr_frame = 0;
			m_ctrl.SetFrame(0);
			m_ctrl.SetStartTime(m_ctrl.GetCurrTime());
		}
	} else {
		if (curr_frame > max_frame) {
			curr_frame = max_frame;
		}
	}
	return curr_frame;
}

}