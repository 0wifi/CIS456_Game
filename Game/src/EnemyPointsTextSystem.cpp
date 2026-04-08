#include "EnemyPointsTextSystem.h"
#include "Game.h"

EnemyPointsTextSystem::EnemyPointsTextSystem(Game* game)
	: _game(game)
{
}

void EnemyPointsTextSystem::update(Mage::ComponentManager& component_manager, float delta_time)
{
	for (auto e : get_entities())
	{
		if (e->is_destroyed())
			continue;

		auto p = component_manager.get_component<EnemyPointsTextComponent>(*e);
		auto t = component_manager.get_component<Transform2DComponent>(*e);

		_game->get_text_renderer()->render_text(*_game->get_font(), p->msg.c_str(),
			t->translation.x, t->translation.y, 1.0f, Mage::Color::white);
	}
}

