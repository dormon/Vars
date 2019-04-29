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

int32_t& Vars::addInt32(std::string const& n, int32_t v)
{
  return impl->add<int32_t>(n, v);
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

int32_t& Vars::getInt32(std::string const& n) const
{
  return impl->get<int32_t>(n);
}

bool&        Vars::addOrGetBool(std::string const& n, bool v){
  return impl->addOrGet<bool>(n,v);
}

std::string& Vars::addOrGetString(std::string const& n, std::string const& v){
  return impl->addOrGet<std::string>(n,v);
}

size_t&      Vars::addOrGetSizeT(std::string const& n, size_t v){
  return impl->addOrGet<size_t>(n,v);
}

float&       Vars::addOrGetFloat(std::string const& n, float v){
  return impl->addOrGet<float>(n,v);
}

uint32_t&    Vars::addOrGetUint32(std::string const& n, uint32_t v){
  return impl->addOrGet<uint32_t>(n,v);
}

int32_t&    Vars::addOrGetInt32(std::string const& n, int32_t v){
  return impl->addOrGet<int32_t>(n,v);
}

void Vars::erase(std::string const& n) { impl->erase(n); }

void Vars::eraseDir(std::string const& n) { impl->eraseDir(n); }

void Vars::eraseVar(std::string const& n) { impl->eraseVar(n); }

bool Vars::isDir(std::string const& n) const { return impl->isDir(n); }

bool Vars::isVar(std::string const& n) const { return impl->isVar(n); }

bool Vars::has(std::string const& n) const { return impl->has(n); }

void Vars::updateTicks(std::string const& n) { impl->updateTicks(n); }

size_t Vars::getTicks(std::string const& n) const { return impl->getTicks(n); }

void Vars::setChangeCallback(std::string const& n, OnChange const& clb)
{
  impl->setChaneCallback(n, clb);
}

std::shared_ptr<vars::Resource> Vars::getResource(std::string const& n) const
{
  return impl->getResource(n);
}

size_t Vars::getNofVars() const { return impl->getNofVars(); }

std::string Vars::getVarName(size_t i) const { return impl->getVarName(i); }

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

void Vars::pushCallerName(std::string const&n){
  impl->pushCallerName(n);
}

void Vars::popCallerName(){
  impl->popCallerName();
}

void Vars::checkTypes(std::string const& n, std::type_info const& t) const
{
  impl->checkTypes(n, t);
}

Vars::Vars() { impl = std::make_unique<VarsImpl>(); }

Vars::~Vars() {}
