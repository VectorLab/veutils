#pragma once

namespace vert{
class Runnable{
	public:
virtual ~Runnable()=default;
/* usage: 
 * std::thread mythread(this_ptr);
 */
virtual void operator()()=0;
};
};

