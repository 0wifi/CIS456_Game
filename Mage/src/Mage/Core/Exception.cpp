#include "Exception.h"

namespace Mage {
	struct Exception::Impl 
	{
		std::string msg;
	};

	Exception::Exception(const char* message)
	{
		_impl = new Impl();
		_impl->msg = std::string(message);
	}

	Exception::~Exception()
	{
		delete _impl;
	}

	const char* Exception::what() const
	{
		return _impl->msg.c_str();
	}
}
