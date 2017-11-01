#ifndef _SPRITE2_MESH_SPRITE_H_
#define _SPRITE2_MESH_SPRITE_H_

#include "sprite2/Sprite.h"

#include <polymesh/MeshTransform.h>

namespace s2
{

class MeshSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MeshSprite();
	MeshSprite(const MeshSprite& mesh);
	MeshSprite& operator = (const MeshSprite& mesh);
	MeshSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~MeshSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg);
	virtual bool Update(const UpdateParams& up);
	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;

	void Lerp(const MeshSprite& begin, const MeshSprite& end, float process);

	const sm::vec2& GetSpeed() const { return m_speed; }
	void SetSpeed(const sm::vec2& spd) { m_speed = spd; }

	const pm::MeshTransform& GetMeshTrans() const { return m_trans; }
	pm::MeshTransform& GetMeshTrans() { return m_trans; }

	const SymConstPtr& GetBaseSym() const { return m_base; }
	void SetBaseSym(const SymPtr& sym) { m_base = sym; }

	bool OnlyDrawBound() const { return m_only_draw_bound; }
	void SetOnlyDrawBound(bool only_draw_bound) { m_only_draw_bound = only_draw_bound; }

protected:
	sm::vec2 m_speed;

	pm::MeshTransform m_trans;

	SymConstPtr m_base = nullptr;

	bool m_only_draw_bound;

	SPRITE_CLONE_FUNC(MeshSprite)

	VI_DUMMY_FUNC

}; // MeshSprite

}

#endif // _SPRITE2_MESH_SPRITE_H_