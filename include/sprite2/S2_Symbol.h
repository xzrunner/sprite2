#ifndef _SPRITE2_SYMBOL_H_
#define _SPRITE2_SYMBOL_H_

#include "pre_defined.h"

#include <SM_Rect.h>
#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <stdint.h>
#include <stddef.h>

namespace s2
{

class RenderParams;
class UpdateParams;
class Sprite;
class SymbolVisitor;
class Actor;

class Symbol : public VIRTUAL_INHERITANCE cu::RefCountObj, private cu::Uncopyable
{
public:
	Symbol();
	Symbol(uint32_t id);
	virtual ~Symbol();
	virtual int Type() const = 0;
	virtual void Traverse(const SymbolVisitor& visitor) = 0;
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const = 0;
	virtual bool Update(const UpdateParams& up, float time) { return false; }
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL) const = 0;
	virtual void SetBoundingDirty() {}
	
	int GetID() const { return m_id; }
	
private:
	uint32_t m_id;

}; // Symbol

}

#endif // _SPRITE2_SYMBOL_H_