#pragma once

#pragma warning(disable : 4100)

#include <Mage/Mage.h>
#include <random>
#include "GravitySystem.h"
#include "MovementSystem.h"
#include "PlayerSystem.h"
#include "LifetimeSystem.h"
#include "DeathByYSystem.h"
#include "CollisionSystem.h"
#include "DestructionNotificationSystem.h"
#include "EnemySpawningSystem.h"
#include "SpriteRenderingSystem.h"

enum EntityTypes : uint_fast32_t
{
	PlayerEntity,
	Platform,
	Bullet
};

class Game final : public Mage::Application
	,	Mage::OnAppClosingEventListener
{
public: 
	Game();

	~Game() override = default;

	void on_app_closing() override;
private:
	std::vector<std::unique_ptr<Mage::Sprite>> _sprites;
	std::unique_ptr<SpriteRenderingSystem> _sprite_rendering_system = nullptr;
	std::unique_ptr<MovementSystem> _movement_system = nullptr;
	std::unique_ptr<GravitySystem>  _gravity_system  = nullptr;
	std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;
	std::unique_ptr<PlayerSystem> _player_system = nullptr;
	std::unique_ptr<CollisionSystem> _collision_system = nullptr;
};