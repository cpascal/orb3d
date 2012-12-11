#pragma once
#include "core\viewfrustum.h"

namespace engine
{
	class LightManager
	{
	public:
		LightManager(void);
		virtual ~LightManager(void);

		bool									Initialize();
		void									Release();

		void									AddLight(LightPtr pLight);
		void									RemoveLight(LightPtr pLight);
		int										GetLightCount();
		
		LightPtr								GetNextLight(LightPtr pLight);

		LightPtr								GetNextAffectingLight(LightPtr pLight, const ViewFrustum& frustum);

	private:

		LightPtr								m_pList;

		int										m_lightCount;

	};
}