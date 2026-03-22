#pragma once

#include "EntityList.h"
#include "../Core/Core.h"

namespace Mage {
	
    class ComponentManager ;

	class MAGE_API System : ICopyDisable
	{
		friend class SystemManager;

	public:
		System();

		virtual ~System();

		virtual void update(ComponentManager& component_manager, float delta_time) = 0;

		EntityList get_entities() const;
	
	private:
		struct Impl; 
		Impl* _impl;

		void add_entity(Entity& entity);

		void remove_entity(Entity& entity);
	};
}