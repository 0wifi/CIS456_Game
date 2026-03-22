#pragma once

#include "Core.h"

namespace Mage
{
	class MAGE_API EventListenerBase : ICopyDisable
	{
	public:
		EventListenerBase() = default;
		virtual ~EventListenerBase() = default;
	};
}