#ifndef _SPRITE2_UTILITY_H_
#define _SPRITE2_UTILITY_H_

#include <painting2/RenderColorCommon.h>
#include <painting2/RenderColorMap.h>

namespace s2
{

class Utility
{
public:
	static void PrepareMat(const S2_MAT& parent_mt, const Sprite* spr, const Actor* actor, S2_MAT& dst)
	{
		if (spr->IsMatDisable()) {
			dst = parent_mt;
		} else {
			if (actor && actor->IsGeoDirty()) {
				static S2_MAT tmp;
				sm::Matrix2D::Mul(spr->GetLocalMat(), parent_mt, tmp);
				sm::Matrix2D::Mul(actor->GetLocalMat(), tmp, dst);
			} else {
				sm::Matrix2D::Mul(spr->GetLocalMat(), parent_mt, dst);
			}
		}
	}

	static void PrepareColor(const pt2::RenderColorCommon& p_col_common, const pt2::RenderColorMap& p_col_map, 
		                     const Sprite* spr, const Actor* actor, 
		                     pt2::RenderColorCommon& dst_col_common, pt2::RenderColorMap& dst_col_map)
	{
		if (spr->IsColorDisable()) 
		{
			dst_col_common = p_col_common;
			dst_col_map = p_col_map;
		} 
		else 
		{
			if (actor && actor->IsColorDirty()) 
			{
				pt2::RenderColorCommon tmp_common;
				pt2::RenderColorMap    tmp_map;
				pt2::RenderColorCommon::Mul(spr->GetColorCommon(), p_col_common, tmp_common);
				pt2::RenderColorMap::Mul(spr->GetColorMap(), p_col_map, tmp_map);
				pt2::RenderColorCommon::Mul(actor->GetColorCommon(), tmp_common, dst_col_common);
				pt2::RenderColorMap::Mul(actor->GetColorMap(), tmp_map, dst_col_map);
			} 
			else 
			{
				pt2::RenderColorCommon::Mul(spr->GetColorCommon(), p_col_common, dst_col_common);
				pt2::RenderColorMap::Mul(spr->GetColorMap(), p_col_map, dst_col_map);
			}
		}
	}

}; // Utility

}

#endif // _SPRITE2_UTILITY_H_