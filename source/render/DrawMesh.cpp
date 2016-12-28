#include "DrawMesh.h"
#include "MeshTriangle.h"
#include "Mesh.h"
#include "S2_RVG.h"
#include "Color.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "RenderTarget.h"

#include <unirender/RenderContext.h>
#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

#include <set>
#include <vector>

namespace s2
{

static Color RED	(204, 51, 102, 128);
static Color GREEN	(102, 204, 51, 128);
static Color BLUE	(102, 51, 204, 128);

void DrawMesh::DrawInfoUV(const Mesh* mesh, const sm::mat4* mt)
{
	std::set<sm::vec2, sm::Vector2Cmp> unique;
	std::vector<sm::vec2> tmp(3);
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	float w = mesh->GetWidth(),
		  h = mesh->GetHeight();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * w;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * h;
			if (mt) {
				tmp[i] = *mt * tmp[i];
			}
			unique.insert(tmp[i]);
		}
		RVG::SetColor(RED);
		RVG::Polyline(tmp, true);
	}
	std::vector<sm::vec2> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	RVG::SetColor(BLUE);
	RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void DrawMesh::DrawInfoXY(const Mesh* mesh, const sm::mat4* mt)
{
	std::set<sm::vec2, sm::Vector2Cmp> unique;
	std::vector<sm::vec2> tmp(3);
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			if (mt) {
				tmp[i] = *mt * tmp[i];
			}
			unique.insert(tmp[i]);
		}
		RVG::SetColor(RED);
		RVG::Polyline(tmp, true);
	}
	std::vector<sm::vec2> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	RVG::SetColor(BLUE);
	RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void DrawMesh::DrawTexture(const Mesh* mesh, const RenderParams& params,
							   const Symbol* base_sym)
{
	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Close();

	const ur::RenderTarget* rt0 = RenderTarget::Instance()->GetRT0();
	int w = rt0->GetTexture()->Width(),
		h = rt0->GetTexture()->Height();
	RenderCtxStack::Instance()->Push(RenderCtx(w, h, w, h));

	DrawMeshToTmp(mesh, params, base_sym);

	RenderCtxStack::Instance()->Pop();

	RenderScissor::Instance()->Open();

	DrawTmpToScreen(mesh, params.mt);
}

void DrawMesh::DrawOnlyMesh(const Mesh* mesh, const sm::mat4& mt, int texid)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);

	const ur::RenderTarget* rt0 = RenderTarget::Instance()->GetRT0();
	int w = rt0->GetTexture()->Width(),
		h = rt0->GetTexture()->Height();
	float ori_w = mesh->GetWidth(),
		  ori_h = mesh->GetHeight();

	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = tris[i];
		sm::vec2 vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = mt * tri->nodes[i]->xy;
			texcoords[i].x = (tri->nodes[i]->uv.x * ori_w - ori_w * 0.5f + w * 0.5f) / w;
			texcoords[i].y = (tri->nodes[i]->uv.y * ori_h - ori_h * 0.5f + h * 0.5f) / h;
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
	}
}

void DrawMesh::DrawMeshToTmp(const Mesh* mesh, const RenderParams& params,
								 const Symbol* base_sym)
{
	RenderTarget::Instance()->GetRT0()->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams _params = params;
	_params.mt.Identity();
	if (base_sym) {
		DrawNode::Draw(base_sym, _params);
	} else {
		DrawNode::Draw(mesh->GetBaseSymbol(), _params);
	}

	mgr->FlushShader();

	RenderTarget::Instance()->GetRT0()->Unbind();
}

void DrawMesh::DrawTmpToScreen(const Mesh* mesh, const sm::mat4& mt)
{
	DrawOnlyMesh(mesh, mt, RenderTarget::Instance()->GetRT0()->GetTexture()->ID());
}

}