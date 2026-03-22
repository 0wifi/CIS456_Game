#pragma once

#include "../Core/Core.h"
#include "EntityList.h"

namespace Mage {

	class MAGE_API ComponentVectorBase : ICopyDisable
	{
	public: 
		ComponentVectorBase() = default;
		
		virtual ~ComponentVectorBase() = default;

		virtual void entity_destroyed(Entity& entity) = 0;

		virtual EntityList get_entities() = 0;
	};

	template<typename T>
	class ComponentVector final : public ComponentVectorBase
	{
	public:
		ComponentVector() = default;

		~ComponentVector() override = default;

		T* get(uint64_t entity_id)
		{
			auto it = _entity_to_index_map.find(entity_id);

			if (it == _entity_to_index_map.end())
			{
				LOG_E_WARN("Component retrieval from entity which didn't have that component;"
					" eid: %d", entity_id);
				return nullptr;
			}

			return &_components.at(it->second);
		}

		void add(Entity& entity, T component)
		{
			auto eid = entity.get_id();

			if (_entity_to_index_map.contains(eid))
				throw Exception("Attempting to add a component to an entity "
					"that already has that component is not allowed.");

			_entities.push_back(&entity);
			_components.push_back(component);

			_entity_to_index_map.insert_or_assign(eid, _components.size() - 1);
			_index_to_entity_map.insert_or_assign(_components.size() - 1, eid);

		}

		void remove(const Entity& entity)
		{
			auto eid = entity.get_id();

			if (!_entity_to_index_map.contains(eid))
				return;

			//move the last item in the vector to the place where the one being removed is
			auto idx_of_removed_component = _entity_to_index_map.at(eid);
			auto idx_of_last_component = _components.size() - 1;
			_components[idx_of_removed_component] = _components.at(idx_of_last_component);

			//now, idx of removed is really the index of what was the last component in the vector
			auto idx_of_arbitrary_other_component = idx_of_removed_component;

			//update the maps to reflect the old last component's new position
			auto entity_id_of_arbitrary_other_component = _index_to_entity_map.at(idx_of_last_component);
			_entity_to_index_map[entity_id_of_arbitrary_other_component] = idx_of_arbitrary_other_component;
			_index_to_entity_map[idx_of_arbitrary_other_component] = entity_id_of_arbitrary_other_component;

			//remove the now incorrect map entries
			_entity_to_index_map.erase(eid);
			_index_to_entity_map.erase(idx_of_last_component);
			
			//remove the last component in the vector
			_components.erase(_components.end() - 1, _components.end());

			//optionally, create a second map to get O(1) vs. O(n) on this vector
			_entities.erase(std::remove(_entities.begin(), _entities.end(), &entity),
				_entities.end());
		}

		void entity_destroyed(Entity& entity) override
		{
			remove(entity);
		}

		EntityList get_entities() override
		{
			return { _entities.data(), _entities.size() };
		}


	private:
		// These are ok because this is a class template,
		// so will be compiled when the game is compiled
		// and use whatever standard C++ library version they have
		std::vector<T> _components;
		std::vector<Entity*> _entities;
		std::unordered_map<uint64_t, uint64_t> _entity_to_index_map;
		std::unordered_map<uint64_t, uint64_t> _index_to_entity_map;
	};

}
