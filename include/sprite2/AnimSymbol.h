#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "S2_Symbol.h"
#include "AnimLerp.h"

#include <vector>
#include <string>
#include <memory>

namespace s2
{

class AnimCopy;
class AnimCurr;

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
		~Frame();

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
	virtual RenderReturn Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual bool Update(const UpdateParams& up, float time);

	const std::vector<Layer*>& GetLayers() const { return m_layers; }
	int GetMaxFrameIdx() const;

	void CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const;

	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }

	const std::shared_ptr<AnimCopy>& GetCopy() const;
	void LoadCopy();

	void BuildCurr();
	
	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(Layer* layer, int idx = -1);
	bool Clear();

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	sm::rect CalcAABB(const Sprite* spr, const Actor* actor) const;

protected:
	std::vector<Layer*> m_layers;	// todo rm

	int m_fps;

	mutable sm::rect m_aabb;

	std::unique_ptr<AnimCurr> m_curr;

	mutable std::shared_ptr<AnimCopy> m_copy;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_