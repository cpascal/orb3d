#include "game_pch.h"
#include "Game.h"



EXPORT_C_API Game* CreateGame()
{
	return new Game;
}

EXPORT_C_API void DestroyGame(engine::GameInterface* pGame)
{
	delete (Game*)pGame;
}



Game::Game(void)
{

}


Game::~Game(void)
{
}
bool Game::Initialize(engine::CoreApiPtr pCore)
{
	using namespace engine;

	m_pCore = pCore;


	m_pTex = m_pCore->GetSysGraphics()->CreateTextureFromFile("./15.png");

	/*math::Vector3 verts[] = 
	{
		math::Vector3(0, 50, 0),
		math::Vector3(50, 0, 0),
		math::Vector3(0, 0, 0),
	};
*/


	struct Vertex
	{
		math::Vector3			pos;
		math::Vector2			uv;
	};

	Vertex verts[] = 
	{
		{ math::Vector3(10, 10, -10), math::Vector2(1, 0),},
		{ math::Vector3(10, -10, -10), math::Vector2(1, 1),},
		{ math::Vector3(-10, -10, -10), math::Vector2(0, 1),},
		{ math::Vector3(-10, 10, -10), math::Vector2(0, 0),},


		{ math::Vector3(10, 10, 10), math::Vector2(0, 1),},
		{ math::Vector3(10, -10, 10),math::Vector2(1, 1),},
		{ math::Vector3(-10, -10, 10),math::Vector2(0, 1),},
		{ math::Vector3(-10, 10, 10),math::Vector2(0, 0),},
	};

	m_pVB = m_pCore->GetSysGraphics()->CreateBuffer(Sys_Graphics::BT_VERTEX_BUFFER, sizeof(Vertex) * 8, verts, true);

	
	uint32 indice[] = 
	{
		0, 1, 2,
		0, 2, 3,

		4, 6, 5,
		4, 7, 6,

		0, 3, 4,
		4, 3, 7,

		1, 5, 6,
		1, 6, 2,

		0, 4, 5,
		0, 5, 1,

		3, 2, 6,
		3, 6, 7,
	};

	m_pIB = m_pCore->GetSysGraphics()->CreateBuffer(Sys_Graphics::BT_INDEX_BUFFER, sizeof(uint32) * 36, indice, true);

	

	m_pGFX = m_pCore->GetSysGraphics()->CreateGFXFromFile("./assets/gfx/test.cgfx");

	VertexElement vf[] = 
	{
		VertexElement(0, VertexElement::POSITION,VertexElement::VE_FLOAT3),
		VertexElement(0, VertexElement::TEXCOORD,VertexElement::VE_FLOAT2),
	};

	m_pGFX->SetVertexFormat(vf, 1);


	m_pGFX->SetTextureByName("diff", m_pTex);
	return true;
}
void Game::Release()
{
	m_pIB->Release();
	m_pVB->Release();
	m_pGFX->Release();


}
bool Game::Update()
{


	using namespace engine;

	static float rad = 0;
	rad += 0.005f;

	math::Vector3 eye(0, 20, -30);

	math::Matrix44 mat = math::MatrixRotationAxisY(rad);

	math::TransformCoord(eye, mat);


	math::Matrix44 view = math::MatrixLookAtLH(eye, math::Vector3(0, 0, 0), math::Vector3(0, 1, 0));
	math::Matrix44 proj = math::MatrixPerspectiveFovLH(1.0f/2.0f * 3.14f, 4.0f/ 3.0f, 0.0001f, 10000.0f);

	m_pGFX->SetMatrixBySemantic("WORLDVIEWPROJ", view * proj);


	m_pCore->GetSysGraphics()->ClearRenderTarget();


	m_pCore->GetSysGraphics()->SetIndexBuffer(m_pIB, Sys_Graphics::IT_INT32);
	m_pCore->GetSysGraphics()->SetVertexBuffer(m_pVB, 0, sizeof(math::Vector3) + sizeof(math::Vector2));
	m_pCore->GetSysGraphics()->SetPrimitiveType(Sys_Graphics::PT_TRIANGLE_LIST);

	m_pGFX->ApplyVertexFormat();


	while(m_pGFX->BeginPass())
	{
		m_pGFX->ApplyPass();

		m_pCore->GetSysGraphics()->DrawPrimitive(36, 0, 0);

		m_pGFX->EndPass();
	}

	m_pCore->GetSysGraphics()->Present();


	return true;
}