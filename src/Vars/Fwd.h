#pragma once

#include <functional>

namespace vars {
class VarsImpl;
class Vars;
class Resource;

using Destructor = std::function<void(void*)>;
using OnChange   = std::function<void()>;

template<typename T>
inline Destructor getDestructor();
}  // namespace vars
