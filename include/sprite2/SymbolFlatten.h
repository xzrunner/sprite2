#ifndef _SPRITE2_SYMBOL_FLATTEN_H_
#define _SPRITE2_SYMBOL_FLATTEN_H_

namespace s2
{

class SymbolFlatten
{
public:
	virtual ~SymbolFlatten() {}
	virtual void UpdateTexcoords() = 0;

}; // SymbolFlatten

}

#endif // _SPRITE2_SYMBOL_FLATTEN_H_