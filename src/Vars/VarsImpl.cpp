#include <Vars/Resource.h>
#include <Vars/VarsImpl.h>
#include <vector>

using namespace vars;

void VarsImpl::ifVarExistsThrow(string const& n) const
{
  if (!has(n)) return;
  throw runtime_error(string("variable: ") + n + " already exists!");
}

void VarsImpl::ifVarDoesNotExistThrow(string const& n) const
{
  if (has(n)) return;
  throw runtime_error(string("variable: ") + n + " does not exist!");
}

void* VarsImpl::add(string const&     n,
                    void*             d,
                    Destructor const& dst,
                    type_info const&  t)
{
  ifVarExistsThrow(n);
  auto id      = resources.size();
  resources[n] = make_shared<Resource>(d, dst, t);
  idToName[id] = n;
  nameToId[n]  = id;
  root.add(n);
  return d;
}

void* VarsImpl::get(string const& n) const
{
  ifVarDoesNotExistThrow(n);
  return resources.at(n)->getData();
}

void* VarsImpl::reCreate(string const&     n,
                         void*             d,
                         Destructor const& dst,
                         type_info const&  t)
{
  if (!has(n)) return add(n, d, dst, t);
  return resources[n]->reCreate(d, dst, t);
}

void VarsImpl::updateTicks(string const& n)
{
  auto& d = resources.at(n);
  d->updateTicks();
}

size_t VarsImpl::getTicks(string const& n) const
{
  return resources.at(n)->getTicks();
}

void VarsImpl::setChaneCallback(string const& n, OnChange const& clb)
{
  resources.at(n)->setChangeCallback(clb);
}

shared_ptr<Resource> VarsImpl::getResource(string const& n) const
{
  return resources.at(n);
}

size_t VarsImpl::getNofVars() const { return resources.size(); }

string VarsImpl::getVarName(size_t i) const { return idToName.at(i); }

void VarsImpl::erase(string const& n)
{
  eraseVar(n);
  eraseDir(n);
}

void VarsImpl::eraseDir(string const& n)
{
  if (!isDir(n)) return;
  auto forRemoval = root.getVars(n);
  for (auto const& r : forRemoval) eraseVar(r);
}

void VarsImpl::eraseVar(string const& n)
{
  if (!isVar(n)) return;
  resources.erase(n);
  auto id = nameToId.at(n);
  nameToId.erase(n);
  idToName.erase(id);
  root.removeVar(n);
}

bool VarsImpl::isDir(string const& n) const { return root.isDir(n); }

bool VarsImpl::isVar(string const& n) const { return root.isVar(n); }

bool VarsImpl::has(string const& n) const { return resources.count(n) > 0; }

type_info const& VarsImpl::getType(string const& n) const
{
  return resources.at(n)->getType();
}

void VarsImpl::checkTypes(string const& n, type_info const& t)
{
  if (getType(n) == t) return;
  throw runtime_error(string("variable: ") + n + " has different type");
}

VarsImpl::~VarsImpl()
{
  vector<string> names;
  for (auto const& x : nameToId) names.push_back(x.first);
  for (auto const& x : names) erase(x);
}
