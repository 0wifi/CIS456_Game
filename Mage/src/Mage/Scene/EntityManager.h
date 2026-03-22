#pragma once

#include "../Core/Core.h"
#include "Entity.h"
#include "EntityList.h"

namespace Mage {

    class SystemManager ;
    class ComponentManager ;

    class MAGE_API EntityManager : ICopyDisable
    {
        friend class Application;

    public:
        ~EntityManager();

        void update();

        Entity* add_entity(uint_fast32_t type);

        EntityList get_all_entities() const;

        EntityList get_entities_by_type(uint_fast32_t type) const;
    
    private:
        struct Impl;
        Impl* _impl;

        EntityManager();

        void set_component_manager(ComponentManager& component_manager);

        void set_system_manager(SystemManager& system_manager);
    };
}
