#pragma once

#include "core\GameObjectComponent.h"

namespace engine
{
	namespace object_component
	{
		class EXPORT_CLASS LightData : public GameObjectComponent
		{
		public:
			LightData(void);
			virtual ~LightData(void);

			bool										CreateLight(LIGHT_TYPE type);
			void										Update();
			void										SetRenderSystem(RenderSystemPtr pRS);
			LightPtr									GetLight();
			
			static LightDataPtr							CreateComponent();

		private:
			bool										OnAttach();
			void										OnDetach();
		private:

			RenderSystemPtr								m_pRS;
			LightPtr									m_pLight;

		};


	}
}