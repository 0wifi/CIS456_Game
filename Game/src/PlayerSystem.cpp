#include "PlayerSystem.h"
#include "Game.h"

#define PLAYER_GRAVITY 1000
#define PLAYER_SCALE 0.5f

#define VELOCITY_PLAYER 300.0f
#define VELOCITY_JUMP 500.0f
#define VELOCITY_SMALL_JUMP 350.0f

#define DURATION_JUMPING 0.15f
#define DURATION_SMALL_JUMP 0.15f

#define DURATION_SHOOTING 0.3f
#define BULLET_SCALE 0.33f
#define BULLET_LIFETIME 1.0f
#define VELOCITY_BULLET 1000.0f
#define BULLET_REL_2_PLAYER_X 36.0f
#define BULLET_REL_2_PLAYER_Y 27.0f
#define BULLET_HALF_X 16.0f
#define BULLET_HALF_Y 5.33f

#define BBOX_RIGHT_FACING_CENTER_X_PLAYER 47.0f
#define BBOX_LEFT_FACING_CENTER_X_PLAYER  77.0f
#define	BBOX_CENTER_Y_PLAYER              79.0f
#define	BBOX_HALF_WIDTH_PLAYER            37.0f
#define	BBOX_HALF_HEIGHT_PLAYER           79.0f

#define OFFSET_PLAYER_CENTER 24.0f

#define SCROLL_MARGIN 0.33f

PlayerSystem::PlayerSystem(Game* game)
	: _game(game)
{
	_game->get_event_manager()->add_on_mouse_button_down_event_listener(this);
	_game->get_event_manager()->add_on_key_down_event_listener(this);
	_game->get_event_manager()->add_on_key_up_event_listener(this);
}

void PlayerSystem::update(Mage::ComponentManager& component_manager, float delta_time)
{
	auto r = component_manager.get_component<RigidBody2DComponent>(*_player_entity);
	auto s = component_manager.get_component<SpriteComponent>(*_player_entity);
	auto t = component_manager.get_component<Transform2DComponent>(*_player_entity);
	auto b = component_manager.get_component<BoundingBoxComponent>(*_player_entity);

	if (t->translation.y < 0.0f - static_cast<float>(s->sprite->get_height()))
		reset_player_entity();

	_last_shot += delta_time;
	if (_last_shot > DURATION_SHOOTING)
		_shooting = false;

	update_player_velocity(r, delta_time);
	update_player_sprite(r, s, t, b);
	update_camera(r, s, t, delta_time);
}

void PlayerSystem::initialize()
{
	_player_sprites["hero_idle"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_idle.png", 9, 0.15f);
	_player_sprites["hero_run"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_run.png", 7, 0.15f);
	_player_sprites["hero_jump"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_jump.png", 4, 0.15f);
	_player_sprites["hero_fall"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_fall.png", 6, 0.15f);
	_player_sprites["hero_shoot"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_shoot.png", 6, 0.05f);
	_player_sprites["hero_run_shoot"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_run_shoot.png", 7, 0.15f);
	_player_sprites["hero_fall_shoot"] = std::make_unique<Mage::Sprite>(
		"res/sprites/hero_fall_shoot.png", 6, 0.05f);
	_player_sprites["bullet_right"] = std::make_unique<Mage::Sprite>(
		"res/sprites/bullet.png", 7, 0.05f);
	_player_sprites["bullet_left"] = std::make_unique<Mage::Sprite>(
		"res/sprites/bullet.png", 7, 0.05f);
	_player_sprites["bullet_left"]->set_flip_x(true);

	_player_entity = _game->get_entity_manager()->add_entity(EntityTypes::PlayerEntity);
	_game->get_component_manager()->add_component(*_player_entity, GravityComponent{});
	_game->get_component_manager()->add_component(*_player_entity, SpriteComponent{});
	_game->get_component_manager()->add_component(*_player_entity, RigidBody2DComponent{});
	_game->get_component_manager()->add_component(*_player_entity, Transform2DComponent{});
	_game->get_component_manager()->add_component(*_player_entity, BoundingBoxComponent
		{
			.center = {BBOX_RIGHT_FACING_CENTER_X_PLAYER, BBOX_CENTER_Y_PLAYER},
			.half_size = {BBOX_HALF_WIDTH_PLAYER, BBOX_HALF_HEIGHT_PLAYER},
			.on_collided = [this](Mage::Entity* self, Mage::Entity* other_entity, const glm::vec2& overlap)
			{
				collision_detected(other_entity, overlap);
			}
		});
	
	reset_player_entity();
}

void PlayerSystem::collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap)
{
	auto oe_bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*other_entity);
	auto oe_t  = _game->get_component_manager()->get_component<Transform2DComponent>(*other_entity);

	auto r  = _game->get_component_manager()->get_component<RigidBody2DComponent>(*_player_entity);
	auto bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*_player_entity);
	auto t  = _game->get_component_manager()->get_component<Transform2DComponent>(*_player_entity);

	auto prev_overlap = CollisionSystem::calculate_overlap(t->prev_translation, t->scale, oe_t->translation, oe_t->scale, bb, oe_bb);

	auto push_x = true;

	if (prev_overlap.x > 0.0f)
		push_x = false;
	else if (prev_overlap.y < 0.0f)
		push_x = overlap.x > overlap.y;

	if (push_x && overlap.y > 0.1f)
	{
		t->translation.x -= overlap.x;
		r->velocity.x = 0.0f;
	}
	else if (overlap.x > 0.1f)
	{
		t->translation.y += overlap.y;
		r->velocity.y = std::max(0.0f, r->velocity.y);

		if(r->velocity.y <= 0.1f)
			_falling = false;
	}
}

void PlayerSystem::reset_player_entity()
{
	auto g = _game->get_component_manager()->get_component<GravityComponent>(*_player_entity);
	auto s = _game->get_component_manager()->get_component<SpriteComponent>(*_player_entity);
	auto r = _game->get_component_manager()->get_component<RigidBody2DComponent>(*_player_entity);
	auto t = _game->get_component_manager()->get_component<Transform2DComponent>(*_player_entity);

	g->force = { 0.0f, -1.0f * PLAYER_GRAVITY };
	s->sprite = _player_sprites["hero_idle"].get();
	r->velocity = { 0.0f, 0.0f };
	t->scale = { PLAYER_SCALE, PLAYER_SCALE };
	t->translation = t->prev_translation = {
		_game->get_camera()->left + (static_cast<float>(_game->get_window()->get_width())
			- static_cast<float>(s->sprite->get_width()) * 0.25f) / 2.0f,
		(static_cast<float>(_game->get_window()->get_height())
			- static_cast<float>(s->sprite->get_height()) * 0.5f) / 2.0f
	};
}

void PlayerSystem::update_player_velocity(RigidBody2DComponent* r, float delta_time)
{

	_last_jump += delta_time;
	if (_jumping && _last_jump > DURATION_JUMPING)
	{
		_jumping = false;
		_falling = true;

		auto jump_button_down_duration = static_cast<std::chrono::duration<float>>(_jump_button_up_at - _jump_button_down_at).count();
		auto jump_velocity = VELOCITY_JUMP;
		if (jump_button_down_duration > 0.0f && jump_button_down_duration < DURATION_SMALL_JUMP)
			jump_velocity = VELOCITY_SMALL_JUMP;
		r->velocity.y = jump_velocity;
	}

	auto prior_velocity_x = r->velocity.x;
	r->velocity.x = 0;
	if (_wasd_states & 0x04) r->velocity.x += -1.0f;
	if (_wasd_states & 0x08) r->velocity.x += 1.0f;

	auto velocity_factor = 1.0f;

	if (std::signbit(prior_velocity_x) != std::signbit(r->velocity.x))
		velocity_factor = 0.1f;
	else if (std::abs(prior_velocity_x) < 0.1f || std::abs(r->velocity.x) < 0.1f)
		velocity_factor = 0.1f;

	r->velocity.x *= VELOCITY_PLAYER * velocity_factor;
}

void PlayerSystem::update_player_sprite(const RigidBody2DComponent* r, SpriteComponent* s, Transform2DComponent* t, BoundingBoxComponent* b)
{
	auto running = std::abs(r->velocity.x) > 0.1f;

	if (_jumping)
		s->sprite = _player_sprites["hero_jump"].get();
	else if (_falling && _shooting)
		s->sprite = _player_sprites["hero_fall_shoot"].get();
	else if (_falling)
		s->sprite = _player_sprites["hero_fall"].get();
	else if (running && _shooting)
		s->sprite = _player_sprites["hero_run_shoot"].get();
	else if (running)
		s->sprite = _player_sprites["hero_run"].get();
	else if (_shooting)
		s->sprite = _player_sprites["hero_shoot"].get();
	else
		s->sprite = _player_sprites["hero_idle"].get();

	auto prior_left_facing = _left_facing;

	if (r->velocity.x < -0.01f) _left_facing = true;
	else if (r->velocity.x > 0.01f) _left_facing = false;

	s->sprite->set_flip_x(_left_facing);

	if (_left_facing && !prior_left_facing)
		t->translation.x -= OFFSET_PLAYER_CENTER;
	else if (!_left_facing && prior_left_facing)
		t->translation.x += OFFSET_PLAYER_CENTER;

	b->center.x = _left_facing ? BBOX_LEFT_FACING_CENTER_X_PLAYER : BBOX_RIGHT_FACING_CENTER_X_PLAYER;
}

void PlayerSystem::update_camera(const RigidBody2DComponent* r, const SpriteComponent* s, const Transform2DComponent* t, float delta_time)
{
	auto window_pos_x = _game->get_camera()->right - t->translation.x;
	auto window_width = static_cast<float>(_game->get_window()->get_width());
	auto scaled_sprite_width = static_cast<float>(s->sprite->get_width()) * t->scale.x;
	auto scrolling_margin = window_width * SCROLL_MARGIN;

	if (window_pos_x - scaled_sprite_width < scrolling_margin
		|| window_pos_x > window_width - scrolling_margin)
	{
		_game->get_camera()->left += r->velocity.x * delta_time;
		_game->get_camera()->right += r->velocity.x * delta_time;
	}
}

void PlayerSystem::on_key_down(Mage::Key key, uint_fast16_t key_modifiers, uint_fast8_t repeat_count)
{
	if (repeat_count > 0)
		return;
	_wasd_states |= (key == Mage::Key::W) ? 0x01 : 0;
	_wasd_states |= (key == Mage::Key::S) ? 0x02 : 0;
	_wasd_states |= (key == Mage::Key::A) ? 0x04 : 0;
	_wasd_states |= (key == Mage::Key::D) ? 0x08 : 0;

	if (key == Mage::Key::Return && !_jumping)
	{
		shoot();
	}

	if (key == Mage::Key::Space && !(_jumping || _falling))
	{
		_jump_button_down_at = std::chrono::system_clock::now();
		jump();
	}
}

void PlayerSystem::on_key_up(Mage::Key key, uint_fast16_t key_modifiers)
{
	_wasd_states &= (key == Mage::Key::W) ? ~0x01 : 0xFF;
	_wasd_states &= (key == Mage::Key::S) ? ~0x02 : 0xFF;
	_wasd_states &= (key == Mage::Key::A) ? ~0x04 : 0xFF;
	_wasd_states &= (key == Mage::Key::D) ? ~0x08 : 0xFF;

	if (key == Mage::Key::Space)
		_jump_button_up_at = std::chrono::system_clock::now();
}


void PlayerSystem::on_mouse_button_down(Mage::MouseButton button, float x, float y, uint8_t click_count)
{
	reset_player_entity();
}

void PlayerSystem::jump()
{
	_jumping = true;
	_player_sprites["hero_jump"]->start_over();
	_last_jump = 0.0f;
}

void PlayerSystem::shoot()
{
	_shooting = true;
	_last_shot = 0.0f;
	add_bullet();
}

void PlayerSystem::add_bullet()
{
	auto t = _game->get_component_manager()->get_component<Transform2DComponent>(*_player_entity);
	
	auto bullet_dir = _left_facing ? -1.0f : 1.0f;
	auto e = _game->get_entity_manager()->add_entity(EntityTypes::Bullet);
	_game->get_component_manager()->add_component<SpriteComponent>(*e,
		{
			.sprite = _left_facing ? _player_sprites["bullet_left"].get() : _player_sprites["bullet_right"].get()
		});
	_game->get_component_manager()->add_component<LifetimeComponent>(*e,
		{
			.remaining = BULLET_LIFETIME
		});
	_game->get_component_manager()->add_component<RigidBody2DComponent>(*e,
		{
			.velocity = {VELOCITY_BULLET * bullet_dir, 0.0f}
		});
	_game->get_component_manager()->add_component<Transform2DComponent>(*e,
		{
			.translation = {t->translation.x + (BULLET_REL_2_PLAYER_X * bullet_dir),
							t->translation.y + BULLET_REL_2_PLAYER_Y},
			.scale = {BULLET_SCALE, BULLET_SCALE}
		});
	_game->get_component_manager()->add_component<BoundingBoxComponent>(*e,
		{
			.center = {BULLET_HALF_X, BULLET_HALF_Y},
			.half_size = {BULLET_HALF_X, BULLET_HALF_Y},
			.on_collided = [&](Mage::Entity* e1, Mage::Entity* e2, const glm::vec2& ol )
			{
				e1->destroy();

				if (e2->get_type() == EntityTypes::Enemy)
				{
					e2->destroy();
				}
			}
		});
}