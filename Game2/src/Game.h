#pragma once

#pragma warning(disable : 4100)

#include <Mage/Mage.h>
#include <random>
#include "GravitySystem.h"
#include "MovementSystem.h"
#include "PlayerSystem.h"
#include "LifetimeSystem.h"
#include "CollisionSystem.h"
#include "DestructionNotificationSystem.h"
#include "EnemySpawningSystem.h"
#include "SpriteRenderingSystem.h"
#include "EnemyPointsTextSystem.h"

enum EntityTypes : uint_fast32_t
{
	PlayerEntity,
	Platform,
	Bullet,
	Enemy,
	Explosion,
	EnemyPointsText
};

class Game final : public Mage::Application
	,	Mage::OnAppClosingEventListener
{
public: 
	Game();

	~Game() override = default;

	void on_app_closing() override;

	Mage::Font* get_font() const;
private:
	std::map<std::string, std::unique_ptr<Mage::Sprite>> _sprites;
	std::unique_ptr<SpriteRenderingSystem> _sprite_rendering_system = nullptr;
	std::unique_ptr<MovementSystem> _movement_system = nullptr;
	std::unique_ptr<GravitySystem>  _gravity_system  = nullptr;
	std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;
	std::unique_ptr<PlayerSystem> _player_system = nullptr;
	std::unique_ptr<CollisionSystem> _collision_system = nullptr;
	std::unique_ptr<EnemySpawningSystem> _enemy_spawning_system = nullptr;
	std::unique_ptr<DestructionNotificationSystem> _destruction_notification_system = nullptr;
	std::unique_ptr<EnemyPointsTextSystem> _enemy_points_text_system = nullptr;
	std::unique_ptr<Mage::Font> _font = nullptr;

	void add_platform(float pos_x, size_t units_wide);
};