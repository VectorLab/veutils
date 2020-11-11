#include <iostream>
#include <cstring>
#include <cstddef>
#include <unistd.h>
#include <stdlib.h>
#include <functional>

// ===== begin header

#include <unordered_map>
#include <string>
#include <stack>
#include <vert/TargetBuilder.hpp>

namespace mb{

class MbLogger:public vert::ILogger{
std::stack<std::string> title;
bool beginl=true;

public:
~MbLogger();
bool is_active();
vert::ILogger* out(std::string const&);
vert::ILogger* endl();

void enter(std::string const&);
void exit();
};

enum class MbCmdEnum{
M_OTHER=0,
M_HELP,// -h --help
M_STATIC,// -static
M_SHARED,// -shared
M_OUT,// -o= --out=
M_TMP,// -t= --tmp= --temp=
M_CC,// --cc= CC=
M_CXX,// --cxx= CXX=
M_SRC,// -s= --src=
M_I,// -I= --include=
M_LL,// -L=
M_L,// -l=
M_A,// -a=
M_RPATH,// -r
M_DCCF,// --ccflags=
M_DCXXF,// --cxxflags=
M_DLDF,// --ldflags=
M_FPIC,// -fPIC
M_STD// -std=
};

std::unordered_map<std::string,MbCmdEnum> MbCmdMap{
{"-h",MbCmdEnum::M_HELP},
{"--help",MbCmdEnum::M_HELP},
{"-static",MbCmdEnum::M_STATIC},
{"-shared",MbCmdEnum::M_SHARED},
{"-o",MbCmdEnum::M_OUT},
{"--out",MbCmdEnum::M_OUT},
{"-t",MbCmdEnum::M_TMP},
{"--tmp",MbCmdEnum::M_TMP},
{"--temp",MbCmdEnum::M_TMP},
{"--cc",MbCmdEnum::M_CC},
{"--cxx",MbCmdEnum::M_CXX},
{"-s",MbCmdEnum::M_SRC},
{"--src",MbCmdEnum::M_SRC},
{"-I",MbCmdEnum::M_I},
{"--include",MbCmdEnum::M_I},
{"-L",MbCmdEnum::M_LL},
{"-l",MbCmdEnum::M_L},
{"-a",MbCmdEnum::M_A},
{"-r",MbCmdEnum::M_RPATH},
{"--ccflags",MbCmdEnum::M_DCCF},
{"--cxxflags",MbCmdEnum::M_DCXXF},
{"--ldflags",MbCmdEnum::M_DLDF},
{"-fPIC",MbCmdEnum::M_FPIC},
{"-std",MbCmdEnum::M_STD}

};

};
// ===== end header
mb::MbLogger::~MbLogger(){};
bool mb::MbLogger::is_active(){return true;};

vert::ILogger* mb::MbLogger::out(std::string const& p1){
if(this->beginl){
std::cout<<"["<<this->title.top()<<"] ";
this->beginl=false;
}
std::cout<<p1;
return this;
};

vert::ILogger* mb::MbLogger::endl(){
this->beginl=true;
std::cout<<std::endl;
return this;
};

void mb::MbLogger::enter(std::string const& p1){
this->title.push(p1);
};

void mb::MbLogger::exit(){
this->title.pop();
};

int main(int argc,char** argv){
if(argc<1){
std::cout<<"too few parameters";
return 1;
}

mb::MbLogger log;
log.enter("mbcmd");

vert::CompileTargetTask task;
task.type=vert::BinaryType::exe;
task.tmpdir=std::filesystem::temp_directory_path();
task.tmpdir/=std::to_string(static_cast<int>(getpid()));
task.cc=getenv("CC");
task.cxx=getenv("CXX");
task.log=&log;
task.userpath=false;
/*
std::function<void(vert::MbCmdEnum cmd_k,std::string const& cmd_v)> param_parser=[&](vert::MbCmdEnum cmd_k,std::string const& cmd_v){
};
*/
#define ERR_BAD_PARAM \
log.out("bad param: ");\
log.out(param_rawstr);\
log.endl();\
return 1;

#define REQUIRE_PARAM_VALUE \
if(""==param_value){\
ERR_BAD_PARAM\
}

// parse arguments
for(int i=1;i<=argc;i++){
std::string param_rawstr(argv[i]);
std::size_t param_has_value=param_rawstr.find_first_of('=');
std::string param_key;
std::string param_value="";
if(std::string::npos==param_has_value){
param_key=param_rawstr;
}else if(0==param_has_value){
ERR_BAD_PARAM
}else{
std::string param_key_temp(argv[i],param_has_value);
std::string param_value_temp(argv[i]+param_has_value+1,param_rawstr.size()-1-param_has_value);
param_key=param_key_temp;
param_value=param_value_temp;
}

if(0==mb::MbCmdMap.count(param_key)){
ERR_BAD_PARAM
}

mb::MbCmdEnum param_key_parsed=mb::MbCmdMap.at(param_key);
switch(param_key_parsed){
case mb::MbCmdEnum::M_HELP:{
log.out("todo")->endl();
return 0;
};
case mb::MbCmdEnum::M_STATIC:{
task.type=vert::BinaryType::a;
break;
};
case mb::MbCmdEnum::M_SHARED:{
task.type=vert::BinaryType::so;
break;
};
case mb::MbCmdEnum::M_OUT:{
REQUIRE_PARAM_VALUE
task.out=param_value;
break;
};
case mb::MbCmdEnum::M_TMP:{
REQUIRE_PARAM_VALUE
task.tmpdir=param_value;
break;
};
case mb::MbCmdEnum::M_CC:{
REQUIRE_PARAM_VALUE
task.cc=param_value;
break;
};
case mb::MbCmdEnum::M_CXX:{
REQUIRE_PARAM_VALUE
task.cxx=param_value;
break;
};
case mb::MbCmdEnum::M_SRC:{
REQUIRE_PARAM_VALUE
std::filesystem::path temp_1=param_value;
task.srcdir.push_back(temp_1);
break;
};
case mb::MbCmdEnum::M_I:{
REQUIRE_PARAM_VALUE
std::filesystem::path temp_1=param_value;
task.includedir.push_back(temp_1);
break;
};
case mb::MbCmdEnum::M_LL:{
REQUIRE_PARAM_VALUE
std::filesystem::path temp_1=param_value;
task.sharedlibdir.push_back(temp_1);
break;
};
case mb::MbCmdEnum::M_L:{
REQUIRE_PARAM_VALUE
task.sharedlibname.push_back(param_value);
break;
};
case mb::MbCmdEnum::M_A:{
REQUIRE_PARAM_VALUE
std::filesystem::path temp_1=param_value;
task.staticlibfile.push_back(temp_1);
break;
};
case mb::MbCmdEnum::M_RPATH:{
task.userpath=true;
break;
};
case mb::MbCmdEnum::M_DCCF:{
REQUIRE_PARAM_VALUE
task.ccflags+=param_value;
task.ccflags+=" ";
break;
};
case mb::MbCmdEnum::M_DCXXF:{
REQUIRE_PARAM_VALUE
task.cxxflags+=param_value;
task.cxxflags+=" ";
break;
};
case mb::MbCmdEnum::M_DLDF:{
REQUIRE_PARAM_VALUE
task.ldflags+=param_value;
task.ldflags+=" ";
break;
};
case mb::MbCmdEnum::M_FPIC:{
task.ccflags+="-fPIC ";
task.cxxflags+="-fPIC ";
task.ldflags+="-fPIC ";
break;
};
case mb::MbCmdEnum::M_STD:{
REQUIRE_PARAM_VALUE
task.ccflags+="-std=c";
task.ccflags+=param_value;
task.ccflags+=" ";
task.cxxflags+="-std=c++";
task.cxxflags+=param_value;
task.cxxflags+=" ";
break;
};
case mb::MbCmdEnum::M_OTHER:
default:
{
ERR_BAD_PARAM
};
}
}

log.out("launch make...");
log.endl();
log.enter("make");

task();

log.exit();
std::filesystem::remove_all(task.tmpdir);
log.out("done.");
log.endl();
};


