#include "sprite2/TexturePolygon.h"
#include "sprite2/ImageSymbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/TexcoordsMap.h"

#include <SM_Process.h>
#include <SM_Triangulation.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/Texture.h>

#include <assert.h>

#include <fstream>

namespace s2
{

TexturePolygon::TexturePolygon(const std::shared_ptr<const ImageSymbol>& img)
	: m_img(img)
{
}

void TexturePolygon::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	if (!m_img->GetTexture()->IsLoadFinished()) {
		return;
	}
	if (m_tris.empty()) {
		return;
	}
	assert(m_tris.size() == m_texcoords.size()
		&& m_tris.size() % 3 == 0);

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	for (int i = 0, n = m_tris.size(); i < n; i += 3) 
	{
		sm::vec2 vertices[4], texcoords[4];
		for (int j = 0; j < 3; ++j) {
			vertices[j] = rp.mt * m_tris[i+j];
			texcoords[j] = m_texcoords[i+j];
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		float _texcoords[8];
		int tex_id;
		if (!m_img->QueryTexcoords(!rp.IsDisableDTexC2(), _texcoords, tex_id)) {
			m_img->OnQueryTexcoordsFail(dlist);
		}

		TexcoordsMap::Trans(_texcoords, texcoords);

		shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
	}

	//auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	//const ee::pt2::Camera* cam = ee::pt2::CameraMgr::Instance()->GetCamera();
	//if (cam->Type() == "ortho") 
	//{
	//	shader_mgr.SetShader(sl::SPRITE2);
	//	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	//	for (int i = 0, n = m_tris.size(); i < n; i += 3) {
	//		sm::vec2 vertices[4], texcoords[4];
	//		for (int j = 0; j < 3; ++j) {
	//			vertices[j] = mt * m_tris[i+j];
	//			texcoords[j] = m_tris_texcoord[i+j];
	//		}
	//		vertices[3] = vertices[2];
	//		texcoords[3] = texcoords[2];

	//		// 		if (ee::Config::Instance()->IsUseDTex()) {
	//		// 			ee::DynamicTexAndFont::Instance()->Draw(vertices, texcoords, 
	//		// 				m_image->GetFilepath(), m_image->GetTexID());
	//		// 		} else {
	//		shader->Draw(&vertices[0].x, &texcoords[0].x, m_image->GetTexID());
	//		//		}
	//	}
	//}
	//else
	//{
	//	const ee::Pseudo3DCamera* pcam = static_cast<const ee::Pseudo3DCamera*>(cam);
	//	shader_mgr.SetShader(sl::SPRITE3);
	//	sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(shader_mgr.GetShader(sl::SPRITE3));
	//	for (int i = 0, n = m_tris.size(); i < n; i += 3) {
	//		CU_VEC<sm::vec3> vertices; 
	//		vertices.resize(3);
	//		CU_VEC<sm::vec2> texcoords;
	//		texcoords.resize(3);
	//		for (int j = 0; j < 3; ++j) {
	//			sm::vec2 v = mt * m_tris[i+j];
	//			vertices[j] = sm::vec3(v.x, v.y, 0);
	//			texcoords[j] = m_tris_texcoord[i+j];
	//		}
	//		shader->Draw(&vertices[0].x, &texcoords[0].x, m_image->GetTexID());
	//	}
	//}
}

void TexturePolygon::Build()
{
	m_tris.clear();
	m_texcoords.clear();

	CU_VEC<sm::vec2> outline;
	sm::rm_duplicate_nodes(m_outline, outline);

	sm::rect r;
	for (int i = 0, n = outline.size(); i < n; ++i) {
		r.Combine(outline[i]);
	}

	CU_VEC<sm::vec2> segments;
	GetTexBoundarySegments(r, segments);
	if (!m_segments.empty()) {
		copy(m_segments.begin(), m_segments.end(), back_inserter(segments));
	}

	sm::triangulate_lines(outline, segments, m_tris);

	CalTexcoords(r);
}

void TexturePolygon::GetTexBoundarySegments(const sm::rect& rect, CU_VEC<sm::vec2>& segments)
{
	static const int EXTEND = 1;
	sm::vec2 sz = m_img->GetNoTrimedSize();
	for (float x = rect.xmin; x < rect.xmax; x += sz.x)
	{
		segments.push_back(sm::vec2(x, rect.ymin - EXTEND));
		segments.push_back(sm::vec2(x, rect.ymax + EXTEND));
	}
	for (float y = rect.ymin; y < rect.ymax; y += sz.y)
	{
		segments.push_back(sm::vec2(rect.xmin - EXTEND, y));
		segments.push_back(sm::vec2(rect.xmax + EXTEND, y));
	}
}

void TexturePolygon::CalTexcoords(const sm::rect& rect)
{
	sm::vec2 sz = m_img->GetNoTrimedSize();
	int index = 0;
	for (int i = 0, n = m_tris.size() / 3; i < n; ++i)
	{
		float cx = 0, cy = 0;
		for (int j = 0; j < 3; ++j)
		{
			cx += m_tris[index + j].x;
			cy += m_tris[index + j].y;
		}
		cx /= 3;
		cy /= 3;

		int ix = static_cast<int>((cx - rect.xmin) / sz.x),
			iy = static_cast<int>((cy - rect.ymin) / sz.y);
		sm::vec2 base;
		base.x = rect.xmin + sz.x * ix;
		base.y = rect.ymin + sz.y * iy;

		for (int j = 0; j < 3; ++j)
		{
			float tx = (m_tris[index + j].x - base.x) / sz.x,
				  ty = (m_tris[index + j].y - base.y) / sz.y;
			m_texcoords.push_back(sm::vec2(tx, ty));
		}

		index +=  3;
	}
}

}