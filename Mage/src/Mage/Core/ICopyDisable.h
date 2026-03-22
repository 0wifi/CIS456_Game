#pragma once

namespace Mage
{
	class MAGE_API ICopyDisable
	{
	public:
		ICopyDisable() = default;
		ICopyDisable(ICopyDisable&&) = delete;
		ICopyDisable(ICopyDisable const&) = delete;
		ICopyDisable& operator=(ICopyDisable&&) = delete;
		ICopyDisable& operator=(ICopyDisable const&) = delete;
	};
}