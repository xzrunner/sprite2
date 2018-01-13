#ifndef _SPRITE2_MODEL_SPRITE_H_
#define _SPRITE2_MODEL_SPRITE_H_

#include "sprite2/config.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/Sprite.h"

#include <SM_Quaternion.h>

namespace s2
{

class ModelSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ModelSprite();
	ModelSprite(const ModelSprite& spr);
	ModelSprite& operator = (const ModelSprite& spr);
	ModelSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~ModelSprite();

	const sm::vec3& GetPos3() const { return m_pos3; }
	void SetPos3(const sm::vec3& pos) { m_pos3 = pos; }
	void Translate3(const sm::vec3& offset) { m_pos3 += offset; }

	const sm::Quaternion& GetOri3() const { return m_ori3; }
	void SetOri3(const sm::Quaternion& ori) { m_ori3 = ori; }
	void Rotate3(const sm::Quaternion& delta);

private:
	sm::vec3       m_pos3;
	sm::Quaternion m_ori3;

	S2_SPR_CLONE_FUNC(ModelSprite)

	VI_DUMMY_FUNC

}; // ModelSprite

}

#endif // S2_DISABLE_MODEL

#endif // _SPRITE2_MODEL_SPRITE_H_