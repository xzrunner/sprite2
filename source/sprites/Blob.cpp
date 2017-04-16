#include "Blob.h"
#include "ImageSprite.h"
#include "ImageSymbol.h"
#include "S2_Symbol.h"
#include "RenderParams.h"

#include <SM_Rect.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

Blob::Blob()
	: m_spr(NULL)
{
}

Blob::Blob(const Sprite* spr, const sm::mat4& mat)
	: m_spr(spr)
	, m_mat(mat)
{
}

void Blob::Add(const ImageSprite* img_spr, const sm::mat4& prev_mat)
{
	Quad quad;
	
	sm::mat4 mat = prev_mat * img_spr->GetLocalMat();
	sm::rect sz = img_spr->GetSymbol()->GetBounding(img_spr);
	quad.vertices[0] = mat * sm::vec2(sz.xmin, sz.ymin);
	quad.vertices[1] = mat * sm::vec2(sz.xmax, sz.ymin);
	quad.vertices[2] = mat * sm::vec2(sz.xmax, sz.ymax);
	quad.vertices[3] = mat * sm::vec2(sz.xmin, sz.ymax);

	float texcoords[8];
	int tex_id;
	const ImageSymbol* img_sym = VI_DOWNCASTING<const ImageSymbol*>(img_spr->GetSymbol());
	if (!img_sym->QueryTexcoords(RenderParams(), texcoords, tex_id)) {
		return;
	}
	
	quad.texcoords[0].Set(texcoords[0], texcoords[1]);
	quad.texcoords[1].Set(texcoords[2], texcoords[3]);
	quad.texcoords[2].Set(texcoords[4], texcoords[5]);
	quad.texcoords[3].Set(texcoords[6], texcoords[7]);

	quad.texid = tex_id;

	m_quads.push_back(quad);
}

void Blob::Draw(const RenderParams& rp) const
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	sm::vec2 vertices_trans[4];
	for (int i = 0, n = m_quads.size(); i < n; ++i) 
	{
		Quad& q = m_quads[i];
 		vertices_trans[0] = rp.mt * q.vertices[0];
 		vertices_trans[1] = rp.mt * q.vertices[1];
 		vertices_trans[2] = rp.mt * q.vertices[2];
 		vertices_trans[3] = rp.mt * q.vertices[3];
		shader->DrawQuad(&vertices_trans[0].x, &q.texcoords[0].x, q.texid);
	}
}

}