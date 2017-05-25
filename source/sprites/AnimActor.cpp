#include "AnimActor.h"
#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimCurrCreator.h"
#include "AnimCurr.h"

#include <assert.h>

namespace s2
{

AnimActor::AnimActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_curr(NULL)
{
	assert(spr->HaveActor());
////	// can't reuse curr, spr's curr will be used when can't find one from a path
//// 	if (spr->ActorCount() == 1) {
//// 		const AnimSprite* anim_spr = VI_DOWNCASTING<const AnimSprite*>(spr);
//// 		const AnimTreeCurr& curr = anim_spr->GetAnimCurr(path);
//// 		curr.AddReference();
//// 		m_curr = const_cast<AnimTreeCurr*>(&curr);
//// 	} else {
//		m_curr = new AnimTreeCurr();
////	}

	const AnimSymbol* anim_sym = VI_DOWNCASTING<const AnimSymbol*>(spr->GetSymbol());
	m_curr = AnimCurrCreator::Create(anim_sym, spr);	
}

AnimActor::~AnimActor()
{
	if (m_curr) {
		m_curr->RemoveReference();
	}
}

}