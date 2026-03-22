#include "SystemList.h"
#include "SystemManager.h"

namespace Mage {

	struct SystemManager::Impl
	{
		std::mutex sync_object;
		ComponentManager* component_manager;
		std::map<size_t, uint32_t> system_type_index_to_id_map;
		std::map<uint32_t, std::vector<uint32_t>> system_id_to_components_map;
		std::map<uint32_t, System*> system_id_to_system_map;
		std::vector<System*> systems;
		uint32_t last_system_id = 0;
	};

	SystemManager::SystemManager()
	{
		LOG_E_INFO("SystemManager starting.");
		_impl = new Impl();
	}

	SystemManager::~SystemManager()
	{
		LOG_E_INFO("SystemManager stopping.");
		_impl = new Impl();
	}

	SystemList SystemManager::get_all_systems() const
	{
		return { _impl->systems.data(), _impl->systems.size() };
	}

	uint32_t SystemManager::get_system_id(size_t type_idx) const
	{
		return _impl->system_type_index_to_id_map[type_idx];
	}

	ComponentManager& SystemManager::get_component_manager() const
	{
		return *_impl->component_manager;
	}

	void SystemManager::set_component_manager(ComponentManager& component_manager)
	{
		_impl->component_manager = &component_manager;
	}

	bool SystemManager::is_system_registered(const System& system) const
	{
		return std::find(_impl->systems.begin(), _impl->systems.end(), &system)
			!= _impl->systems.end();
	}

	void SystemManager::register_system_internal(System& system, size_t type_idx)
	{
		std::lock_guard lock(_impl->sync_object);
		auto new_id = ++_impl->last_system_id;
		_impl->systems.push_back(&system);
		_impl->system_type_index_to_id_map.insert(std::make_pair(
			type_idx, new_id));
		_impl->system_id_to_system_map.insert(std::make_pair(new_id, &system));
	}

	void SystemManager::add_system_component(uint32_t system_id, uint32_t component_id)
	{
		_impl->system_id_to_components_map[system_id].push_back(component_id);
	}

	void SystemManager::notify_systems_of_entity_component_addition(
		Entity& entity, const uint32_t* component_ids, size_t component_ids_size)
	{
		std::vector entity_components(component_ids, component_ids + component_ids_size);

		//Find any systems that operate on entities with all of the associated component IDs
		//	and if those systems aren't already operating on the entity, add the entity to the system
		for (auto& s : _impl->system_id_to_components_map)
		{
			auto& scids = s.second;

			auto entity_has_all_system_components = std::all_of(
				scids.begin(), scids.end(), [&](uint32_t scid)
				{
					return std::find(entity_components.begin(), entity_components.end(), scid)
						!= entity_components.end();
				});
			
			if (entity_has_all_system_components)
				_impl->system_id_to_system_map[s.first]->add_entity(entity);
		}
	}

	void SystemManager::notify_systems_of_entity_component_removal(
		Entity& entity, uint32_t component_id)
	{
		for (auto s : _impl->system_id_to_system_map)
		{
			auto& scids = _impl->system_id_to_components_map[s.first];
			if (std::find(scids.begin(), scids.end(), component_id) != scids.end())
				s.second->remove_entity(entity);
		}
	}

	void SystemManager::entity_destroyed(Entity& entity)
	{
		for (auto s : _impl->systems)
			s->remove_entity(entity);
	}
}
