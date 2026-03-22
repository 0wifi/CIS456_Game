#pragma once

#include "../Core/Core.h"
#include "System.h"
#include "ComponentManager.h"

namespace Mage {

    class SystemList ;

    class MAGE_API SystemManager : ICopyDisable
    {
        friend class Application;
        friend class ComponentManager;
        friend class EntityManager;

    public:
        ~SystemManager();

        template<typename ...Ts>
        void register_system(System& system)
        {
            LOG_E_INFO("System '%s' registered.", typeid(system).name());;
            if (is_system_registered(system))
                throw Exception("Attempting to register a system that has "
                                " already been registered is not allowed.");

			auto type_idx = std::type_index(typeid(system)).hash_code();
            register_system_internal(system, type_idx);

            auto system_id = get_system_id(std::type_index(typeid(system)).hash_code());

            // for each type T in Ts: record component's id as
            //          the id of a component the system cares about
            (void)std::initializer_list<int>{(
                add_system_component(system_id,
                    get_component_manager().get_component_id<Ts>()
                ), 0) ... };
        }

    private:    
        struct Impl;
        Impl* _impl;

        SystemManager();

        //These next 4 are only used by SystemManager and not the friend classes,
        // but still need to be in this header (and not a part of Impl) due to
        // register_system (which compiles in the game) referencing them.

        ComponentManager& get_component_manager() const;

        bool is_system_registered(const System& system) const;

        void register_system_internal(System& system, size_t type_idx);

        void add_system_component(uint_fast32_t system_id, uint_fast32_t component_id);

        ////

        void set_component_manager(ComponentManager& component_manager);

        SystemList get_all_systems() const;

        uint_fast32_t get_system_id(size_t type_idx) const;

        void notify_systems_of_entity_component_addition(
            Entity& entity, const uint32_t* component_ids, size_t component_ids_size);

        void notify_systems_of_entity_component_removal(
            Entity& entity, uint32_t component_id);

        void entity_destroyed(Entity& entity);
    };
}
