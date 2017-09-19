#ifndef _SPRITE2_COMPLEX_FLATTEN_H_
#define _SPRITE2_COMPLEX_FLATTEN_H_

#include "SymbolFlatten.h"
#include "sprite2/Flatten.h"

namespace s2
{

class ComplexFlatten : public SymbolFlatten
{
public:
	virtual void UpdateTexcoords();

	void Clear();

	RenderReturn Draw(const RenderParams& rp, int action) const;
	bool Update(const UpdateParams& up, int action);

	void SetData(const std::vector<Flatten>& actions) { m_actions = actions; }
	const Flatten* GetFlatten(int action) const;
	
private:
	std::vector<Flatten> m_actions;

}; // ComplexFlatten

}

#endif // _SPRITE2_COMPLEX_FLATTEN_H_