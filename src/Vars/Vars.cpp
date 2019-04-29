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

void* Vars::get(std::string const& n) const { return impl->get(n); }

#define IMPLEMENT_ADD_BASIC(name,type)\
type& Vars::add##name(std::string const& n, type const& v)\
{\
  return impl->add<type>(n, v);\
}

IMPLEMENT_ADD_BASIC(String,std::string)
IMPLEMENT_ADD_BASIC(Float ,float      )
IMPLEMENT_ADD_BASIC(SizeT ,size_t     )
IMPLEMENT_ADD_BASIC(Uint32,uint32_t   )
IMPLEMENT_ADD_BASIC(Int32 ,int32_t    )
IMPLEMENT_ADD_BASIC(Uint16,uint16_t   )
IMPLEMENT_ADD_BASIC(Int16 ,int16_t    )
IMPLEMENT_ADD_BASIC(Uint8 ,uint8_t    )
IMPLEMENT_ADD_BASIC(Int8  ,int8_t     )
IMPLEMENT_ADD_BASIC(Bool  ,bool       )

#define IMPLEMENT_GET_BASIC(name,type)\
type& Vars::get##name(std::string const& n) const\
{\
  return impl->get<type>(n);\
}

IMPLEMENT_GET_BASIC(String,std::string)
IMPLEMENT_GET_BASIC(Float ,float      )
IMPLEMENT_GET_BASIC(SizeT ,size_t     )
IMPLEMENT_GET_BASIC(Uint32,uint32_t   )
IMPLEMENT_GET_BASIC(Int32 ,int32_t    )
IMPLEMENT_GET_BASIC(Uint16,uint16_t   )
IMPLEMENT_GET_BASIC(Int16 ,int16_t    )
IMPLEMENT_GET_BASIC(Uint8 ,uint8_t    )
IMPLEMENT_GET_BASIC(Int8  ,int8_t     )
IMPLEMENT_GET_BASIC(Bool  ,bool       )

#define IMPLEMENT_ADD_OR_GET_BASIC(name,type)\
type& Vars::addOrGet##name(std::string const& n, type const& v){\
  return impl->addOrGet<type>(n,v);\
}

IMPLEMENT_ADD_OR_GET_BASIC(String,std::string)
IMPLEMENT_ADD_OR_GET_BASIC(Float ,float      )
IMPLEMENT_ADD_OR_GET_BASIC(SizeT ,size_t     )
IMPLEMENT_ADD_OR_GET_BASIC(Uint32,uint32_t   )
IMPLEMENT_ADD_OR_GET_BASIC(Int32 ,int32_t    )
IMPLEMENT_ADD_OR_GET_BASIC(Uint16,uint16_t   )
IMPLEMENT_ADD_OR_GET_BASIC(Int16 ,int16_t    )
IMPLEMENT_ADD_OR_GET_BASIC(Uint8 ,uint8_t    )
IMPLEMENT_ADD_OR_GET_BASIC(Int8  ,int8_t     )
IMPLEMENT_ADD_OR_GET_BASIC(Bool  ,bool       )

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
