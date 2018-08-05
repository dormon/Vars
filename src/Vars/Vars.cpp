#include <Vars/Resource.h>
#include <Vars/Vars.h>
#include <Vars/VarsImpl.h>

using namespace vars;

void* Vars::add(std::string const&                n,
                void*                             d,
                std::function<void(void*)> const& dst,
                std::type_info const&             t)
{
  return impl->add(n, d, dst, t);
}

bool& Vars::addBool(std::string const& n, bool v)
{
  return impl->add<bool>(n, v);
}

std::string& Vars::addString(std::string const& n, std::string const& v)
{
  return impl->add<std::string>(n, v);
}

size_t& Vars::addSizeT(std::string const& n, size_t v)
{
  return impl->add<size_t>(n, v);
}

float& Vars::addFloat(std::string const& n, float v)
{
  return impl->add<float>(n, v);
}

uint32_t& Vars::addUint32(std::string const& n, uint32_t v)
{
  return impl->add<uint32_t>(n, v);
}

void* Vars::get(std::string const& n) const { return impl->get(n); }

std::string& Vars::getString(std::string const& n) const
{
  return impl->get<std::string>(n);
}

bool& Vars::getBool(std::string const& n) const { return impl->get<bool>(n); }

size_t& Vars::getSizeT(std::string const& n) const
{
  return impl->get<size_t>(n);
}

float& Vars::getFloat(std::string const& n) const
{
  return impl->get<float>(n);
}

uint32_t& Vars::getUint32(std::string const& n) const
{
  return impl->get<uint32_t>(n);
}

void Vars::erase(std::string const& n) { impl->erase(n); }

bool Vars::has(std::string const& n) const { return impl->has(n); }

void Vars::updateTicks(std::string const& n)
{
  auto& d = impl->data.at(n);
  d->updateTicks();
}

size_t Vars::getTicks(std::string const& n) const
{
  return impl->data.at(n)->getTicks();
}

void Vars::setChangeCallback(std::string const& n, OnChange const& clb)
{
  impl->data.at(n)->setChangeCallback(clb);
}

std::shared_ptr<vars::Resource> Vars::getResource(std::string const& n) const
{
  return impl->data.at(n);
}

size_t Vars::getNofVars() const { return impl->data.size(); }

std::string Vars::getVarName(size_t i) const { return impl->idToName.at(i); }

std::type_info const& Vars::getType(std::string const& n) const
{
  return impl->getType(n);
}

void* Vars::reCreate(std::string const&    n,
                     void*                 d,
                     Destructor const&     dst,
                     std::type_info const& t)
{
  return impl->reCreate(n, d, dst, t);
}

void Vars::checkTypes(std::string const& n, std::type_info const& t) const
{
  impl->checkTypes(n, t);
}

Vars::Vars() { impl = std::make_unique<VarsImpl>(); }

Vars::~Vars() {}
