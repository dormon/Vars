#pragma once

#include <Vars/Vars.h>
#include <map>
#include <set>
#include <vector>
#include <tuple>

using namespace std;

class vars::Directory{
  public:
    void add(string const&fullPath);
    void remove(string const&fullPath);
    bool hasDir(string const&d)const;
    bool hasVar(string const&v)const;
    Directory&getDir(string const&d);
    Directory const&getDir(string const&d)const;
    set<string>getVars()const;
    bool isDir(string const&fullPath)const;
    bool isVar(string const&fullPath)const;
    map<string,Directory>dirs;
    set<string>vars;
};

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
  void updateTicks(string const&n);
  size_t getTicks(string const&n)const;
  void setChaneCallback(string const&n,OnChange const&clb);
  shared_ptr<Resource>getResource(string const&n)const;
  size_t getNofVars()const;
  string getVarName(size_t i)const;

  template <typename T>
  T& add(string const& n, T const& v);
  template <typename T>
  T&                                get(string const& n) const;

 protected:
  map<string, shared_ptr<Resource>> resources;
  Directory                         root;
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

//vector<tuple<string,string>>getAllSplits(string const&p);
//void addDirectory(vars::Directory&root,string const&fullPath);
//void removeDirectory(map<string,set<string>>&directories,string const&dirPath);
//set<string>getAllSubDirs(map<string,set<string>>const&directories,string const&dir);
