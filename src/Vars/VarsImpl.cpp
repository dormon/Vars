#include <Vars/VarsImpl.h>
#include <Vars/Resource.h>

using namespace vars;

void VarsImpl::ifVarExistsThrow(string const& n) const {
  if (!has(n)) return;
  throw runtime_error(string("variable: ") + n + " already exists!");
}

void VarsImpl::ifVarDoesNotExistThrow(string const& n) const {
  if (has(n)) return;
  throw runtime_error(string("variable: ") + n + " does not exist!");
}

tuple<string,string> splitPathToFirstDirAndRest(string const&fullPath){
  auto pos = fullPath.find(".");
  if(pos == string::npos)return tuple<string,string>("",fullPath);
  return tuple<string,string>(fullPath.substr(0,pos),fullPath.substr(pos+1));
}

void Directory::add(string const&fullPath){
  auto split = splitPathToFirstDirAndRest(fullPath);
  auto const&dir  = std::get<0>(split);
  auto const&rest = std::get<1>(split);
  if(dir == ""){
    vars.insert(rest);
    return;
  }
  if(dirs.count(dir) == 0)
    dirs[dir] = {};
  dirs.at(dir).add(rest);
}

bool Directory::hasDir(string const&d)const{
  return dirs.count(d) > 0;
}

bool Directory::hasVar(string const&v)const{
  return vars.count(v) > 0;
}

Directory&Directory::getDir(string const&d){
  return dirs.at(d);
}

Directory const&Directory::getDir(string const&d)const{
  return dirs.at(d);
}

set<string>Directory::getVars()const{
  set<string>result;
  result = vars;
  for(auto const&d:dirs){
    auto subVars = d.second.getVars();
    for(auto const&sv:subVars)
      result.insert(d.first+"."+sv);
  }
  return result;
}

bool Directory::isDir(string const&fullPath)const{
  auto const split = splitPathToFirstDirAndRest(fullPath);
  auto const&dir  = std::get<0>(split);
  auto const&rest = std::get<1>(split);
  if(dir == "")
    return dirs.count(rest) > 0;
  return dirs.at(dir).isDir(rest);
}

bool Directory::isVar(string const&fullPath)const{
  auto const split = splitPathToFirstDirAndRest(fullPath);
  auto const&dir  = std::get<0>(split);
  auto const&rest = std::get<1>(split);
  if(dir == "")
    return vars.count(rest);
  return dirs.at(dir).isVar(rest);
}

void Directory::remove(string const&fullPath){
  auto const split = splitPathToFirstDirAndRest(fullPath);
  auto const&dir  = std::get<0>(split);
  auto const&rest = std::get<1>(split);
  if(dir == ""){
    vars.erase(rest);
    dirs.erase(rest);
    return;
  }
  dirs.at(dir).remove(rest);
}

void* VarsImpl::add(string const&     n,
                    void*             d,
                    Destructor const& dst,
                    type_info const&  t) {
  ifVarExistsThrow(n);
  auto id      = resources.size();
  resources[n] = make_shared<Resource>(d, dst, t);
  idToName[id] = n;
  nameToId[n]  = id;
  root.add(n);
  return d;
}

void* VarsImpl::get(string const& n) const {
  ifVarDoesNotExistThrow(n);
  return resources.at(n)->getData();
}

void* VarsImpl::reCreate(string const&n,void*d,Destructor const&dst,type_info const&t){
  if(!has(n))
    return add(n,d,dst,t);
  return resources[n]->reCreate(d,dst,t);
}

void VarsImpl::updateTicks(string const&n){
  auto& d = resources.at(n);
  d->updateTicks();
}

size_t VarsImpl::getTicks(string const&n)const{
  return resources.at(n)->getTicks();
}

void VarsImpl::setChaneCallback(string const&n,OnChange const&clb){
  resources.at(n)->setChangeCallback(clb);
}

shared_ptr<Resource>VarsImpl::getResource(string const&n)const{
  return resources.at(n);
}

size_t VarsImpl::getNofVars()const{
  return resources.size(); 
}

string VarsImpl::getVarName(size_t i)const{
  return idToName.at(i); 
}


void VarsImpl::erase(string const& n) {
  resources.erase(n);
  auto id = nameToId[n];
  nameToId.erase(n);
  idToName.erase(id);
}

bool VarsImpl::has(string const& n) const { return resources.count(n) > 0; }

type_info const& VarsImpl::getType(string const& n) const {
  return resources.at(n)->getType();
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
