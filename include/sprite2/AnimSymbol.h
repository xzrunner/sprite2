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

class AnimSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	class Frame
	{
	public:
		int index;
		std::vector<Sprite*> sprs;

		bool tween;
		std::vector<std::pair<AnimLerp::SprData, std::unique_ptr<ILerp>>> lerps;

	public:
		Frame() : index(0), tween(false) {}
		~Frame();

	}; // Frame

	class Layer
	{
	public:
		std::string name;
		std::vector<std::unique_ptr<Frame>> frames;

	public:
		int GetCurrFrame(int index) const;
		int GetNextFrame(int index) const;

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

	const std::vector<std::unique_ptr<Layer>>& GetLayers() const { return m_layers; }
	int GetMaxFrameIdx() const;

	void CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const;

	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }

	const std::shared_ptr<AnimCopy>& GetCopy() const;
	void LoadCopy();
	
	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(std::unique_ptr<Layer> layer, int idx = -1);
	bool Clear();

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	sm::rect CalcAABB(const Sprite* spr, const Actor* actor) const;

protected:
	std::vector<std::unique_ptr<Layer>> m_layers;

	int m_fps;

	mutable sm::rect m_aabb;

	mutable std::shared_ptr<AnimCopy> m_copy;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_