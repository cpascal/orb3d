#include "StdAfx.h"
#include "Renderer.h"
#include "GridMesh.h"
#include "AppContext.h"
#include "EditorCamera.h"


Renderer::Renderer(void)
{
	m_bShowGrid			= true;
}


Renderer::~Renderer(void)
{
}
bool Renderer::Initialize(int vpw, int vph)
{
	using namespace math;
	using namespace engine;

	m_pGrid = boost::shared_ptr<GridMesh>(new GridMesh());

	if(false == m_pGrid->Init(300, 10))
	{
		return false;
	}
	m_pCamera = EditorCameraPtr(new EditorCamera);
	m_pCamera->SetViewPort(vpw, vph);

	m_pCamera->Init();
	m_pCamera->Update();
	
	
	return true;
}
void Renderer::Resize(int cx, int cy)
{
	engine::Sys_GraphicsPtr pGraphics = AppContext::GetSysGraphics();
	if(pGraphics != NULL)
	{
		if(cx != 0 && cy != 0)
		{
			AppContext::GetCoreApi()->GetRenderSystem()->ResizeFrameBuffer(cx, cy);
			m_pCamera->SetViewPort(cx, cy);
		}
	}
}
void Renderer::Release()
{
	if(m_pGrid)
	{
		m_pGrid->Release();
		m_pGrid.reset();
	}
}
void Renderer::Render()
{
	AppContext::GetCoreApi()->Update();

	m_bShowGrid ? AppContext::GetCoreApi()->AddRenderData(m_pGrid) : 0;

	m_pCamera->Update();

	AppContext::GetCoreApi()->Render();
	AppContext::GetCoreApi()->ClearRenderQueue();

	AppContext::GetCoreApi()->Present();

}
void Renderer::UpdateSemantics()
{

}
void Renderer::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pCamera->OnMouseMove(nFlags, point);
}
void Renderer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pCamera->OnMouseWheel(nFlags, zDelta, pt);
}
void Renderer::OnMouseRButtonDown(UINT nFlags, CPoint point)
{
	m_pCamera->OnMouseRButtonDown(nFlags, point);
}
void Renderer::OnMouseRButtonUp(UINT nFlags, CPoint point)
{
	m_pCamera->OnMouseRButtonUp(nFlags, point);
}

void Renderer::CreateObject_FromTemplate_Sphere()
{
	using namespace engine;

	Sys_GraphicsPtr pGraphics = AppContext::GetSysGraphics();


	GameObjectPtr pObj = AppContext::CreateGameObject(L"Sphere");

	MaterialPtr pMaterial = pGraphics->CreateMaterialFromFile("./assets/material/editor_shape.fx");
	pMaterial->SelectTechByName("dr_render_gbuffer");

	MeshPtr pMesh = MeshUtil::CreateSphere(20, 100, 100, pMaterial);

	RenderSystemPtr pRS = AppContext::GetCoreApi()->GetRenderSystem();

	MeshDataPtr pMD = boost::shared_dynamic_cast<MeshData>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"MeshData"));
	pMD->SetMesh(pMesh);

	pObj->AddComponent(pMD);

	MeshRendererPtr pMR = boost::shared_dynamic_cast<MeshRenderer>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"MeshRenderer"));
	pMR->SetRenderSystem(pRS);
	pObj->AddComponent(pMR);
}
void Renderer::CreateObject_FromTemplate_Plane()
{
	using namespace engine;

	GameObjectPtr pObj = AppContext::CreateGameObject(L"Plane");

	Sys_GraphicsPtr pGraphics = AppContext::GetSysGraphics();

	MaterialPtr pMaterial = pGraphics->CreateMaterialFromFile("./assets/material/editor_shape.fx");
	MeshPtr pMesh = MeshUtil::CreatePlane(200, pMaterial);

	RenderSystemPtr pRS = AppContext::GetCoreApi()->GetRenderSystem();

	MeshDataPtr pMD = boost::shared_dynamic_cast<MeshData>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"MeshData"));
	pMD->SetMesh(pMesh);

	pObj->AddComponent(pMD);

	MeshRendererPtr pMR = boost::shared_dynamic_cast<MeshRenderer>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"MeshRenderer"));
	pMR->SetRenderSystem(pRS);
	pObj->AddComponent(pMR);
}
void Renderer::CreateObject_FromTemplate_Cube()
{
	using namespace engine;

	GameObjectPtr pObj = AppContext::CreateGameObject(L"Cube");

	Sys_GraphicsPtr pGraphics = AppContext::GetSysGraphics();

	MaterialPtr pMaterial = pGraphics->CreateMaterialFromFile("./assets/material/editor_shape.fx");
	MeshPtr pMesh = MeshUtil::CreateCube(20, pMaterial);

	RenderSystemPtr pRS = AppContext::GetCoreApi()->GetRenderSystem();

	MeshDataPtr pMD = boost::shared_dynamic_cast<MeshData>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"MeshData"));
	pMD->SetMesh(pMesh);

	pObj->AddComponent(pMD);

	MeshRendererPtr pMR = boost::shared_dynamic_cast<MeshRenderer>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"MeshRenderer"));
	pMR->SetRenderSystem(pRS);
	pObj->AddComponent(pMR);

}
void Renderer::CreateObject_Empty()
{
	using namespace engine;
	GameObjectPtr pObj = AppContext::CreateGameObject(L"GameObject");
}
void Renderer::CreateObject_FromTemplate_DirLight()
{
	using namespace engine;
	
	GameObjectPtr pObj = AppContext::CreateGameObject(L"Directional Light");
		
	Light_DirPtr pLight = boost::shared_dynamic_cast<Light_Dir>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"DirectionalLight"));
	pLight->SetRenderSystem(AppContext::GetCoreApi()->GetRenderSystem());

	pObj->AddComponent(pLight);
}


void Renderer::CreateObject_FromTemplate_PointLight()
{
	using namespace engine;
	
	GameObjectPtr pObj = AppContext::CreateGameObject(L"Point Light");
		
	Light_PointPtr pLight = boost::shared_dynamic_cast<Light_Point>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"PointLight"));
	pLight->SetRenderSystem(AppContext::GetCoreApi()->GetRenderSystem());

	pObj->AddComponent(pLight);
	
}


void Renderer::CreateObject_FromTemplate_SpotLight()
{
	using namespace engine;
	
	GameObjectPtr pObj = AppContext::CreateGameObject(L"Spot Light");
		
	Light_SpotPtr pLight = boost::shared_dynamic_cast<Light_Spot>(AppContext::GetCoreApi()->CreateGameObjectComponent(L"SpotLight"));
	pLight->SetRenderSystem(AppContext::GetCoreApi()->GetRenderSystem());

	pObj->AddComponent(pLight);

}
void Renderer::ShowGrid(bool bShow)
{
	m_bShowGrid = bShow;
}
bool Renderer::ShowingGrid()
{
	return m_bShowGrid;
}
