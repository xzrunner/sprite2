#ifndef _SPRITE2_MESH_SPRITE_H_
#define _SPRITE2_MESH_SPRITE_H_

#include "S2_Sprite.h"

#include <polymesh/MeshTransform.h>

namespace s2
{

class MeshSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MeshSprite();
	MeshSprite(const MeshSprite& mesh);
	MeshSprite& operator = (const MeshSprite& mesh);
	MeshSprite(Symbol* sym, uint32_t id = -1);
	virtual ~MeshSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual MeshSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(Message msg, const SprTreePath& path);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& path, bool force = false);
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;

	void Lerp(const MeshSprite* begin, const MeshSprite* end, float process);

	const sm::vec2& GetSpeed() const { return m_speed; }
	void SetSpeed(const sm::vec2& spd) { m_speed = spd; }

	const pm::MeshTransform& GetMeshTrans() const { return m_trans; }
	pm::MeshTransform& GetMeshTrans() { return m_trans; }

	const Symbol* GetBaseSym() const { return m_base; }
	void SetBaseSym(const Symbol* sym);

	bool OnlyDrawBound() const { return m_only_draw_bound; }
	void SetOnlyDrawBound(bool only_draw_bound) { m_only_draw_bound = only_draw_bound; }

protected:
	sm::vec2 m_speed;

	pm::MeshTransform m_trans;

 	const Symbol* m_base;

	bool m_only_draw_bound;

	VI_DUMMY_FUNC

}; // MeshSprite

}

#endif // _SPRITE2_MESH_SPRITE_H_