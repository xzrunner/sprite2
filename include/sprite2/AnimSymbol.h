#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "s2_config.h"

#include "AnimLerp.h"
#include "S2_Symbol.h"
#ifdef S2_ANIM_CURR_V0
#include "AnimCurr.h"
#elif defined S2_ANIM_CURR_V1
#include "AnimCurr2.h"
#include "AnimCopy.h"
#elif defined S2_ANIM_CURR_V2
#include "AnimCurr3.h"
#include "AnimCopy2.h"
#endif

#include <vector>

namespace s2
{

class AnimSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	class Frame
	{
	public:
		int index;
		std::vector<Sprite*> sprs;

		bool tween;
		std::vector<std::pair<AnimLerp::SprData, ILerp*> > lerps;

	public:
		Frame() : index(0), tween(false) {}

	}; // Frame

	class Layer
	{
	public:
		std::string name;
		std::vector<Frame*> frames;

	public:
		Frame* GetCurrFrame(int index) const;
		Frame* GetNextFrame(int index) const;

	}; // Layer

public:
	AnimSymbol();
	AnimSymbol(uint32_t id);
	virtual ~AnimSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymVisitor& visitor);
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual bool Update(const RenderParams& rp, float time);
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const std::vector<Layer*>& GetLayers() const { return m_layers; }
	int GetMaxFrameIdx() const;

	void CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const;

	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }

#ifdef S2_ANIM_CURR_V1
	const AnimCopy& GetCopy() const { return m_copy; }
	void LoadCopy();
#elif defined S2_ANIM_CURR_V2
	const AnimCopy2& GetCopy() const { return m_copy; }
	void LoadCopy();
#endif

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(Layer* layer);
	bool Clear();

private:
	void Draw(const RenderParams& rp, int idx = 1);

protected:
	std::vector<Layer*> m_layers;

	int m_fps;

#ifdef S2_ANIM_CURR_V0
	AnimCurr m_curr;
#elif defined S2_ANIM_CURR_V1
	AnimCopy  m_copy;
	AnimCurr2 m_curr;
#elif defined S2_ANIM_CURR_V2
	AnimCopy2 m_copy;
	AnimCurr3 m_curr;
#endif

	mutable sm::rect m_size;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_