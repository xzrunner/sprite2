#include "Flatten.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "ImageSymbol.h"
#include "S2_Texture.h"
#include "FlattenMgr.h"
#include "S2_Sprite.h"
#include "UpdateParams.h"
#include "SetStaticFrameVisitor.h"
#include "SprVisitorParams.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

Flatten::Flatten()
	: m_flags(0)
{
	InitFlags();
}

void Flatten::Combine(const Flatten& ft, const S2_MAT& prev_mt) 
{
	int base = m_quads.size();

	m_quads.reserve(m_quads.size() + ft.m_quads.size());
	for (int i = 0, n = ft.m_quads.size(); i < n; ++i) 
	{
		Quad q = ft.m_quads[i];
		q.vertices[0] = prev_mt * q.vertices[0];
		q.vertices[1] = prev_mt * q.vertices[1];
		q.vertices[2] = prev_mt * q.vertices[2];
		q.vertices[3] = prev_mt * q.vertices[3];
		m_images.push_back(ft.m_images[i]);
		SetTexcoordsNeedUpdate(true);
		m_quads.push_back(q);
	}

	m_nodes.reserve(m_nodes.size() + ft.m_nodes.size());
	for (int i = 0, n = ft.m_nodes.size(); i < n; ++i) 
	{
		m_nodes.push_back(ft.m_nodes[i]);
		Node& node = m_nodes.back();
		node.prev_mat = prev_mt * node.prev_mat;
		node.idx = base + node.idx;
	}
}

void Flatten::Clear() 
{ 
	SetTexcoordsNeedUpdate(true);
	m_images.clear();
	m_quads.clear(); 
	m_nodes.clear();
}

RenderReturn Flatten::Draw(const RenderParams& rp) const
{
	RenderReturn ret = RENDER_OK;

	if (!rp.IsDisableDTexC2() && IsTexcoordsNeedUpdate()) {
		UpdateDTexC2(0, m_quads.size());
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	if (m_nodes.empty())
	{
		if (m_quads.empty()) {
			ret = RENDER_NO_DATA;
		} else {
			ret = DrawQuads(0, m_quads.size(), rp, shader);
		}
	} 
	else
	{
		int begin = 0, end = 0;
		for (int i = 0, n = m_nodes.size(); i < n; ++i)
		{
			const Node& node = m_nodes[i];

			begin = end;
			end = node.idx;
			if (begin == end) {
				ret |= RENDER_NO_DATA;
			} else {
				ret |= DrawQuads(begin, end, rp, shader);
			}

			RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
			*rp_child = rp;
			rp_child->actor = node.actor;
			sm::Matrix2D::Mul(node.prev_mat, rp.mt, rp_child->mt);
			DrawNode::Draw(node.spr, *rp_child);
			RenderParamsPool::Instance()->Push(rp_child); 
		}
	}

	return ret;
}

bool Flatten::Update(const UpdateParams& up, const Sprite* spr)
{
	if (m_nodes.empty()) {
		return false;
	}

	bool dirty = false;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;
	up_child->Push(spr);
	for (int i = 0, n = m_nodes.size(); i < n; ++i) 
	{
		Sprite* child = m_nodes[i].spr;
		up_child->SetActor(child->QueryActor(up.GetActor()));
		if (child->Update(*up_child)) {
			dirty = true;
		}
	}
	UpdateParamsPool::Instance()->Push(up_child);

	return dirty;
}

void Flatten::SetFrame(const UpdateParams& up, int frame)
{
	if (m_nodes.empty()) {
		return;
	}

	for (int i = 0, n = m_nodes.size(); i < n; ++i) 
	{
		Sprite* child = m_nodes[i].spr;
		
		SetStaticFrameVisitor visitor(frame);
		SprVisitorParams vp;
		vp.actor = child->QueryActor(up.GetActor());
		child->Traverse(visitor, vp, false);
	}
}

void Flatten::AddQuad(const ImageSymbol* img, const sm::vec2 vertices[4])
{
	m_images.push_back(img);

	Quad q;
	img->QueryTexcoords(false, &q.texcoords[0].x, q.tex_id);
	memcpy(q.vertices, vertices, sizeof(q.vertices));
	m_quads.push_back(q);

	SetTexcoordsNeedUpdate(true);
}

void Flatten::AddNode(Sprite* spr, Actor* actor, const S2_MAT& prev_mat)
{
	Node node;
	if (spr) {
		spr->AddReference();
	}
	node.spr      = spr;
	node.actor    = actor;
	node.prev_mat = prev_mat;
	node.idx      = m_quads.size();
	m_nodes.push_back(node);
}

void Flatten::UpdateTexcoords() const
{
	SetTexcoordsNeedUpdate(true);
	for (int i = 0, n = m_quads.size(); i < n; ++i) {
		const ImageSymbol* src = m_images[i];
		Quad& dst = m_quads[i];
		src->QueryTexcoords(true, &dst.texcoords[0].x, dst.tex_id);
	}
}

RenderReturn Flatten::DrawQuads(int begin, int end, const RenderParams& rp, sl::Sprite2Shader* shader) const
{
	// todo: should use quad's color
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

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
	if (m_quads.empty()) {
		SetTexcoordsNeedUpdate(false);
		return;
	}

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

	prev_mat = node.prev_mat;

	idx = node.idx;

	return *this;
}

}