#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class DestructionNotificationSystem final : public Mage::System
{
public:
	void update(Mage::ComponentManager& component_manager, float delta_time) override
	{
		for (auto e : get_entities())
		{
			if (e->is_destroyed())
			{
				auto d = component_manager.get_component<DestructionNotificationComponent>(*e);
				if (d->on_destroyed != nullptr)
					d->on_destroyed();
			}
		}
	}
};