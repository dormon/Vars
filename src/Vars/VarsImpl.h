#pragma once

#include <Vars/Vars.h>
#include <map>

using namespace std;

class vars::VarsImpl {
 public:
  void* add(string const&     n,
            void*             d,
            Destructor const& dst,
            type_info const&  t);

  void             erase(string const& n);
  bool             has(string const& n) const;
  type_info const& getType(string const& n) const;
  void             checkTypes(string const& n, type_info const& t);
  void             ifVarExistsThrow(string const& n) const;
  void             ifVarDoesNotExistThrow(string const& n) const;
  ~VarsImpl();
  void* get(string const& n) const;
  void* reCreate(string const&n,void*d,Destructor const&dst,type_info const&t);

  template <typename T>
  T& add(string const& n, T const& v);
  template <typename T>
  T&                                get(string const& n) const;
  map<string, shared_ptr<Resource>> data;
  map<size_t, string>               idToName;
  map<string, size_t>               nameToId;
};

template <typename T>
T& vars::VarsImpl::add(string const& n, T const& v) {
  auto d   = new T(v);
  auto dst = getDestructor<T>();
  return reinterpret_cast<T&>(*reinterpret_cast<T*>(add(n, d, dst, typeid(T))));
}

template <typename T>
T& vars::VarsImpl::get(string const& n) const {
  return reinterpret_cast<T&>(*reinterpret_cast<T*>(get(n)));
}
