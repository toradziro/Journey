#include "jnypch.h"
#include "Components.h"
#include "Journey/Renderer/Camera.h"
#include "Journey/ImGui/Controls/DropDownList.h"
#include "Journey/ResourceManagers/TextureManager.h"
#include "Journey/Core/Application.h"
#include "imgui.h"

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

	//-- Sprite Component
	entt::meta<SpriteComponent>()
		.type(entt::hashed_string(SpriteComponent::C_COMPONENT_NAME)).prop(C_PROP_REMOVABLE)
		.data<&SpriteComponent::m_color>(entt::hashed_string("Color")).prop(C_PROP_NAME_HS, "Color")
		.data<&SpriteComponent::m_texture>(entt::hashed_string("Texture")).prop(C_PROP_NAME_HS, "Texture")
			.prop(C_CASTOM_UI_DRAW,
				std::function<void(SpriteComponent&, entt::entity)>([](SpriteComponent& component, entt::entity e)
				{
					u32 currSelectedTexture = DropDownList::C_INVALID_INDEX;

					auto& textureManager = Application::subsystems().st<TextureManager>();
					const auto& textureAssetsPaths = textureManager.allTexturesOnDisk();
					if (component.m_texture)
					{
						for (u32 i = 0; i < textureAssetsPaths.size(); ++i)
						{
							if (textureAssetsPaths[i] == component.m_texture->path())
							{
								//-- Plus one needs to handle no texture
								currSelectedTexture = i + 1;
								break;
							}
						}
					}

					std::vector<std::string> pathsAsStrs;
					pathsAsStrs.reserve(textureAssetsPaths.size() + 1);
					//-- If we want plain squad with no texture
					pathsAsStrs.push_back("None");
					for (const auto& path : textureAssetsPaths)
					{
						pathsAsStrs.emplace_back(path.generic_string());
					}

					std::string label = fmt::format("##textureSelector{}", static_cast<u32>(e));
					ImVec2 cursorPos = ImGui::GetCursorPos();
					if (DropDownList(pathsAsStrs, label, currSelectedTexture).draw())
					{
						if (currSelectedTexture == 0)
						{
							component.m_texture = {};
						}
						else
						{
							component.m_texture = textureManager.create
							(
								//-- So here we need to sub that one
								textureAssetsPaths[currSelectedTexture - 1].generic_string()
							);
						}
					}
					constexpr ImVec2 C_DRAG_AND_DROP_TARGET_SIZE = { 300.0f, 20.0f };
					const ImVec2 ddSizeWithDpi = {
						C_DRAG_AND_DROP_TARGET_SIZE.x * ImGui::GetWindowDpiScale(),
						C_DRAG_AND_DROP_TARGET_SIZE.y * ImGui::GetWindowDpiScale()
					};
					ImGui::SetCursorPos(cursorPos);
					ImGui::InvisibleButton("##dragAndDropTarget", ddSizeWithDpi);
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
						{
							fs_path path(std::string(static_cast<const char*>(payload->Data), payload->DataSize));
							if (path.extension() == ".png")
							{
								component.m_texture = textureManager.create(path.generic_string());
							}
						}
						ImGui::EndDragDropTarget();
					}
				}));
	
	//-- EntityNameComponent
	entt::meta<EntityNameComponent>()
		.type(entt::hashed_string(EntityNameComponent::C_COMPONENT_NAME))
		.data<&EntityNameComponent::m_name>(entt::hashed_string("Name")).prop(C_PROP_NAME_HS, "Name");
	
	//-- CameraComponent
	entt::meta<CameraComponent>()
		.type(entt::hashed_string(CameraComponent::C_COMPONENT_NAME)).prop(C_PROP_REMOVABLE)
		.data<&CameraComponent::m_primer>(entt::hashed_string("isMainCamera")).prop(C_PROP_NAME_HS, "Main Camera")
		.data<&CameraComponent::m_perspective>(entt::hashed_string("isPerspective")).prop(C_PROP_NAME_HS, "Is Perspective")
		.data<&CameraComponent::m_zoom>(entt::hashed_string("cameraZoom"))
			.prop(C_PROP_NAME_HS, "Ortho Distance")
			.prop(C_ON_PROP_CHANGE_HS, std::function<void(CameraComponent&, f32)>([](CameraComponent& component, f32 val)
				{
					component.m_zoom = val;
					component.m_camera.setZoom(val);
				}))
		.data<&CameraComponent::m_fov>(entt::hashed_string("cameraFov"))
			.prop(C_PROP_NAME_HS, "Perspective FOV")
			.prop(C_ON_PROP_CHANGE_HS, std::function<void(CameraComponent&, f32)>([](CameraComponent& component, f32 val)
				{
					component.m_fov = val;
					component.m_perspectiveCamera.setFov(val);
				}));

	//-- Main hero example
	entt::meta<MainHeroComponent>()
		.type(entt::hashed_string(MainHeroComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "Main Hero")
		.prop(C_PROP_REMOVABLE)
		.data<&MainHeroComponent::m_movementSpeed>(entt::hashed_string("moveSpeed")).prop(C_PROP_NAME_HS, "Move Speed");

	//-- Physic come here
	entt::meta<RigidBodyComponent>()
		.type(entt::hashed_string(RigidBodyComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "Rigid Body")
		.prop(C_PROP_REMOVABLE)
		.data<&RigidBodyComponent::m_bodyType>(entt::hashed_string("m_bodyType")).prop(C_PROP_NAME_HS, "Body Type")
			.prop(C_CASTOM_UI_DRAW,
				std::function<void(RigidBodyComponent&, entt::entity)>([](RigidBodyComponent& component, entt::entity e)
				{
					std::vector<std::string> bodyTypes = { "Static", "Dynamic", "Kinematic" };
					std::string label = fmt::format("##RigidBodyType{}", static_cast<u32>(e));
					u32 currSelected = static_cast<u32>(component.m_bodyType);
					if (DropDownList(bodyTypes, label, currSelected).draw())
					{
						component.m_bodyType = static_cast<BodyType>(currSelected);
					}
				}))
		.data<&RigidBodyComponent::m_angularDamping>(entt::hashed_string("m_angularDamping")).prop(C_PROP_NAME_HS, "Angular Damping")
		.data<&RigidBodyComponent::m_angularVelocity>(entt::hashed_string("m_angularVelocity")).prop(C_PROP_NAME_HS, "Angular Velocity")
		.data<&RigidBodyComponent::m_gravityScale>(entt::hashed_string("m_gravityScale")).prop(C_PROP_NAME_HS, "Gravity Scale")
		.data<&RigidBodyComponent::m_linearDamping>(entt::hashed_string("m_linearDamping")).prop(C_PROP_NAME_HS, "Linear Damping")
		.data<&RigidBodyComponent::m_linearVelocity>(entt::hashed_string("m_linearVelocity")).prop(C_PROP_NAME_HS, "Linear Velocity")
		.data<&RigidBodyComponent::m_allowFastRotation>(entt::hashed_string("m_allowFastRotation")).prop(C_PROP_NAME_HS, "Allow Fast Rotation")
		.data<&RigidBodyComponent::m_fixedRotation>(entt::hashed_string("m_fixedRotation")).prop(C_PROP_NAME_HS, "Fixed Rotation")
		.data<&RigidBodyComponent::m_isBullet>(entt::hashed_string("m_isBullet")).prop(C_PROP_NAME_HS, "Is Bullet");

	entt::meta<BoxColliderComponent>()
		.type(entt::hashed_string(BoxColliderComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "Box Collider")
		.prop(C_PROP_REMOVABLE)
		.data<&BoxColliderComponent::m_size>(entt::hashed_string("m_size")).prop(C_PROP_NAME_HS, "Size")
		.data<&BoxColliderComponent::m_density>(entt::hashed_string("m_density")).prop(C_PROP_NAME_HS, "Density")
		.data<&BoxColliderComponent::m_friction>(entt::hashed_string("m_friction")).prop(C_PROP_NAME_HS, "Friction")
		.data<&BoxColliderComponent::m_restitution>(entt::hashed_string("m_restitution")).prop(C_PROP_NAME_HS, "Restitution");

	// CircleColliderComponent
	entt::meta<CircleColliderComponent>()
		.type(entt::hashed_string(CircleColliderComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "Circle Collider")
		.prop(C_PROP_REMOVABLE)
		.data<&CircleColliderComponent::m_radius>(entt::hashed_string("m_radius")).prop(C_PROP_NAME_HS, "Radius")
		.data<&CircleColliderComponent::m_density>(entt::hashed_string("m_density")).prop(C_PROP_NAME_HS, "Density")
		.data<&CircleColliderComponent::m_friction>(entt::hashed_string("m_friction")).prop(C_PROP_NAME_HS, "Friction")
		.data<&CircleColliderComponent::m_restitution>(entt::hashed_string("m_restitution")).prop(C_PROP_NAME_HS, "Restitution");

	// UuidComponent
	entt::meta<UuidComponent>()
		.type(entt::hashed_string(UuidComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "UUID")
		.prop(C_PROP_REMOVABLE)
		.data<&UuidComponent::m_uuid>(entt::hashed_string("m_uuid")).prop(C_PROP_NAME_HS, "UUID")
		.prop(C_CASTOM_UI_DRAW,
			std::function<void(UuidComponent&, entt::entity)>([](UuidComponent& component, entt::entity e)
			{
				ImGui::Text("%llu", (u64)component.m_uuid);
			}));

	// CircleComponent
	entt::meta<CircleComponent>()
		.type(entt::hashed_string(CircleComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "Circle")
		.prop(C_PROP_REMOVABLE)
		//.data<&CircleComponent::m_radius>(entt::hashed_string("m_radius")).prop(C_PROP_NAME_HS, "Radius")
		.data<&CircleComponent::m_color>(entt::hashed_string("m_color")).prop(C_PROP_NAME_HS, "Color")
		.data<&CircleComponent::m_edgeThikness>(entt::hashed_string("m_edgeThikness")).prop(C_PROP_NAME_HS, "Edge Thikness");

	// Line
	entt::meta<LineComponent>()
		.type(entt::hashed_string(LineComponent::C_COMPONENT_NAME))
		.prop(C_PROP_NAME_HS, "Line")
		.prop(C_PROP_REMOVABLE)
		.data<&LineComponent::m_color>(entt::hashed_string("m_color")).prop(C_PROP_NAME_HS, "Color")
		.data<&LineComponent::m_startPoint>(entt::hashed_string("m_startPoint")).prop(C_PROP_NAME_HS, "Start Point")
		.data<&LineComponent::m_endPoint>(entt::hashed_string("m_endPoint")).prop(C_PROP_NAME_HS, "End Point");
}

} //-- jny