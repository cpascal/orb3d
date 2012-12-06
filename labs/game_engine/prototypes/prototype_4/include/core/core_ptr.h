#pragma once

#include <boost\shared_ptr.hpp>
namespace engine
{
	class GameObject;
	class GameObjectManager;
	class GameObjectComponent;
	class GameObjectTemplate;

	namespace object_component
	{
		class MeshData;
		class MeshRenderer;
	}

	typedef boost::shared_ptr<GameObjectTemplate>								GameObjectTemplatePtr;
	typedef boost::shared_ptr<object_component::MeshData>						MeshDataPtr;
	typedef boost::shared_ptr<object_component::MeshRenderer>					MeshRendererPtr;
	typedef boost::shared_ptr<GameObjectManager>								GameObjectManagerPtr;
	typedef boost::shared_ptr<GameObject>										GameObjectPtr;
	typedef boost::shared_ptr<GameObjectComponent>								GameObjectComponentPtr;



	class Sys_Input;
	class Sys_Graphics;
	class Sys_Sound;
	class Sys_Physics;
	class SysManager;


	typedef boost::shared_ptr<SysManager>					SysManagerPtr;

	typedef boost::shared_ptr<Sys_Physics>					Sys_PhysicsPtr;
	typedef boost::shared_ptr<Sys_Input>					Sys_InputPtr;
	typedef boost::shared_ptr<Sys_Graphics>					Sys_GraphicsPtr;
	typedef boost::shared_ptr<Sys_Sound>					Sys_SoundPtr;


	class DataStream;

	
	class Mesh;
	class SubMesh;

	typedef boost::shared_ptr<SubMesh>						SubMeshPtr;
	typedef boost::shared_ptr<DataStream>					DataStreamPtr;
	typedef boost::shared_ptr<Mesh>							MeshPtr;

	class DepthStencilBuffer;
	class RenderData;
	class RenderTarget;
	class Texture;
	class Material;
	class GPUBuffer;
	class MultiRenderTarget;

	typedef boost::shared_ptr<MultiRenderTarget>			MultiRenderTargetPtr;
	typedef boost::shared_ptr<DepthStencilBuffer>			DepthStencilBufferPtr;
	typedef boost::shared_ptr<RenderTarget>					RenderTargetPtr;
	typedef boost::shared_ptr<RenderData>					RenderDataPtr;
	typedef boost::shared_ptr<GPUBuffer>					GPUBufferPtr;
	typedef boost::shared_ptr<Texture>						TexturePtr;
	typedef boost::shared_ptr<Material>						MaterialPtr;


	class CoreApi;
	class RenderSystem;

	typedef boost::shared_ptr<RenderSystem>					RenderSystemPtr;
	typedef boost::shared_ptr<CoreApi>						CoreApiPtr;


	class EventDispatcher;
	class Event;

	typedef boost::shared_ptr<Event>						EventPtr;
	typedef boost::shared_ptr<EventDispatcher>				EventDispatcherPtr;

}
