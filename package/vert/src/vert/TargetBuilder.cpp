
#include <cstdlib>

#include <vert/TargetBuilder.hpp>
#include <vert/FolderRecurser.hpp>

void vert::CompileTargetTask::operator()(){
this->log->out("load arguments...");

this->status=TaskStatus::pending;
if(!std::filesystem::is_directory(this->tmpdir)){
	std::filesystem::remove_all(this->tmpdir);
}
std::filesystem::create_directories(this->tmpdir);
std::string v1_cc_args="",v2_cxx_args="";

for(auto& v4_for_v:this->includedir){
v1_cc_args+=" -I";
v1_cc_args+=v4_for_v.c_str();
v2_cxx_args+=" -I";
v2_cxx_args+=v4_for_v.c_str();
}
v1_cc_args+=" ";
v2_cxx_args+=" ";
v1_cc_args+=this->ccflags;
v2_cxx_args+=this->cxxflags;
v1_cc_args+=" ";
v2_cxx_args+=" ";

std::string v3_of(" ");

std::function<bool(std::filesystem::path const&)> v9=
[&](std::filesystem::path const&v5_cf){
if(!std::filesystem::is_regular_file(v5_cf)){ return false; }
std::string v6_ft(v5_cf.extension().c_str());
std::string v10(v5_cf.c_str());
if(1==this->cxxft.count(v6_ft)){
this->log->out("CXX ")->out(v10)->endl();

std::string v7_on("_o_");
v7_on+=std::to_string(v3_of.size());
v7_on+="_.o";
std::filesystem::path v7_of=this->tmpdir;
v7_of/=v7_on.c_str();
std::string v8_c=this->cxx;
v8_c+=" ";
v8_c+=v10;
v8_c+=" -o ";
v8_c+=v7_of.c_str();
v8_c+=v1_cc_args;

int v9_nr=std::system(v8_c.c_str());
if(0!=v9_nr){
this->status = TaskStatus::fail;
this->log->out("compile failed: ")->out(std::to_string(v9_nr))->endl()
->out(v8_c)->endl();
return true;
}

v3_of+=v7_of.c_str();
v3_of+=" ";
}else if(1==this->ccft.count(v6_ft)){
this->log->out("CC ")->out(v10)->endl();

std::string v7_on("_o_");
v7_on+=std::to_string(v3_of.size());
v7_on+="_.o";
std::filesystem::path v7_of=this->tmpdir;
v7_of/=v7_on.c_str();
std::string v8_c=this->cc;
v8_c+=" ";
v8_c+=v10;
v8_c+=" -o ";
v8_c+=v7_of.c_str();
v8_c+=v2_cxx_args;                                            
int v9_nr=std::system(v8_c.c_str());
if(0!=v9_nr){
this->status = TaskStatus::fail;
this->log->out("compile failed: ")->out(std::to_string(v9_nr))->endl()
->out(v8_c)->endl();
return true;
}
v3_of+=v7_of.c_str();
v3_of+=" ";
}
return false;
}
;

this->log->out("ok")->endl();

for(auto& v4_for_v:this->srcdir){
if(std::filesystem::is_directory(v4_for_v)){
vert::FolderRecurser::foreach_pcall(v4_for_v,v9);
}else if(std::filesystem::is_regular_file(v4_for_v)){
v9(v4_for_v);
}else{
continue;
}

if(TaskStatus::fail==this->status){
std::filesystem::remove_all(this->tmpdir);
this->log->out("build failed")->endl();
return;
}
}

if(" "==v3_of){
std::filesystem::remove_all(this->tmpdir);
this->log->out("no input files")->endl();
return;
}

std::string v6(this->out.c_str());
this->log->out("LD ")->out(v6)->endl();

std::string v4_c_ld=this->cxx;
v4_c_ld+=" ";
v4_c_ld+=v3_of;
v4_c_ld+=" ";
for(auto& v5_sf:this->staticlibfile){
v4_c_ld+=v5_sf.c_str();
v4_c_ld+=" ";
}
v4_c_ld+="-o ";
v4_c_ld+=v6;
v4_c_ld+=" ";
v4_c_ld+=this->ldflags;
v4_c_ld+=" ";
switch(this->type){
case BinaryType::so:{
v4_c_ld+="-shared ";
if(this->userpath){
v4_c_ld+="-Wl,-rpath='$ORIGIN' ";
}
break;
};
case BinaryType::a:{
v4_c_ld+="-static ";
break;
};
case BinaryType::exe:{
if(this->userpath){
v4_c_ld+=" -Wl,-rpath='$ORIGIN/../lib' ";
}
};
default:{ break; };
}
for(auto& v5_sf:this->sharedlibdir){
v4_c_ld+="-L";
v4_c_ld+=v5_sf.c_str();
v4_c_ld+=" ";
}
for(auto& v5_sf:this->sharedlibname){
v4_c_ld+="-l";
v4_c_ld+=v5_sf.c_str();
v4_c_ld+=" ";
}

int v5_r=std::system(v4_c_ld.c_str());
if(0!=v5_r){
this->status = TaskStatus::fail;
this->log->out("link failed: ")->out(std::to_string(v5_r))->endl()
->out(v4_c_ld)->endl();
}else{
this->status=TaskStatus::done;
this->log->out("done.")->endl();
std::filesystem::remove_all(this->tmpdir);
}
};

