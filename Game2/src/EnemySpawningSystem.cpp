#include "EnemySpawningSystem.h"
#include "Game.h"
#include "Rand.h"

#define MIN_TIME_BETWEEN_ENEMY_SPAWNS 1.0f
#define ENEMY_SPAWN_PROBABILITY 0.3f
#define ENEMY_HALF_SIZE_X 22.0f
#define ENEMY_HALF_SIZE_Y 17.5f
#define ENEMY_VELOCITY_X (-500.0f)

EnemySpawningSystem::EnemySpawningSystem(Game* game)
	:_game(game)
{
	Rand::add_uniform_real_distribution("enemy_spawn_y_pos", 50.0f, _game->get_camera()->top / 2);

	_enemy_sprite = std::make_unique<Mage::Sprite>("res/sprites/enemy.png", 4, 0.05f);
}

void EnemySpawningSystem::update(Mage::ComponentManager& component_manager, float delta_time)
{
	for (auto& e : get_entities())
	{
		auto t = component_manager.get_component<Transform2DComponent>(*e);

		if (t->translation.x < _game->get_camera()->left)
			e->destroy();

		if (e->is_destroyed())
			_enemy_instances.erase(e->get_id());
	}

	_last_spawn += delta_time;

	if (_last_spawn >= MIN_TIME_BETWEEN_ENEMY_SPAWNS && Rand::get_uniform_real() < ENEMY_SPAWN_PROBABILITY)
	{
		_last_spawn = 0.0f;

		auto e = _game->get_entity_manager()->add_entity(EntityTypes::Enemy);
		_enemy_instances[e->get_id()] = std::make_unique<Mage::Sprite>(_enemy_sprite.get());

		component_manager.add_component<EnemyComponent>(*e, {});
		component_manager.add_component<Transform2DComponent>(*e,
			{
				.translation = {
					_game->get_camera()->right,
					Rand::get_uniform_real("enemy_spawn_y_pos")}
			});
		component_manager.add_component<RigidBody2DComponent>(*e,
			{
				.velocity = {ENEMY_VELOCITY_X, 0.0f}
			});
		component_manager.add_component<SpriteComponent>(*e,
			{
				.sprite = _enemy_instances[e->get_id()].get()
			});
		component_manager.add_component<BoundingBoxComponent>(*e,
			{
				.center    = {ENEMY_HALF_SIZE_X, ENEMY_HALF_SIZE_Y},
				.half_size = {ENEMY_HALF_SIZE_X, ENEMY_HALF_SIZE_Y}
			});
	}
}
