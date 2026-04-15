#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class SpriteRenderingSystem final : public Mage::System
{

private:
	Mage::SpriteRenderer* _sprite_renderer;

public:
	explicit SpriteRenderingSystem(Mage::SpriteRenderer& sprite_renderer)
		: _sprite_renderer(&sprite_renderer)
	{
	}

	void update(Mage::ComponentManager& component_manager, float delta_time) override
	{
		for (auto e : get_entities())
		{
			if (e->is_destroyed())
				continue;

			auto s = component_manager.get_component<SpriteComponent>(*e);
			auto t = component_manager.get_component<Transform2DComponent>(*e);

			if (s->sprite != nullptr)
				_sprite_renderer->render(*s->sprite, t->translation, t->scale, t->rotation, delta_time);
		}
	}
};