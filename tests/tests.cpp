#include<Vars/Vars.h>
#include<Vars/Resource.h>
#include<Vars/VarsImpl.h>
#include<iostream>
#include<fstream>
#include<memory>

#include<catch.hpp>

using namespace vars;
using namespace std;

SCENARIO("Basic vars tests"){
  Vars vars;
  REQUIRE(vars.has("checkBox") == false);
  vars.addBool("checkBox",true);
  REQUIRE(vars.has("checkBox") == true);
  REQUIRE(vars.getType("checkBox") == typeid(bool));
  REQUIRE(vars.getBool("checkBox")==true);
  REQUIRE(vars.getTicks("checkBox")==1);
  vars.getBool("checkBox") = false;
  vars.updateTicks("checkBox");
  REQUIRE(vars.getBool("checkBox")==false);
  REQUIRE(vars.getTicks("checkBox")==2);
  vars.erase("checkBox");
  REQUIRE(vars.has("checkBox") == false);
}

SCENARIO("Recreate tests"){
  Vars vars;
  vars.addBool("var",false);
  REQUIRE(vars.getBool("var") == false);
  vars.reCreate<float>("var",1.3f);
  REQUIRE(vars.getTicks("var") == 2);
  REQUIRE(vars.getFloat("var") == 1.3f);
}

SCENARIO("Recreate tests, return value"){
  Vars vars;
  auto ptr = vars.reCreate("a",(void*)17,[](void*){},typeid(float));
  REQUIRE(ptr == (void*)17);
  ptr = vars.reCreate("a",(void*)9223,[](void*){},typeid(float));
  REQUIRE(ptr == (void*)9223);
}

class Object{
  public:
    Object(size_t n){
      d = new float[n];
    }
    ~Object(){
      delete[]d;
    }
  protected:
    float*d = nullptr;
};

SCENARIO("Custom object"){
  Vars vars;
  vars.add<Object>("obj",10);
  vars.erase("obj");
}

class Base{
  public:
    Base(Vars&vars):vars(vars){}
    virtual ~Base(){}
    Vars&vars;
};

class Child: public Base{
  public:
    Child(Vars&vars):Base(vars){
      vars.addBool("asdasd",false);
    }
    virtual ~Child(){
      vars.erase("asdasd");
    }
};

SCENARIO("Vars in child object"){
  Vars vars;
  REQUIRE(vars.has("asdasd") == false);
  auto c = make_unique<Child>(vars);
  REQUIRE(vars.has("asdasd") == true);
  c = nullptr;
  REQUIRE(vars.has("asdasd") == false);
}

SCENARIO("Vars in child object, child object in vars"){
  Vars vars;
  REQUIRE(vars.has("asdasd") == false);
  vars.add<Child>("child",vars);
  REQUIRE(vars.has("asdasd") == true);
  REQUIRE(vars.has("child") == true);
  vars.erase("child");
  REQUIRE(vars.has("asdasd") == false);
  REQUIRE(vars.has("child") == false);
}

SCENARIO("Vars in child object, child object in vars, default destructor"){
  Vars vars;
  REQUIRE(vars.has("asdasd") == false);
  vars.add<Child>("child",vars);
  REQUIRE(vars.has("asdasd") == true);
  REQUIRE(vars.has("child") == true);
}

SCENARIO("Directory"){
  Directory root;
  root.add("aaa");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.dirs.empty());

  root.add("aaa");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.dirs.empty());

  root.add("a.b");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b"});

  root.add("a.b");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b"});

  root.add("a.c");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b","c"});

  root.add("a.b.x");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  auto const&adir = root.getDir("a");
  REQUIRE(adir.vars == set<string>{"b","c"});
  REQUIRE(adir.hasDir("b"));
  REQUIRE(adir.getDir("b").vars == set<string>{"x"});

  root.add("a.e.x");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").hasDir("e"));
  REQUIRE(root.getDir("a").getDir("e").hasVar("x"));

  REQUIRE(root.getVars() == set<string>{"aaa","a.b","a.c","a.b.x","a.e.x"});

  REQUIRE(root.isDir("aaa") == false);
  REQUIRE(root.isVar("aaa") == true );
  REQUIRE(root.isVar("a.b") == true );
  REQUIRE(root.isVar("a.c") == true );
  REQUIRE(root.isVar("a.b.x") == true );
  REQUIRE(root.isVar("a.e.x") == true );
  REQUIRE(root.isDir("a.b") == true);
  REQUIRE(root.isDir("a.e") == true);

  root.remove("aaa");
  REQUIRE(root.getVars() == set<string>{"a.b","a.c","a.b.x","a.e.x"});
  root.remove("a.b");
  REQUIRE(root.getVars() == set<string>{"a.c","a.e.x"});
  root.remove("a");
  REQUIRE(root.getVars() == set<string>{});
}

