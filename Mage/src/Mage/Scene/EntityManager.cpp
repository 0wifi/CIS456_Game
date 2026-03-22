#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

namespace Mage {

	struct EntityManager::Impl 
	{
		std::mutex sync_object;
		ComponentManager* component_manager;
		SystemManager* system_manager;
		std::vector<std::unique_ptr<Entity>> all_entities;
		std::vector<std::unique_ptr<Entity>> pending_entity_additions;
		std::vector<Entity*> all_entity_ptrs;
		std::map<uint_fast32_t, std::vector<Entity*>> entity_type_map;
		uint64_t last_entity_id = 0;
	};

	EntityManager::EntityManager()
	{
		LOG_E_INFO("EntityManager starting.");
		_impl = new Impl();
	}

	EntityManager::~EntityManager()
	{
		delete _impl;
		LOG_E_INFO("EntityManager stopping.");
	}

	void EntityManager::update()
	{
		//LOG_E_DEBUG("EntityManager update called.");

		//move pending additions into use

		for (auto& e : _impl->pending_entity_additions)
		{
			auto pe = e.get();
			_impl->all_entities.push_back(std::move(e));
			_impl->all_entity_ptrs.push_back(pe);
			_impl->entity_type_map[pe->get_type()].push_back(pe);
		}
		_impl->pending_entity_additions.clear();
		
		//remove pending deletions from use
		
		auto new_end = std::remove_if(_impl->all_entity_ptrs.begin(),
			_impl->all_entity_ptrs.end(),
			[](const Entity* e) { return e->is_destroyed(); });
		_impl->all_entity_ptrs.erase(new_end, _impl->all_entity_ptrs.end());

		for (auto& t : _impl->entity_type_map)
		{
			auto new_end2 = std::remove_if(t.second.begin(), t.second.end(),
				[](const Entity* e) { return e->is_destroyed(); });
			t.second.erase(new_end2, t.second.end());
		}

		for (auto& e : _impl->all_entities)
		{
			if (e->is_destroyed()) {
				_impl->system_manager->entity_destroyed(*e);
				_impl->component_manager->entity_destroyed(*e);
			}
		}

		auto new_end3 = std::remove_if(_impl->all_entities.begin(),
			_impl->all_entities.end(),
			[](const std::unique_ptr<Entity>& e) { return e->is_destroyed(); });
		_impl->all_entities.erase(new_end3, _impl->all_entities.end());
	}

	Entity* EntityManager::add_entity(uint_fast32_t type)
	{
		std::lock_guard lock(_impl->sync_object);
		auto new_id = ++_impl->last_entity_id;
		auto e = std::unique_ptr<Entity>(new Entity(new_id, type));
		auto pe = e.get();
		_impl->pending_entity_additions.push_back(std::move(e));
		LOG_E_DEBUG("Entity added to EntityManager; id: %d; type: %d", new_id, type);
		return pe;
	}

	EntityList EntityManager::get_all_entities() const
	{
		return { _impl->all_entity_ptrs.data(), _impl->all_entities.size() };
	}

	EntityList EntityManager::get_entities_by_type(uint_fast32_t type) const
	{
		return { _impl->entity_type_map[type].data(),
			_impl->entity_type_map[type].size() };
	}

	void EntityManager::set_component_manager(ComponentManager& component_manager)
	{
		_impl->component_manager = &component_manager;
	}

	void EntityManager::set_system_manager(SystemManager& system_manager)
	{
		_impl->system_manager = &system_manager;
	}
}
