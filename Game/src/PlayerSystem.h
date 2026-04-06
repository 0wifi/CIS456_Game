#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class Game ;

class PlayerSystem final :
	public Mage::System,
	public Mage::OnMouseButtonDownEventListener,
	public Mage::OnKeyDownEventListener,
	public Mage::OnKeyUpEventListener,
	public Mage::OnControllerAxisMotionEventListener,
	public Mage::OnControllerButtonDownEventListener,
	public Mage::OnControllerButtonUpEventListener
{
public:
	explicit PlayerSystem(Game* game);

	~PlayerSystem() override = default;

	void update(Mage::ComponentManager& component_manager, float delta_time) override;

	void initialize();

	void on_key_down(Mage::Key key, uint_fast16_t key_modifiers, uint_fast8_t repeat_count) override;
	void on_key_up(Mage::Key key, uint_fast16_t key_modifiers) override;

	void on_mouse_button_down(Mage::MouseButton button, float x, float y, uint8_t click_count) override;

	void on_controller_axis_motion(int_fast32_t controller_id, uint_fast32_t axis_id, float axis_value) override;
	void on_controller_button_down(int_fast32_t controller_id, uint_fast8_t button_id) override;
	void on_controller_button_up(int_fast32_t controller_id, uint_fast8_t button_id) override;

private:
	Game* _game = nullptr;
	Mage::Entity* _player_entity = nullptr;

	std::unique_ptr<Mage::Controller> _controller = nullptr;
	float _controller_x_axis_value = 0.0f;
	float _controller_y_axis_value = 0.0f;

	std::map<std::string, std::unique_ptr<Mage::Sprite>> _player_sprites;
	bool _left_facing = false;
	uint_fast8_t _wasd_states = 0;
	bool _jumping = false;
	bool _falling = false;
	float _last_jump = 0.0f;
	std::chrono::time_point<std::chrono::system_clock> _jump_button_down_at;
	std::chrono::time_point<std::chrono::system_clock> _jump_button_up_at;

	bool _shooting = false;
	float _last_shot = 0.0f;

	bool _dead = false;

	void collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap);

	void reset_player_entity();

	void update_player_velocity(RigidBody2DComponent* r, float delta_time);
	void update_player_sprite(const RigidBody2DComponent* r, SpriteComponent* s,
								Transform2DComponent* t, BoundingBoxComponent* b);
	void update_camera(const RigidBody2DComponent* r, const SpriteComponent* s,
								const Transform2DComponent* t, float delta_time);

	void jump();
	void shoot();

	void die();

	std::atomic<bool> _revive = false;

	void add_bullet();

	void add_explosion(float x, float y, float scale_x, float scale_y);

};