#pragma once

#include "Core.h"

namespace Mage {

	class MAGE_API Exception
	{
	public:
		explicit Exception(const char* message);

		~Exception();

		const char* what() const;

	private:
		struct Impl;
		Impl* _impl;
		
	};
}
