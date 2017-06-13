#include "ModelSymbol.h"

#ifndef S2_DISABLE_MODEL

#include "SymType.h"

#include <model3/Model.h>
#include <model3/Mesh.h>
#include <model3/m3_typedef.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Model3Shader.h>

namespace s2
{

ModelSymbol::ModelSymbol()
	: m_model(NULL)
{
}

ModelSymbol::~ModelSymbol()
{
	if (m_model) {
		m_model->RemoveReference();
	}
}

int ModelSymbol::Type() const
{
	return SYM_MODEL;
}

RenderReturn ModelSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
// 	if (!spr) {
// 		return;
// 	}
//	const Sprite* s = dynamic_cast<const Sprite*>(spr);

// 	sm::mat4 mat = sm::mat4(s->GetOri3().ToMatrix()) * 
// 		sm::mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);
// 	e3d::DrawCube(mat, m_aabb, ee::BLACK);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MODEL3);
	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader());
	const std::vector<m3::Mesh*>& meshes = m_model->GetAllMeshes();
	for (int i = 0, n = meshes.size(); i < n; ++i) 
	{
		const m3::Mesh* mesh = meshes[i];

		const m3::Material& material = mesh->GetMaterial();
		shader->SetMaterial(material.ambient, material.diffuse, material.specular, 
			material.shininess, material.tex);
		shader->SetLightPosition(sm::vec3(0.25f, 0.25f, 1));

		int vertex_type = mesh->GetVertexType();
		bool normal = vertex_type & m3::VERTEX_FLAG_NORMALS;
		bool texcoords = vertex_type & m3::VERTEX_FLAG_TEXCOORDS;
		shader->Draw(mesh->GetVertices(), mesh->GetIndices(), normal, texcoords);
	}

	return s2::RENDER_OK;
}

void ModelSymbol::SetModel(m3::Model* model)
{
	cu::RefCountObjAssign(m_model, model);
}

sm::rect ModelSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	// empty
	return sm::rect();
}

}

#endif // S2_DISABLE_MODEL