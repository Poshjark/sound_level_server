#pragma once

#include "boost/thread/mutex.hpp"
#include "boost/thread/lock_guard.hpp"
#include "boost/thread/condition_variable.hpp"

namespace Utils
{
	using mutex_t = ::boost::mutex;
	using Locker = ::boost::unique_lock<mutex_t>;
	using cv_t = ::boost::condition_variable_any;
}
