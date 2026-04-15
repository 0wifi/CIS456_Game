#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class LifetimeSystem final : public Mage::System
{
public:
	void update(Mage::ComponentManager& component_manager, float delta_time) override
	{
		for (auto& e : get_entities())
		{
			if (e->is_destroyed())
				continue;

			auto l = component_manager.get_component<LifetimeComponent>(*e);

			l->remaining -= delta_time;

			if (l->remaining <= 0.0f)
			{
				e->destroy();

				if (l->on_destroyed != nullptr)
					l->on_destroyed();
			}
		}
	}
};