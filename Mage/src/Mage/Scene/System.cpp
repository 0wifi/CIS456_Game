#include "System.h"

namespace Mage {
	struct System::Impl {
		std::vector<Entity*> entities;
	};

	System::System()
	{
		LOG_E_DEBUG("System created.");
		_impl = new Impl();
	}

	System::~System()
	{
		delete _impl;
		LOG_E_DEBUG("System destroyed.");
	}

	EntityList System::get_entities() const
	{
		return { _impl->entities.data(), _impl->entities.size() };
	}

	void System::add_entity(Entity& entity)
	{
		if (std::find(_impl->entities.begin(), _impl->entities.end(), &entity)
			== _impl->entities.end())
		{
			_impl->entities.push_back(&entity);
		}
	}

	void System::remove_entity(Entity& entity)
	{
		_impl->entities.erase(std::remove(_impl->entities.begin(),
			_impl->entities.end(), &entity), _impl->entities.end());
	}
}
