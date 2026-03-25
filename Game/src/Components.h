#pragma once

#include <Mage/Mage.h>

struct Transform2DComponent
{
	glm::vec2 translation      = { 0.0f, 0.0f };
	glm::vec2 scale            = { 1.0f, 1.0f };
	float rotation		       =          0.0f ;
	glm::vec2 prev_translation = { 0.0f, 0.0f };
};

struct RigidBody2DComponent
{
	glm::vec2 velocity = { 0.0f, 0.0f };
};

struct GravityComponent
{
	glm::vec2 force = { 0.0f, 0.0f };
};

struct SpriteComponent
{
	Mage::Sprite* sprite;
};

struct LifetimeComponent
{
	float remaining = 0.0f;
	std::function<void()> on_destroyed = nullptr;
};

struct BoundingBoxComponent
{
	glm::vec2 center    { 0.5f, 0.5f };
	glm::vec2 half_size { 0.5f, 0.5f };

	std::function<void(Mage::Entity*, Mage::Entity*, const glm::vec2&)> on_collided = nullptr;
};

struct EnemyComponent
{
};

struct DestructionNotificationComponent
{
	std::function<void()> on_destroyed = nullptr;
};