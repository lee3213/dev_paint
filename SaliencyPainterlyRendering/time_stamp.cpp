
#pragma once
#include "stdafx.h"

#include <ios>
#include <iostream>
#include <streambuf>
#include "define.h"
#include "time_stamp.h"
using namespace std;

void time_stamp::time_stamping( string stamp_tag, int start_or_end) {

	if (start_or_end == TIME_STAMP_START) {
		time(&s_time);
		localtime_s(&t_e, &s_time);
		strftime(s_buff, 20, "%Y-%m-%d %H:%M:%S", &t_e);

	
	}
	else {


	}
}
