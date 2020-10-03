#pragma once
#include <filesystem>
#include <vector>
#include <string>

#include "Runnable.hpp"
#include "BinaryType.hpp"
#include "TaskStatus.hpp"
#include "ILogger.hpp"

namespace vert{

class CompileTargetTask:public Runnable{
	public:
		void operator()();

TaskStatus status;// @readonly
BinaryType type;
std::filesystem::path out,tmpdir;
std::string cc,cxx;
std::vector<std::filesystem::path> 
	srcdir,includedir,sharedlibdir,staticlibfile;
std::vector<std::string> sharedlibname;
ILogger* log=nullptr;
bool userpath=true;

#ifdef __ANDROID__
std::string ccflags=" -fPIC -O3 -std=c17 -c ";
std::string cxxflags=" -fPIC -O3 -std=c++17 -c ";
#else
std::string ccflags=" -fPIC -s -O3 -std=c17 -c ";
std::string cxxflags=" -fPIC -s -O3 -std=c++17 -c ";
#endif
std::string ldflags=" -fPIC ";

};

};

