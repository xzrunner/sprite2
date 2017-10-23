#include "QueryLoadedVisitor.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "ImageSymbol.h"

namespace s2
{

QueryLoadedVisitor::QueryLoadedVisitor()
	: m_loaded(true)
{
}

VisitResult QueryLoadedVisitor::Visit(const SprConstPtr& spr, const SprVisitorParams& params)
{
	SymType type = static_cast<SymType>(spr->GetSymbol()->Type());
	if (type == SYM_SHAPE ||
		type == SYM_AUDIO) {
		return VISIT_OVER;
	} else if (type == SYM_IMAGE) {
		auto img_sym = S2_VI_PTR_DOWN_CAST<const ImageSymbol>(spr->GetSymbol());
		if (!img_sym->IsLoaded()) {
			m_loaded = false;
			return VISIT_STOP;
		} else {
			return VISIT_OVER;
		}
	} else {
		return VISIT_INTO;
	}
}

}