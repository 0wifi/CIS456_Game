#pragma once

#include <random>
#include <Mage/Mage.h>
#include "Components.h"

class Game ;

class EnemySpawningSystem final : public Mage::System
{
public:
	explicit EnemySpawningSystem(Game* game);

	void update(Mage::ComponentManager& component_manager, float delta_time) override;

private: 
	Game* _game = nullptr;
	float _last_spawn = 0.0f;
	std::unique_ptr<Mage::Sprite> _enemy_sprite;
	std::map<uint64_t, std::unique_ptr<Mage::Sprite>> _enemy_instances;
};