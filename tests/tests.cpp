#include<Vars/Vars.h>
#include<Vars/Resource.h>
#include<iostream>
#include<fstream>
#include<memory>

#define CATCH_CONFIG_MAIN
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
