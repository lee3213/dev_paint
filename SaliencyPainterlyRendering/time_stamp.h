#pragma once
#include "stdafx.h"

#include <ios>
#include <iostream>
#include <streambuf>
#include "time.h"
#include "define.h"

using namespace std;

class time_stamp {
private:
	 time_t s_time;
	 time_t e_time;


	tm t_s, t_e;
public:
 time_stamp() {};
	 ~time_stamp() {}
	 char s_buff[20];
	 void time_stamp::time_stamping(string stamp_tag, int start_or_end);
}
;
