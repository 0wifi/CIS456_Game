#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class Game ;

class CollisionSystem : public Mage::System
{
public:
	explicit CollisionSystem(Game* game);

	static glm::vec2 calculate_overlap(
		const glm::vec2& translation_a, const glm::vec2& scale_a,
		const glm::vec2& translation_b, const glm::vec2& scale_b,
		const BoundingBoxComponent* bounding_box_a,
		const BoundingBoxComponent* bounding_box_b );

	void update(Mage::ComponentManager& component_manager, float delta_time) override;

private:
	Game* _game;
};