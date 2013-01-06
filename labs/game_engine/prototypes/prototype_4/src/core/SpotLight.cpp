#include "core_pch.h"
#include "..\..\include\core\SpotLight.h"
#include "core\MeshUtil.h"
#include "core\GPUBuffer.h"
#include "core\Sys_Graphics.h"
#include "core_utils.h"

#include "core\Material.h"
#include "core\RenderSystem.h"

namespace engine
{
	SpotLight::SpotLight(void) : Light(LT_SPOTLIGHT)
	{
		m_angle			= 45;
		m_nVerts		= 0;
		m_range			= 10;
	}


	SpotLight::~SpotLight(void)
	{
	}
	bool SpotLight::Create(Sys_GraphicsPtr pGraphics)
	{
		math::Vector3* pVerts = MeshUtil::CreateSpotLightCone(m_range, m_angle, 50, m_nVerts);

		m_pVB = pGraphics->CreateBuffer(BT_VERTEX_BUFFER, sizeof(math::Vector3) * m_nVerts, pVerts, false);
		
		mem_free(pVerts);
		
		if(m_pVB == GPUBufferPtr())
		{
			return false;
		}

		m_pMaterial = pGraphics->CreateMaterialFromFile("./assets/material/dr_render_spot_light.fx");
		if(m_pMaterial == MaterialPtr())
		{
			return false;
		}
		VertexFormat vf;
		vf.AddElement(VertexElement(0, VertexElement::POSITION, G_FORMAT_R32G32B32_FLOAT));
		m_pMaterial->SetVertexFormat(vf);

		return true;
	}
	const float& SpotLight::GetAngle()
	{
		return m_angle;
	}
	void SpotLight::SetAngle(const float& angle)
	{
		m_angle = angle;
	}
	const float& SpotLight::GetRange()
	{
		return m_range;
	}
	void SpotLight::SetRange(const float& range)
	{
		m_range = range;
	}
	void SpotLight::DrawLightVolumn(Sys_GraphicsPtr pGraphics)
	{
		pGraphics->SetVertexBuffer(m_pVB, 0, sizeof(math::Vector3));
		pGraphics->SetPrimitiveType(PT_TRIANGLE_LIST);

		pGraphics->Draw(m_nVerts, 0);
	}
	void SpotLight::Release()
	{
		if(m_pMaterial)
		{
			m_pMaterial->Release();
			m_pMaterial.reset();
		}
		if(m_pVB)
		{
			m_pVB->Release();
			m_pVB.reset();
		}
	}
	void SpotLight::RenderLight(RenderSystemPtr pRS)
	{
		const math::Matrix44& view = pRS->GetViewMatrix();
		const math::Matrix44& proj = pRS->GetProjMatrix();
		const math::Matrix44& world = GetWorldTM();

		m_pMaterial->SetWorldMatrix(world);
		m_pMaterial->SetViewMatrix(view);
		m_pMaterial->SetProjMatrix(proj);

		using namespace math;

		struct SpotLightParam
		{
			Vector3			color;
			
		};

		SpotLightParam l;
		
		//l.intensity = GetIntensity();
		const math::Color4& diffClr = GetDiffuseColor();
		l.color = math::Vector3(diffClr.r, diffClr.g, diffClr.b);
		//l.radius = GetRadius();
		//l.specular_pow = GetSpecularPow();

		m_pMaterial->SetCBByName("light", &l, sizeof(SpotLightParam));
		m_pMaterial->SetGBuffer(pRS->GetGBuffer());
		m_pMaterial->ApplyVertexFormat();
		
		Sys_GraphicsPtr pGraphics = pRS->GetSysGraphics();
		pGraphics->SetVertexBuffer(m_pVB, 0, sizeof(math::Vector3));
		pGraphics->SetPrimitiveType(PT_TRIANGLE_LIST);
		
		int nPass = 0;
		m_pMaterial->Begin(nPass);
		for(int i = 0; i < nPass; ++i)
		{
			m_pMaterial->ApplyPass(i);
			pGraphics->Draw(m_nVerts, 0);
		}
		m_pMaterial->End();
	}
}
