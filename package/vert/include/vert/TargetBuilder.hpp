#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_set>

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
std::unordered_set<std::string> ccft={".c"},cxxft={".cpp"};

#ifdef __ANDROID__
std::string ccflags=" -c ";
std::string cxxflags=" -c ";
#else
std::string ccflags=" -c ";
std::string cxxflags=" -c ";
#endif
std::string ldflags=" ";

};

};

