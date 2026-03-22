#include "Game.h"
#include "Components.h"
#include "Rand.h"

Game::Game() : Application("Game", false, 1024, 768, 1)
{
	Mage::Log::get().set_engine_log_level(Mage::Log::Level::Debug);

	get_event_manager()->add_on_app_closing_event_listener(this);

	get_component_manager()->register_component<Transform2DComponent>();
	get_component_manager()->register_component<RigidBody2DComponent>();
	get_component_manager()->register_component<GravityComponent>();
	get_component_manager()->register_component<SpriteComponent>();
	get_component_manager()->register_component<LifetimeComponent>();
	get_component_manager()->register_component<BoundingBoxComponent>();

	_sprite_rendering_system = std::make_unique<SpriteRenderingSystem>(*get_sprite_renderer());
	_gravity_system = std::make_unique<GravitySystem>();
	_movement_system = std::make_unique<MovementSystem>();
	_lifetime_system = std::make_unique<LifetimeSystem>();
	_player_system = std::make_unique<PlayerSystem>(this);
	_collision_system = std::make_unique<CollisionSystem>(this);

	get_system_manager()->register_system<SpriteComponent, Transform2DComponent>(*_sprite_rendering_system);
	get_system_manager()->register_system<RigidBody2DComponent, GravityComponent>(*_gravity_system);
	get_system_manager()->register_system<Transform2DComponent, RigidBody2DComponent>(*_movement_system);
	get_system_manager()->register_system<LifetimeComponent>(*_lifetime_system);
	get_system_manager()->register_system<GravityComponent, SpriteComponent, RigidBody2DComponent, Transform2DComponent>(*_player_system);
	get_system_manager()->register_system<Transform2DComponent, BoundingBoxComponent>(*_collision_system);

	_player_system->initialize();

	Rand::add_uniform_real_distribution("rotation", -90.0f, 90.0f);
	Rand::add_uniform_real_distribution("scale_x", 10.0f, 50.0f);
	Rand::add_uniform_real_distribution("scale_y", 1.0f, 10.0f);
	Rand::add_uniform_real_distribution("color", 0.5f, 1.0f);
	Rand::add_uniform_real_distribution("lifetime", 2.0f, 30.0f);
	Rand::add_uniform_real_distribution("gravity", -50.0f, -5.0f);
	Rand::add_uniform_real_distribution("pos_x", 0.0f, get_camera()->right);
	Rand::add_uniform_real_distribution("pos_y", get_camera()->top, get_camera()->top * 2);

	_sprites.push_back(std::make_unique<Mage::Sprite>(
		Mage::Color::custom(
			Rand::get_uniform_real("color"),
			Rand::get_uniform_real("color"),
			Rand::get_uniform_real("color"),
			Rand::get_uniform_real("color")
		)));
	auto p = get_entity_manager()->add_entity(EntityTypes::Platform);
	get_component_manager()->add_component<SpriteComponent>(*p,
		{
			.sprite = _sprites.back().get()
		});
	get_component_manager()->add_component<Transform2DComponent>(*p,
		{
			.translation = {200.0f, 5.0f},
			.scale = {400.0f, 10.0f}
		});
	get_component_manager()->add_component<BoundingBoxComponent>(*p,
		{
		});

	//MARK: confetti:)
	/*LOG_WARN("Entity creation starting...");

	for(auto i = 0; i < 5000; ++i)
	{
		_sprites.push_back(std::make_unique<Mage::Sprite>(
			Mage::Color::custom(
				Rand::get_uniform_real("color"),
				Rand::get_uniform_real("color"),
				Rand::get_uniform_real("color"),
				Rand::get_uniform_real("color")
			)
		) ) ;

		auto entity = get_entity_manager()->add_entity(1);
		get_component_manager()->add_component<SpriteComponent>(*entity,
			{
				.sprite = _sprites.back().get()
			} ) ;
		get_component_manager()->add_component<LifetimeComponent>(*entity,
			{
				.remaining = Rand::get_uniform_real("lifetime")
			} ) ;
		get_component_manager()->add_component<Transform2DComponent>(*entity, 
			{
				.translation = { Rand::get_uniform_real("pos_x"), Rand::get_uniform_real("pos_y") },
				.scale = { Rand::get_uniform_real("scale_x"), Rand::get_uniform_real("scale_y")},
				.rotation = Rand::get_uniform_real("rotation"),
			} ) ;
		get_component_manager()->add_component<RigidBody2DComponent>(*entity,
			{
				.velocity = { 0.0f, 0.0f },
			} ) ;
		get_component_manager()->add_component<GravityComponent>(*entity,
			{
				.force = {0.0f, Rand::get_uniform_real("gravity")},
			} ) ;
	}

	LOG_WARN("Entity creation completed...");*/
}

void Game::on_app_closing()
{
	close();
}