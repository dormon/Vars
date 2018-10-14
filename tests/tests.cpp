#include<Vars/Vars.h>
#include<Vars/Resource.h>
#include<Vars/VarsImpl.h>
#include<iostream>
#include<fstream>
#include<memory>

#include<catch.hpp>

using namespace vars;
using namespace std;

SCENARIO("Directory"){
  Directory root;

  REQUIRE(root.getVars() == set<string>{});

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

  REQUIRE(root.getVars("a") == set<string>{"a.b","a.c","a.b.x","a.e.x"});
  REQUIRE(root.getVars("a.b") == set<string>{"a.b.x"});
  REQUIRE(root.getVars("a.e") == set<string>{"a.e.x"});
  REQUIRE(root.getVars("a.e.x") == set<string>{});

  root.remove("aaa");
  REQUIRE(root.getVars() == set<string>{"a.b","a.c","a.b.x","a.e.x"});
  root.remove("a.b");
  REQUIRE(root.getVars() == set<string>{"a.c","a.e.x"});
  root.remove("a");
  REQUIRE(root.getVars() == set<string>{});
}

SCENARIO("Directory II"){
  Directory root;
  root.add("a");
  root.add("a.a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);

  root.removeVar("a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == false);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);

  root.removeVar("a.a");
  REQUIRE(root.isDir("a") == false);
  REQUIRE(root.isVar("a") == false);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == false);

  root.add("a");
  root.add("a.a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);
  root.remove("a");
  REQUIRE(root.isDir("a") == false);
  REQUIRE(root.isVar("a") == false);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == false);

  root.add("a");
  root.add("a.a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);
  root.remove("a.a");
  REQUIRE(root.isDir("a") == false);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == false);
}

SCENARIO("Basic vars tests"){
  Vars vars;
  REQUIRE(vars.has("checkBox") == false);
  vars.addBool("checkBox",true);
  REQUIRE(vars.has("checkBox") == true);
  REQUIRE(vars.getType("checkBox") == typeid(bool));
  REQUIRE(vars.getBool("checkBox")==true);
  REQUIRE(vars.getTicks("checkBox")==1);
  REQUIRE(vars.isDir("checkBox") == false);
  REQUIRE(vars.isVar("checkBox") == true);
  vars.getBool("checkBox") = false;
  vars.updateTicks("checkBox");
  REQUIRE(vars.getBool("checkBox")==false);
  REQUIRE(vars.getTicks("checkBox")==2);
  vars.erase("checkBox");
  REQUIRE(vars.has("checkBox") == false);
  REQUIRE(vars.isDir("checkBox") == false);
  REQUIRE(vars.isVar("checkBox") == false);
}

SCENARIO("Vars vs Dirs"){
  Vars vars;
  vars.addBool("a");
  vars.addBool("a.a");

  REQUIRE(vars.isDir("a") == true);
  REQUIRE(vars.isVar("a") == true);
  REQUIRE(vars.isVar("a.a") == true);
  REQUIRE(vars.isDir("a.a") == false);
}

SCENARIO("Erase vars tests"){
  Vars vars;
  vars.addBool("a.a.a");
  vars.addBool("a.a.b");
  vars.addBool("a.a.c");
  vars.addBool("a.b.a");
  vars.addBool("a.b.b");
  vars.addBool("a.b.c");
  vars.addBool("a.c.a");
  vars.addBool("a.c.b");
  vars.addBool("a.c.c");
  vars.addBool("a.a");
  vars.addBool("a.b");
  vars.addBool("a.c");
  vars.addBool("a");
  vars.addBool("b");
  vars.addBool("c");
  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a") == true);
  REQUIRE(vars.isDir("a.b") == true);
  REQUIRE(vars.isDir("a.c") == true);
  REQUIRE(vars.isDir("a") == true);
  REQUIRE(vars.isDir("b") == false);
  REQUIRE(vars.isDir("c") == false);

  REQUIRE(vars.isVar("a.a.a") == true);
  REQUIRE(vars.isVar("a.a.b") == true);
  REQUIRE(vars.isVar("a.a.c") == true);
  REQUIRE(vars.isVar("a.a.a") == true);
  REQUIRE(vars.isVar("a.a.b") == true);
  REQUIRE(vars.isVar("a.a.c") == true);
  REQUIRE(vars.isVar("a.a") == true);
  REQUIRE(vars.isVar("a.b") == true);
  REQUIRE(vars.isVar("a.c") == true);
  REQUIRE(vars.isVar("a") == true);
  REQUIRE(vars.isVar("b") == true);
  REQUIRE(vars.isVar("c") == true);

  vars.eraseDir("a");

  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a") == false);
  REQUIRE(vars.isDir("a.b") == false);
  REQUIRE(vars.isDir("a.c") == false);
  REQUIRE(vars.isDir("a") == false);
  REQUIRE(vars.isDir("b") == false);
  REQUIRE(vars.isDir("c") == false);

  REQUIRE(vars.isVar("a.a.a") == false);
  REQUIRE(vars.isVar("a.a.b") == false);
  REQUIRE(vars.isVar("a.a.c") == false);
  REQUIRE(vars.isVar("a.a.a") == false);
  REQUIRE(vars.isVar("a.a.b") == false);
  REQUIRE(vars.isVar("a.a.c") == false);
  REQUIRE(vars.isVar("a.a") == false);
  REQUIRE(vars.isVar("a.b") == false);
  REQUIRE(vars.isVar("a.c") == false);
  REQUIRE(vars.isVar("a") == true);
  REQUIRE(vars.isVar("b") == true);
  REQUIRE(vars.isVar("c") == true);

  vars.eraseVar("a");
  vars.eraseVar("b");
  vars.eraseVar("c");
  REQUIRE(vars.isVar("a") == false);
  REQUIRE(vars.isVar("b") == false);
  REQUIRE(vars.isVar("c") == false);

  vars.addBool("a.a");
  vars.addBool("a.b");
  vars.addBool("a.c");
  vars.addBool("a");
  vars.erase("a");

  REQUIRE(vars.isVar("a.a") == false);
  REQUIRE(vars.isVar("a.b") == false);
  REQUIRE(vars.isVar("a.c") == false);
  REQUIRE(vars.isVar("a") == false);
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


