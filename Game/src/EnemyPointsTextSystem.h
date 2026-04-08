#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class Game;

class EnemyPointsTextSystem : public Mage::System
{
public:
	explicit EnemyPointsTextSystem(Game* game);

	void update(Mage::ComponentManager& component_manager, float delta_time) override;

private:
	Game* _game;
};