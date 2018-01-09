#ifndef WPP_PROFILE_H
#define WPP_PROFILE_H

struct profiler 
{
	int start_;
	int stop_;

	int start();
	int now();
	int stop();
	
	inline int used(){ return  stop_ - start_; }

	int write();

};

#endif WPP_PROFILE_H 