#include <Vars/Resource.h>

using namespace vars;
using namespace std;

vars::Resource::Resource(void* d, Destructor const& dst, type_info const& t)
    : data(d), destructor(dst), type(t) {}

vars::Resource::~Resource() { destructor(data); }

void vars::Resource::updateTicks() {
  ticks++;
  if (changeCallback) changeCallback();
}

size_t vars::Resource::getTicks() const { return ticks; }

type_info const& vars::Resource::getType() const { return type; }

void vars::Resource::setChangeCallback(OnChange const& clb) {
  changeCallback = clb;
}

void* vars::Resource::getData() const { return data; }
