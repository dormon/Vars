#include<Vars/Vars.h>
#include<Vars/Resource.h>
#include<iostream>
#include<fstream>

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
