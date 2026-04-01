#include "Controller.h"

namespace Mage {
	struct Controller::Impl
	{
		SDL_Joystick* joystick_id;
		SDL_GameController* controller_id;
	};

	Controller::Controller(uint_fast32_t controller_idx)
	{
		_impl = new Impl;
		_impl->joystick_id = SDL_JoystickOpen(static_cast<int>(controller_idx));
		_impl->controller_id = SDL_GameControllerOpen(static_cast<int>(controller_idx));
		LOG_E_INFO("Controller created; idx: %d", controller_idx);
	}

	Controller::~Controller()
	{
		SDL_GameControllerClose(_impl->controller_id);
		SDL_JoystickClose(_impl->joystick_id);
		delete _impl;
		LOG_E_INFO("Controller destroyed");
	}

	uint_fast8_t Controller::get_controller_count()
	{
		auto c = SDL_NumJoysticks();
		if (c < 0)
			return 0;
		return static_cast<uint_fast32_t>(SDL_NumJoysticks());
	}

	bool Controller::has_rumble() const
	{
		if (_impl->controller_id == nullptr)
			return false;
		return SDL_GameControllerHasRumble(_impl->controller_id) == SDL_TRUE;
	}

	bool Controller::has_accelerometer() const
	{
		if (_impl->controller_id == nullptr)
			return false;
		return SDL_GameControllerHasSensor(_impl->controller_id, SDL_SENSOR_ACCEL) == SDL_TRUE;
	}
	
	bool Controller::has_gyroscope() const
	{
		if (_impl->controller_id == nullptr)
			return false;
		return SDL_GameControllerHasSensor(_impl->controller_id, SDL_SENSOR_GYRO) == SDL_TRUE;
	}

	uint_fast16_t Controller::get_number_of_axes() const
	{
		if (_impl->controller_id == nullptr)
			return 0;
		return static_cast<uint_fast16_t>(SDL_JoystickNumAxes(_impl->joystick_id));
	}

	uint_fast16_t Controller::get_number_of_buttons() const
	{
		if (_impl->controller_id == nullptr)
			return 0;
		return static_cast<uint_fast16_t>(SDL_JoystickNumButtons(_impl->joystick_id));
	}

	void Controller::rumble(uint_fast32_t ms_to_rumble, uint16_t high_freq_intensity,
		uint16_t low_freq_intensity) const
	{
		if (_impl->controller_id == nullptr)
			return;
		SDL_GameControllerRumble(_impl->controller_id, low_freq_intensity, high_freq_intensity, ms_to_rumble);
	}
}
