#pragma once

#include "../Core/Core.h"

namespace Mage {

	class MAGE_API Sprite : ICopyDisable
	{
	public:
		explicit Sprite(const Color& color);

		Sprite(const char* sprite_image_file, uint_fast32_t frames, float frame_time);

		~Sprite();

		bool is_textured() const;

		uint_fast32_t get_width() const;
		uint_fast32_t get_height() const;

		Color& get_color() const;
		void set_color(const Color& color);

		bool get_flip_x() const;
		void set_flip_x(bool flip);

		void start_over();

		void draw(float elapsed_time) const;

	private:
		struct Impl;
		Impl* _impl;
	};
}