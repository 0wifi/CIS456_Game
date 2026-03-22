#include "CollisionSystem.h"
#include "Game.h"

CollisionSystem::CollisionSystem(Game* game)
	: _game(game)
{
}

glm::vec2 CollisionSystem::calculate_overlap(const glm::vec2& translation_a, const glm::vec2& scale_a, const glm::vec2& translation_b, const glm::vec2& scale_b, const BoundingBoxComponent* bounding_box_a, const BoundingBoxComponent* bounding_box_b)
{
	auto x1 = (bounding_box_a->center.x * scale_a.x) + translation_a.x;
	auto x2 = (bounding_box_b->center.x * scale_b.x) + translation_b.x;
	auto y1 = (bounding_box_a->center.y * scale_a.y) + translation_a.y;
	auto y2 = (bounding_box_b->center.y * scale_b.y) + translation_b.y;

	auto delta_x = std::abs(x1 - x2);
	auto delta_y = std::abs(y1 - y2);

	auto ol_x = (bounding_box_a->half_size.x * scale_a.x)
		      + (bounding_box_b->half_size.x * scale_b.x) - delta_x;
	auto ol_y = (bounding_box_a->half_size.y * scale_a.y)
		      + (bounding_box_b->half_size.y * scale_b.y) - delta_y;

	return { ol_x, ol_y };
}

void CollisionSystem::update(Mage::ComponentManager& component_manager, float delta_time)
{
	for (auto e1 : get_entities())
	{
		if (e1->is_destroyed())
			continue;

		for (auto e2 : get_entities())
		{
			if (e2->is_destroyed() || e1->get_id() == e2->get_id())
				continue;

			auto bb1 = component_manager.get_component<BoundingBoxComponent>(*e1);
			auto t1 = component_manager.get_component<Transform2DComponent>(*e1);
			auto bb2 = component_manager.get_component<BoundingBoxComponent>(*e2);
			auto t2 = component_manager.get_component<Transform2DComponent>(*e2);

			auto ol = calculate_overlap(t1->translation, t1->scale, t2->translation, t2->scale, bb1, bb2);

			if (ol.x > 0.0f && ol.y > 0.0f)
			{
				if (bb1->on_collided != nullptr)
					bb1->on_collided(e1, e2, ol);
				if (bb2->on_collided != nullptr)
					bb2->on_collided(e2, e1, ol);
			}
		}
	}
}

