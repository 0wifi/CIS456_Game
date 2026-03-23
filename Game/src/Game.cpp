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

	Rand::add_uniform_real_distribution("platform_width", 1.0f, 7.0f);
	Rand::add_uniform_real_distribution("platform_gap_width", 1.0f, 7.0f);

	_sprites["platform_left"] = std::make_unique<Mage::Sprite>("res/sprites/platform_left.png", 1, 1.0f);
	_sprites["platform_middle"] = std::make_unique<Mage::Sprite>("res/sprites/platform_middle.png", 1, 1.0f);
	_sprites["platform_right"] = std::make_unique<Mage::Sprite>("res/sprites/platform_right.png", 1, 1.0f);
	_sprites["platform_single"] = std::make_unique<Mage::Sprite>("res/sprites/platform_single.png", 1, 1.0f);

	add_platform(0.0, 6);

	for (size_t i = 350; i < 3000; )
	{
		auto pw = static_cast<size_t>(Rand::get_uniform_real("platform_width"));
		add_platform(static_cast<float>(i), pw); 
		i += 50 * pw;

		auto gw = static_cast<size_t>(Rand::get_uniform_real("platform_gap_width"));
		i += 50 * gw;
	}

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

void Game::add_platform(float pos_x, size_t units_wide)
{
	if (units_wide < 2)
	{
		auto e = get_entity_manager()->add_entity(EntityTypes::Platform);
		get_component_manager()->add_component<SpriteComponent>(*e,
			{
				.sprite = _sprites["platform_single"].get()
			});
		get_component_manager()->add_component<Transform2DComponent>(*e,
			{
				.translation = {pos_x, 0.0f},
				.scale = {0.5f, 0.5f}
			});
		get_component_manager()->add_component<BoundingBoxComponent>(*e,
			{
				.center = {50.0f, 50.0f},
				.half_size = {50.0f, 50.0f}
			});
		return;
	}

	auto e = get_entity_manager()->add_entity(EntityTypes::Platform);
	get_component_manager()->add_component<SpriteComponent>(*e,
		{
			.sprite = _sprites["platform_left"].get()
		});
	get_component_manager()->add_component<Transform2DComponent>(*e,
		{
			.translation = {pos_x, 0.0f},
			.scale = {0.5f, 0.5f}
		});
	get_component_manager()->add_component<BoundingBoxComponent>(*e,
		{
			.center = {50.0f, 50.0f},
			.half_size = {50.0f, 50.0f}
		});


	auto middle_start = static_cast<size_t>(pos_x) + 50;
	auto middle_end = static_cast<size_t>(pos_x) + (units_wide - 1) * 50;
	for (size_t i = middle_start; i < middle_end; i += 50)
	{
		e = get_entity_manager()->add_entity(EntityTypes::Platform);
		get_component_manager()->add_component<SpriteComponent>(*e,
			{
				.sprite = _sprites["platform_middle"].get()
			});
		get_component_manager()->add_component<Transform2DComponent>(*e,
			{
				.translation = {static_cast<float>(i), 0.0f},
				.scale = {0.5f, 0.5f}
			});
		get_component_manager()->add_component<BoundingBoxComponent>(*e,
			{
				.center = {50.0f, 50.0f},
				.half_size = {50.0f, 50.0f}
			});
	}

	e = get_entity_manager()->add_entity(EntityTypes::Platform);
	get_component_manager()->add_component<SpriteComponent>(*e,
		{
			.sprite = _sprites["platform_right"].get()
		});
	get_component_manager()->add_component<Transform2DComponent>(*e,
		{
			.translation = {static_cast<float>(middle_end), 0.0f},
			.scale = {0.5f, 0.5f}
		});
	get_component_manager()->add_component<BoundingBoxComponent>(*e,
		{
			.center = {50.0f, 50.0f},
			.half_size = {50.0f, 50.0f}
		});
}
