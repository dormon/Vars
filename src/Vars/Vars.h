#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include <Vars/Fwd.h>
#include <Vars/vars_export.h>

class vars::Vars {
 public:
  VARS_EXPORT Vars();
  VARS_EXPORT ~Vars();
  VARS_EXPORT void*        add(std::string const&    n,
                               void*                 d,
                               Destructor const&     dst,
                               std::type_info const& t);
  VARS_EXPORT bool&        addBool(std::string const& n, bool v = false);
  VARS_EXPORT std::string& addString(std::string const& n, std::string const& v = "");
  VARS_EXPORT size_t&      addSizeT(std::string const& n, size_t v = 0);
  VARS_EXPORT float&       addFloat(std::string const& n, float v = 0.f);
  VARS_EXPORT uint32_t&    addUint32(std::string const& n, uint32_t v = 0);
  VARS_EXPORT void*        get(std::string const& n) const;
  VARS_EXPORT std::string& getString(std::string const& n) const;
  VARS_EXPORT bool&        getBool(std::string const& n) const;
  VARS_EXPORT size_t&      getSizeT(std::string const& n) const;
  VARS_EXPORT float&       getFloat(std::string const& n) const;
  VARS_EXPORT uint32_t&    getUint32(std::string const& n) const;

  VARS_EXPORT bool&        addOrGetBool(std::string const& n, bool v = false);
  VARS_EXPORT std::string& addOrGetString(std::string const& n, std::string const& v = "");
  VARS_EXPORT size_t&      addOrGetSizeT(std::string const& n, size_t v = 0);
  VARS_EXPORT float&       addOrGetFloat(std::string const& n, float v = 0.f);
  VARS_EXPORT uint32_t&    addOrGetUint32(std::string const& n, uint32_t v = 0);

  VARS_EXPORT void         erase(std::string const& n);
  VARS_EXPORT void         eraseDir(std::string const& n);
  VARS_EXPORT void         eraseVar(std::string const& n);
  VARS_EXPORT bool         isDir(std::string const& n)const;
  VARS_EXPORT bool         isVar(std::string const& n)const;
  VARS_EXPORT bool         has(std::string const& n) const;
  VARS_EXPORT void         updateTicks(std::string const& n);
  VARS_EXPORT size_t       getTicks(std::string const& n) const;
  VARS_EXPORT void         setChangeCallback(std::string const& n, OnChange const& clb);
  VARS_EXPORT std::shared_ptr<Resource> getResource(std::string const& n) const;
  VARS_EXPORT size_t                    getNofVars() const;
  VARS_EXPORT std::string               getVarName(size_t i) const;
  VARS_EXPORT std::type_info const&     getType(std::string const& n) const;
  VARS_EXPORT void*                     reCreate(std::string const&    n,
                                     void*                 d,
                                     Destructor const&     dst,
                                     std::type_info const& t);
  VARS_EXPORT void pushCallerName(std::string const&n = "");
  VARS_EXPORT void popCallerName();

  template<typename T,typename...ARGS>
  std::vector<T>&addVector(std::string const&n,ARGS&&... args);
  template<typename T>
  std::vector<T>&getVector(std::string const&n);
  template<typename T,typename...ARGS>
  std::vector<T>&reCreateVector(std::string const&n, ARGS&&...args);
  template <typename CLASS, typename... ARGS>
  CLASS* add(std::string const& n, ARGS&&... args);
  template <typename CLASS>
  CLASS* get(std::string const& n) const;
  template <typename CLASS,typename... ARGS>
  CLASS* addOrGet(std::string const& n, ARGS&&... args);
  template <typename CLASS>
  CLASS* getReinterpret(std::string const& n) const;
  template <typename CLASS, typename... ARGS>
  CLASS* reCreate(std::string const& n, ARGS&&... args);

 private:
  VARS_EXPORT void checkTypes(std::string const& n, std::type_info const& t) const;
  friend class VarsImpl;
  std::unique_ptr<VarsImpl> impl;
};


template<typename T,typename...ARGS>
std::vector<T>&vars::Vars::addVector(std::string const&n,ARGS&&... args){
  auto r = add<std::vector<T>>(n,args...);
  return *reinterpret_cast<std::vector<T>*>(r);
}

template<typename T>
std::vector<T>&vars::Vars::getVector(std::string const&n){
  return *get<std::vector<T>>(n);
}

template<typename T,typename...ARGS>
std::vector<T>&vars::Vars::reCreateVector(std::string const&n, ARGS&&...args){
  return *reCreate<std::vector<T>>(n,args...);
}

template <typename CLASS, typename... ARGS>
CLASS* vars::Vars::add(std::string const& n, ARGS&&... args)
{
  void* data = new CLASS(args...);
  auto  r    = add(n, data, getDestructor<CLASS>(), typeid(CLASS));
  return reinterpret_cast<CLASS*>(r);
}

template <typename CLASS>
CLASS* vars::Vars::get(std::string const& n) const
{
  checkTypes(n, typeid(CLASS));
  return reinterpret_cast<CLASS*>(get(n));
}

template <typename CLASS, typename... ARGS>
CLASS* vars::Vars::addOrGet(std::string const& n, ARGS&&... args)
{
  if(has(n))return get<CLASS>(n);
  return add<CLASS>(n,args...);
}

template <typename CLASS>
CLASS* vars::Vars::getReinterpret(std::string const& n) const
{
  return reinterpret_cast<CLASS*>(get(n));
}

template <typename CLASS, typename... ARGS>
CLASS* vars::Vars::reCreate(std::string const& n, ARGS&&... args)
{
  void* data = new CLASS(args...);
  auto  r    = reCreate(n, data, getDestructor<CLASS>(), typeid(CLASS));
  return reinterpret_cast<CLASS*>(r);
}

template <typename T>
inline vars::Destructor vars::getDestructor()
{
  return [](void* ptr) { delete reinterpret_cast<T*>(ptr); };
}
