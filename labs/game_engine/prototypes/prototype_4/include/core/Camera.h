#pragma once
#include "core\GameObjectComponent.h"

namespace engine
{
	namespace object_component
	{
		class Camera : public GameObjectComponent
		{
		public:
			Camera(void);
			virtual ~Camera(void);
		};


	}
}
