#include "AnimCurrCreator.h"
#include "AnimFlattenCurr.h"
#include "AnimTreeCurr.h"
#include "AnimSymbol.h"
#include "UpdateParams.h"

namespace s2
{

AnimCurr* AnimCurrCreator::Create(const AnimSymbol* sym, const Sprite* spr)
{
	AnimCurr* ret = NULL;

	bool flatten = sym->HasFlatten();
	if (flatten) 
	{
		AnimFlattenCurr* curr = new AnimFlattenCurr(sym->GetMaxFrameIdx());
		ret = curr;
	} 
	else 
	{
		AnimTreeCurr* curr = new AnimTreeCurr();
		curr->SetAnimCopy(sym->GetCopy());
		ret = curr;
	}
	ret->Start(UpdateParams(), spr);

	return ret;
}

}