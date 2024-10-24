#include "jnypch.h"
#include "Components.h"
#include "Journey/Renderer/Camera.h"

#include <entt.hpp>

namespace jny
{

/*
struct MyType
{
	int value;
	void func() { ... }
};

meta::reflect<MyType>(meta::hash("MyType"))
	.data<&MyType::value>(meta::hash("value"))
	.func<&MyType::func>(meta::hash("func"));

//-- Types usage example
auto myTypeMeta = meta::resolve<MyType>();
auto myTypeMetaByName = meta::resolve(meta::hash("MyType"));

//-- Create instance by constructor
auto ctor = myTypeMeta.ctor<int>();
meta::any instance = ctor.invoke(42);

//-- Containers example
std::vector<int> vec{1, 2, 3};
meta::any any = meta::forward_as_meta(vec);

if(any.type().is_sequence_container()) {
	auto view = any.as_sequence_container();
	for(meta::any element : view) {
	}
}
*/

void registerComponents()
{
	//-- We need some glm types to be also reflected
	entt::meta<glm::vec3>()
		.data<&glm::vec3::x>(entt::hashed_string("x")).prop(C_PROP_NAME_HS, "x")
		.data<&glm::vec3::y>(entt::hashed_string("y")).prop(C_PROP_NAME_HS, "y")
		.data<&glm::vec3::z>(entt::hashed_string("z")).prop(C_PROP_NAME_HS, "z");

	entt::meta<glm::vec4>()
		.data<&glm::vec4::r>(entt::hashed_string("r")).prop(C_PROP_NAME_HS, "r")
		.data<&glm::vec4::g>(entt::hashed_string("g")).prop(C_PROP_NAME_HS, "g")
		.data<&glm::vec4::b>(entt::hashed_string("b")).prop(C_PROP_NAME_HS, "b")
		.data<&glm::vec4::a>(entt::hashed_string("a")).prop(C_PROP_NAME_HS, "a");
	//-- Add reflection of components
	//-- Transform component
	entt::meta<TransformComponent>()
		.type(entt::hashed_string(TransformComponent::C_COMPONENT_NAME))
		.data<&TransformComponent::m_position>(entt::hashed_string("Position"))
			.prop(C_PROP_NAME_HS, "Position")
		.data<&TransformComponent::m_scale>(entt::hashed_string("Scale")).prop(C_PROP_NAME_HS, "Scale")
		.data<&TransformComponent::m_rotation>(entt::hashed_string("Rotation")).prop(C_PROP_NAME_HS, "Rotation");
	//-- SpriteComponent
	entt::meta<SpriteComponent>()
		.type(entt::hashed_string(SpriteComponent::C_COMPONENT_NAME))
		.data<&SpriteComponent::m_color>(entt::hashed_string("Color")).prop(C_PROP_NAME_HS, "Color");
		//.data<&SpriteComponent::m_texture>;
	//-- EntityNameComponent
	entt::meta<EntityNameComponent>()
		.type(entt::hashed_string(EntityNameComponent::C_COMPONENT_NAME))
		.data<&EntityNameComponent::m_name>(entt::hashed_string("Name")).prop(C_PROP_NAME_HS, "Name");
	//-- CameraComponent
	entt::meta<CameraComponent>()
		.type(entt::hashed_string(CameraComponent::C_COMPONENT_NAME))
		.data<&CameraComponent::m_primer>(entt::hashed_string("isMainCamera")).prop(C_PROP_NAME_HS, "Main Camera")
		.data<&CameraComponent::m_zoom>(entt::hashed_string("cameraZoom"))
			.prop(C_PROP_NAME_HS, "Distance")
			.prop(C_ON_PROP_CHANGE_HS, std::function<void(CameraComponent&, f32)>([](CameraComponent& component, f32 val)
				{
					component.m_camera.setZoom(val);
				}));
	//-- NativeScriptComponent
	entt::meta<NativeScriptComponent>()
		.type(entt::hashed_string(NativeScriptComponent::C_COMPONENT_NAME));
}

} //-- jny