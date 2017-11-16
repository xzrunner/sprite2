#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/AnimLerp.h"
#include "sprite2/AnimState.h"

#include <cu/cu_stl.h>

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
		CU_VEC<SprPtr> sprs;

		bool tween;
		CU_VEC<std::pair<AnimLerp::SprData, std::unique_ptr<ILerp>>> lerps;

	public:
		Frame() : index(0), tween(false) {}

	}; // Frame

#ifdef USE_MM_ALLOCATOR
	using FramePtr = std::unique_ptr<Frame, mm::alloc_deleter<mm::Allocator<Frame>>>;
#else
	using FramePtr = std::unique_ptr<Frame>;
#endif // USE_MM_ALLOCATOR

	class Layer
	{
	public:
		CU_STR name;
		CU_VEC<FramePtr> frames;

	public:
		int GetCurrFrame(int index) const;
		int GetNextFrame(int index) const;

	}; // Layer

#ifdef USE_MM_ALLOCATOR
	using LayerPtr = std::unique_ptr<Layer, mm::alloc_deleter<mm::Allocator<Layer>>>;
#else
	using LayerPtr = std::unique_ptr<Layer>;
#endif // USE_MM_ALLOCATOR

public:
	AnimSymbol();
	AnimSymbol(uint32_t id);
	virtual ~AnimSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override;
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN
	virtual bool Update(const UpdateParams& up, float time) override;

	auto& GetLayers() const { return m_layers; }
	int GetMaxFrameIdx() const;

	void CreateFrameSprites(int frame, CU_VEC<SprPtr>& sprs) const;

	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }

	const std::shared_ptr<AnimCopy>& GetCopy() const;
	void LoadCopy();
	
	void BuildCurr();

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(LayerPtr& layer, int idx = -1);
	bool Clear();

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr,
		const Actor* actor = nullptr, bool cache = true) const override;

private:
	sm::rect CalcAABB(const Sprite* spr, const Actor* actor) const;

protected:
	CU_VEC<LayerPtr> m_layers;

	int m_fps;

	mutable sm::rect m_aabb;

	mutable std::shared_ptr<AnimCopy> m_copy = nullptr;

	AnimState m_state;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_