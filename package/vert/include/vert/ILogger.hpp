#pragma once
#include <string>

namespace vert{
class ILogger{
	public:
virtual ~ILogger()=default;

virtual bool is_active()=0;
virtual ILogger* out(std::string const&)=0;
virtual ILogger* endl()=0;
};
};

