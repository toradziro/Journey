#include "jnypch.h"
#include "PhysicSystem.h"
#include "Journey/Scene/Components.h"

namespace jny
{

b2BodyType convertType(BodyType internalType)
{
	switch (internalType)
	{
		case BodyType::Static:
			return b2BodyType::b2_staticBody;
		case BodyType::Dynamic:
			return b2BodyType::b2_dynamicBody;
		case BodyType::Kinematic:
			return b2BodyType::b2_kinematicBody;
		default:
			break;
	}
	JNY_ASSERT(false, "");
	return b2BodyType::b2_staticBody;
}

void PhysicSystem::attach()
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, -10.0f };
	m_worldId = b2CreateWorld(&worldDef);
	m_registry.view<RigidBodyComponent>().each([&](entt::entity e, RigidBodyComponent& rbc)
		{
			auto& tc = m_registry.get<TransformComponent>(e);
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = convertType(rbc.m_bodyType);
			bodyDef.rotation = b2MakeRot(glm::radians(tc.m_rotation.z));
			bodyDef.position = { tc.m_position.x, tc.m_position.y };
			bodyDef.angularDamping = rbc.m_angularDamping;
			bodyDef.angularVelocity = rbc.m_angularVelocity;
			bodyDef.gravityScale = rbc.m_gravityScale;
			bodyDef.linearDamping = rbc.m_linearDamping;
			bodyDef.linearVelocity = { rbc.m_linearVelocity.x, rbc.m_linearVelocity.y };
			bodyDef.allowFastRotation = rbc.m_allowFastRotation;
			bodyDef.fixedRotation = rbc.m_fixedRotation;
			bodyDef.isBullet = rbc.m_isBullet;

			rbc.m_internalBoxId = b2CreateBody(m_worldId, &bodyDef);

			if (m_registry.all_of<BoxColliderComponent>(e))
			{
				auto& bCollider = m_registry.get<BoxColliderComponent>(e);
				b2Polygon box = b2MakeBox(bCollider.m_size * tc.m_scale.x, bCollider.m_size * tc.m_scale.y);
				b2ShapeDef shapeDef = b2DefaultShapeDef();

				shapeDef.density = bCollider.m_density;
				shapeDef.friction = bCollider.m_friction;
				shapeDef.restitution = bCollider.m_restitution;

				b2CreatePolygonShape(rbc.m_internalBoxId, &shapeDef, &box);
			}
		});
}

void PhysicSystem::detach()
{
	b2DestroyWorld(m_worldId);
}

void PhysicSystem::update(f32 dt)
{
	constexpr i32 subStepCount = 4;
	b2World_Step(m_worldId, dt, subStepCount);

	m_registry.view<RigidBodyComponent>().each([&](entt::entity e, RigidBodyComponent& rbc)
		{
			auto& tc = m_registry.get<TransformComponent>(e);

			b2Vec2 position = b2Body_GetPosition(rbc.m_internalBoxId);
			tc.m_position.x = position.x;
			tc.m_position.y = position.y;

			b2Rot rotation = b2Body_GetRotation(rbc.m_internalBoxId);
			f32 rotInRad = b2Rot_GetAngle(rotation);
			f32 rotInDegrees = glm::degrees(rotInRad);

			tc.m_rotation.z = rotInDegrees;
		});
}

}