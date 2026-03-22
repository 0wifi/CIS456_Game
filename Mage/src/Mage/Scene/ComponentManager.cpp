#include "ComponentManager.h"
#include "SystemManager.h"

namespace Mage {

    struct ComponentManager::Impl
    {
        std::mutex sync_object;
        SystemManager* system_manager;
        std::unordered_map<size_t, uint_fast32_t> component_type_to_id_map;
        std::unordered_map<uint_fast32_t, ComponentVectorBase*> component_vectors;
        uint_fast32_t last_component_id = 0;
    };

    ComponentManager::ComponentManager()
    {
        LOG_E_INFO("ComponentManager starting.");
        _impl = new Impl();
    }

    ComponentManager::~ComponentManager()
    {
        LOG_E_INFO("ComponentManager stopping.");
        for (auto& cv : _impl->component_vectors)
            delete cv.second;
        delete _impl;
    } 

    void ComponentManager::set_system_manager(SystemManager& system_manager)
    {
        _impl->system_manager = &system_manager;
    }

    void ComponentManager::register_component(size_t type_idx, ComponentVectorBase* component_vector)
    {
        std::lock_guard lock(_impl->sync_object);
        auto new_id = ++_impl->last_component_id;
        _impl->component_type_to_id_map.insert(
            std::make_pair(type_idx, new_id));
        _impl->component_vectors.insert(
            std::make_pair(new_id, component_vector));
    }

    ComponentVectorBase* ComponentManager::get_component_vector(uint_fast32_t component_id) const
    {
        return _impl->component_vectors[component_id];
    }

    bool ComponentManager::get_component_id(size_t type_idx, uint_fast32_t* component_id) const
    {
        auto it = _impl->component_type_to_id_map.find(type_idx);
        if (it == _impl->component_type_to_id_map.end())
            return false;
        *component_id = it->second;
        return true;
    }

    void ComponentManager::notify_systems_of_entity_component_addition(Entity& entity, uint_fast32_t component_id) const
    {
        std::vector<uint_fast32_t> entity_component_ids;
        
        entity_component_ids.push_back(component_id);
        
        for (auto& cv : _impl->component_vectors)
        {
            auto entities = cv.second->get_entities();
            if (std::find(entities.begin(), entities.end(), &entity)
                != entities.end())
                entity_component_ids.push_back(cv.first);
        }

        _impl->system_manager->notify_systems_of_entity_component_addition(entity,
            entity_component_ids.data(), entity_component_ids.size());
    }

    void ComponentManager::notify_systems_of_entity_component_removal(
        Entity& entity, uint_fast32_t component_id) const
    {
        _impl->system_manager->notify_systems_of_entity_component_removal(
            entity, component_id);
    }

    void ComponentManager::entity_destroyed(Entity& entity)
    {
        for (auto& cv : _impl->component_vectors)
            cv.second->entity_destroyed(entity);
    }
}
