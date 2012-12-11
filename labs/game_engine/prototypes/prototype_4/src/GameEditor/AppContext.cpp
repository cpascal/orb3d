#include "StdAfx.h"
#include "AppContext.h"
#include "Renderer.h"
#include "MainFrm.h"
#include "Project.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"


engine::CoreApiPtr					AppContext::s_pCore;
engine::Sys_GraphicsPtr				AppContext::s_pSysGraphics;
RendererPtr							AppContext::s_pRenderer;

int									AppContext::s_RTWidth	= 0;
int									AppContext::s_RTHeight	= 0;
AppContext::AppContext(void)
{
}


AppContext::~AppContext(void)
{
}
engine::CoreApiPtr AppContext::GetCoreApi()
{
	return s_pCore;
}
engine::RenderSystemPtr AppContext::GetRenderSystem()
{
	return s_pCore->GetRenderSystem();
}
bool AppContext::InitContext(HWND hwnd, int w, int h)
{
	s_RTWidth = w;
	s_RTHeight = h;
	using namespace engine;

	s_pCore = CoreApiPtr(new CoreApi());

	GraphicsSetting setting;
	setting.backBufferCount = 2;
	setting.depthStencilFormat = G_FORMAT_D24_UNORM_S8_UINT;
	setting.frameBufferFormat = G_FORMAT_R8G8B8A8_UNORM;
	setting.frameBufferHeight = h;
	setting.frameBufferWidth = w;
	setting.multiSampleCount = 1;
	setting.multiSampleQuality = 0;
	setting.windowed = true;
	setting.wnd = hwnd;

	if(false == s_pCore->Initialize(setting))
	{
		return false;
	}

	s_pRenderer = RendererPtr(new Renderer);
	if(s_pRenderer->Initialize(w, h) == false)
	{
		return false;
	}

	return true;
}
engine::Sys_GraphicsPtr AppContext::GetSysGraphics()
{
	if(s_pCore == NULL)
	{
		return engine::Sys_GraphicsPtr();
	}
	return s_pCore->GetSysGraphics();
}
void AppContext::ReleaseContext()
{
	if(s_pRenderer)
	{
		s_pRenderer->Release();
		s_pRenderer.reset();
	}

	if(s_pCore != NULL)
	{
		s_pCore->Release();
		s_pCore.reset();
	}
}
void AppContext::UpdateContext()
{
	if(s_pCore)
	{
		s_pCore->Update();
	}
}
int AppContext::GetRTViewWidth()
{
	return s_RTWidth;
}
int AppContext::GetRTViewHeight()
{
	return s_RTHeight;
}
void AppContext::ResizeRenderer(int cx, int cy)
{
	s_RTWidth = cx;
	s_RTHeight = cy;

	if(s_pRenderer)
	{
		s_pRenderer->Resize(cx, cy);
	}
}
RendererPtr AppContext::GetRenderer()
{
	return s_pRenderer;
}
CMainFrame* AppContext::GetMainFrame()
{
	return (CMainFrame*)AfxGetMainWnd();
}
void AppContext::UpdateObjectView()
{
	CMainFrame* pMain = GetMainFrame();
	pMain->GetObjectView()->UpdateObjectView();
}

void AppContext::OutputInfo(const CString& info)
{
	CMainFrame* pMain = GetMainFrame();
	pMain->GetOutput()->OuputInfo(info);
}
void AppContext::OutputBuild(const CString& info)
{
	CMainFrame* pMain = GetMainFrame();
	pMain->GetOutput()->OuputBuild(info);
}
void AppContext::UpdatePropGrid(engine::GameObjectPtr pObj)
{
	CMainFrame* pMain = GetMainFrame();
	pMain->GetPropGrid()->UpdateGameObjectProp(pObj);
}
