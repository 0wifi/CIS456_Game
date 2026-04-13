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

		auto msg = p->msg.c_str();
		auto msg_s = p->msg_scale;
	
		auto tw = _game->get_font()->measure_text(msg, msg_s);
		auto left = t->translation.x - _game->get_camera()->left - (tw / 2.0f);
		auto bot = _game->get_camera()->top - t->translation.y;

		_game->get_text_renderer()->render_text(*_game->get_font(), msg,
			left, bot, msg_s, Mage::Color::custom(0.0f, 1.0f, 0.0f, p->current_alpha));
	}
}

