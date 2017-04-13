#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "AnimLerp.h"
#include "S2_Symbol.h"
#include "AnimCurr.h"
#include "AnimCopy.h"

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
	virtual void Traverse(const SymbolVisitor& visitor);
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual bool Update(const UpdateParams& up, float time);
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

	const std::vector<Layer*>& GetLayers() const { return m_layers; }
	int GetMaxFrameIdx() const;

	void CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const;

	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }

	const AnimCopy& GetCopy() const { return m_copy; }
	void LoadCopy();

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(Layer* layer);
	bool Clear();

private:
	void Draw(const RenderParams& rp, int idx = 1);

	sm::rect CalcAABB(const Sprite* spr, const Actor* actor) const;

protected:
	std::vector<Layer*> m_layers;

	int m_fps;

	AnimCopy m_copy;
	AnimCurr m_curr;

	mutable sm::rect m_aabb;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_