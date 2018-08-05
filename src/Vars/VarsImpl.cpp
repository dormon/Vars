#include <Vars/VarsImpl.h>
#include <Vars/Resource.h>
#include <vector>

using namespace vars;

void VarsImpl::ifVarExistsThrow(string const& n) const {
  if (!has(n)) return;
  throw runtime_error(string("variable: ") + n + " already exists!");
}

void VarsImpl::ifVarDoesNotExistThrow(string const& n) const {
  if (has(n)) return;
  throw runtime_error(string("variable: ") + n + " does not exist!");
}

void* VarsImpl::add(string const&     n,
                    void*             d,
                    Destructor const& dst,
                    type_info const&  t) {
  ifVarExistsThrow(n);
  auto id      = data.size();
  data[n]      = make_shared<Resource>(d, dst, t);
  idToName[id] = n;
  nameToId[n]  = id;
  return d;
}

void* VarsImpl::get(string const& n) const {
  ifVarDoesNotExistThrow(n);
  return data.at(n)->getData();
}

void* VarsImpl::reCreate(string const&n,void*d,Destructor const&dst,type_info const&t){
  if(!has(n))
    return add(n,d,dst,t);
  return data[n]->reCreate(d,dst,t);
}

void VarsImpl::erase(string const& n) {
  data.erase(n);
  auto id = nameToId[n];
  nameToId.erase(n);
  idToName.erase(id);
}

bool VarsImpl::has(string const& n) const { return data.count(n) > 0; }

type_info const& VarsImpl::getType(string const& n) const {
  return data.at(n)->getType();
}

void VarsImpl::checkTypes(string const& n, type_info const& t) {
  if (getType(n) == t) return;
  throw runtime_error(string("variable: ") + n + " has different type");
}

VarsImpl::~VarsImpl() {
  std::vector<std::string>names;
  for(auto const&x:nameToId)
    names.push_back(x.first);
  for(auto const&x:names)
    erase(x);
}
