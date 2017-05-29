#include "Flatten.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "ImageSymbol.h"
#include "S2_Texture.h"
#include "FlattenMgr.h"
#include "S2_Sprite.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

Flatten::Flatten()
{
	InitFlags();
}

void Flatten::Combine(const Flatten& ft, const S2_MAT& mt) 
{
	int base = m_quads.size();

	m_quads.reserve(m_quads.size() + ft.m_quads.size());
	for (int i = 0, n = ft.m_quads.size(); i < n; ++i) 
	{
		Quad q = ft.m_quads[i];
		q.vertices[0] = mt * q.vertices[0];
		q.vertices[1] = mt * q.vertices[1];
		q.vertices[2] = mt * q.vertices[2];
		q.vertices[3] = mt * q.vertices[3];
		m_images.push_back(ft.m_images[i]);
		m_quads.push_back(q);
	}

	m_nodes.reserve(m_nodes.size() + ft.m_nodes.size());
	for (int i = 0, n = ft.m_nodes.size(); i < n; ++i) 
	{
		m_nodes.push_back(ft.m_nodes[i]);
		Node& node = m_nodes.back();
		node.mat = mt * node.mat;
		node.idx = base + node.idx;
	}
}

void Flatten::Clear() 
{ 
	m_images.clear();
	m_quads.clear(); 
	m_nodes.clear();
}

RenderReturn Flatten::Draw(const RenderParams& rp) const
{
	// todo: should use quad's color
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	RenderReturn ret = RENDER_OK;

	if (m_nodes.empty())
	{
		ret = DrawQuads(0, m_quads.size(), rp, shader);
	} 
	else
	{
		int begin = 0, end = 0;
		for (int i = 0, n = m_nodes.size(); i < n; ++i)
		{
			const Node& node = m_nodes[i];
			begin = end;
			end = node.idx;
			ret |= DrawQuads(begin, end, rp, shader);

			RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
			*rp_child = rp;
			rp_child->actor = node.actor;
			sm::Matrix2D::Mul(node.mat, rp.mt, rp_child->mt);
			DrawNode::Draw(node.spr, *rp_child);
			RenderParamsPool::Instance()->Push(rp_child); 
		}
	}

	return ret;
}

void Flatten::AddQuad(const ImageSymbol* img, const sm::vec2 vertices[4])
{
	m_images.push_back(img);

	Quad q;
	img->QueryTexcoords(false, &q.texcoords[0].x, q.tex_id);
	memcpy(q.vertices, vertices, sizeof(q.vertices));
	m_quads.push_back(q);
}

void Flatten::AddNode(const Sprite* spr, const Actor* actor, const S2_MAT& mat)
{
	Node node;
	if (spr) {
		spr->AddReference();
	}
	node.spr = spr;
	node.actor = actor;
	node.mat = mat;
	node.idx = m_quads.size();
	m_nodes.push_back(node);
}

void Flatten::UpdateTexcoords() const
{
	for (int i = 0, n = m_quads.size(); i < n; ++i) {
		const ImageSymbol* src = m_images[i];
		Quad& dst = m_quads[i];
		src->QueryTexcoords(true, &dst.texcoords[0].x, dst.tex_id);
	}
}

RenderReturn Flatten::DrawQuads(int begin, int end, const RenderParams& rp, sl::Sprite2Shader* shader) const
{
	if (begin == end) {
		return RENDER_NO_DATA;
	}

	if (!rp.IsDisableDTexC2() && IsTexcoordsNeedUpdate()) {
		UpdateDTexC2(begin, end);
	}

	static sm::vec2 VERTEX_BUF[4];

	float x, y;
	const Quad* ptr_quad = &m_quads[begin];
	const float* mt = rp.mt.x;
	for (int i = begin; i < end; ++i, ++ptr_quad)
	{
		const sm::vec2* ptr_src = ptr_quad->vertices;
		sm::vec2* ptr_dst = &VERTEX_BUF[0];

		x = (ptr_src->x * mt[0] + ptr_src->y * mt[2]) + mt[4];
		y = (ptr_src->x * mt[1] + ptr_src->y * mt[3]) + mt[5];
		ptr_dst->x = x;
		ptr_dst->y = y;
		++ptr_dst;
		++ptr_src;

		x = (ptr_src->x * mt[0] + ptr_src->y * mt[2]) + mt[4];
		y = (ptr_src->x * mt[1] + ptr_src->y * mt[3]) + mt[5];
		ptr_dst->x = x;
		ptr_dst->y = y;
		++ptr_dst;
		++ptr_src;

		x = (ptr_src->x * mt[0] + ptr_src->y * mt[2]) + mt[4];
		y = (ptr_src->x * mt[1] + ptr_src->y * mt[3]) + mt[5];
		ptr_dst->x = x;
		ptr_dst->y = y;
		++ptr_dst;
		++ptr_src;

		x = (ptr_src->x * mt[0] + ptr_src->y * mt[2]) + mt[4];
		y = (ptr_src->x * mt[1] + ptr_src->y * mt[3]) + mt[5];
		ptr_dst->x = x;
		ptr_dst->y = y;
		++ptr_dst;
		++ptr_src;

		shader->DrawQuad(&VERTEX_BUF[0].x, &ptr_quad->texcoords[0].x, ptr_quad->tex_id);
	}

	return RENDER_OK;
}

void Flatten::UpdateDTexC2(int begin, int end) const
{
	bool loaded = false;

	bool need_update = false;

	Quad* ptr_quad = &m_quads[begin];
	const ImageSymbol*const* ptr_img = &m_images[begin];
	for (int i = begin; i < end; ++i, ++ptr_quad, ++ptr_img)	{
		if (ptr_quad->tex_id == (*ptr_img)->GetTexture()->GetTexID()) 
		{
			need_update = true;
			if ((*ptr_img)->OnQueryTexcoordsFail()) {
				loaded = true;
			}
		}
	}

	SetTexcoordsNeedUpdate(need_update);

	if (loaded) {
		FlattenMgr::Instance()->UpdateTexcoords();
	}
}

void Flatten::InitFlags()
{
	SetTexcoordsNeedUpdate(true);
}

/************************************************************************/
/* class Flatten::Quad                                                  */
/************************************************************************/

Flatten::Quad::Quad(const Quad& quad)
{
	this->operator = (quad);
}

Flatten::Quad& Flatten::Quad::operator = (const Quad& quad)
{
	tex_id = quad.tex_id;
	memcpy(vertices, quad.vertices, sizeof(vertices));
	memcpy(texcoords, quad.texcoords, sizeof(texcoords));
	return *this;
}

/************************************************************************/
/* class Flatten::Node                                                  */
/************************************************************************/

Flatten::Node::Node()
	: spr(NULL)
	, actor(NULL)
	, idx(-1)
{
}

Flatten::Node::Node(const Node& node)
{
	this->operator = (node);
}

Flatten::Node& Flatten::Node::operator = (const Node& node)
{
	spr = node.spr;
	if (spr) {
		spr->AddReference();
	}

	actor = node.actor;

	mat = node.mat;

	idx = node.idx;

	return *this;
}

}