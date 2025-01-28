#pragma once

#include "jnypch.h"

namespace jny
{

template <typename T>
using s_ptr = std::shared_ptr<T>;

template <typename T>
using u_ptr = std::unique_ptr<T>;

template <typename T>
using w_ptr = std::weak_ptr<T>;

} //-- jny