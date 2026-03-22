#pragma once

#include "../Core/Core.h"

namespace Mage {
	class MAGE_API Camera : ICopyDisable
	{
		friend class Application;

	public:
		~Camera() = default;

		float left;
		float right;
		float top;
		float bottom;

	private:
		Camera() = default;
	};
}
