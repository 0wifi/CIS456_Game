#pragma once

#include "../Core/Core.h"

namespace Mage {
	class MAGE_API Controller
	{
	public:
		explicit Controller(uint_fast32_t controller_idx);

		~Controller();

		static uint_fast8_t get_controller_count();

		bool has_rumble() const;
		bool has_accelerometer() const;
		bool has_gyroscope() const;
		uint_fast16_t get_number_of_axes() const;
		uint_fast16_t get_number_of_buttons() const;

		void rumble(uint_fast32_t ms_to_rumble, uint_fast16_t high_freq_intensity,
			uint_fast16_t low_freq_intensity) const;

	private:
		struct Impl;
		Impl* _impl;
	};
}
