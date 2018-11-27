#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <comdef.h> 
#include <windows.h>
#include "define.h"
#include "json/json.h"

using namespace std;
#include "extern.h"

#include "dir.h"

int ts[][MAX_DEPTH] = {
	{ 4,4,4,4,4,4,4,4,4,4,4,1,1 },
	{ 6,6,4,4,4,4,4,4,4,4,4,1,1 },
	{ 8,8,6,6,6,6,6,6,6,6,6,1,1 },
	{ 6,6,6,6,6,6,6,6,6,6,6,1,1 },
	{ 8,8,8,6,6,6,6,6,6,6,6,1,1 },
	{ 8,8,8,8,8,8,8,8,8,8,8,1,1 }
};

int f_count;
int write_try_scale_json_content(string from_jsonfolderPath) {
	Json::StreamWriterBuilder wbuilder;
	Json::Value root;
	string json_file_name;
	//int f_count;
	string f_path_name;
	int sizeof_ts_y = sizeof(ts);
	int sizeof_ts_x = sizeof(ts[0]);
	string f_name;
	for(int ts_y = 0; ts_y < sizeof_ts_y; ts_y++)
	{
		for (int ts_x = 0; ts_x < sizeof_ts_x; ts_x) {
			if ( ts_x < 4)
				f_name += to_string(ts[ts_x][ts_y]);
			string a;
			a = "a" + to_string(ts_x);
			root[a] = ts[ts_x][ts_y];;
		}
	}
	
	root["count_y"] = sizeof_ts_x;
	root["count_x"] = sizeof_ts_x;
	json_file_name = f_name+".json";
	std::string outputConfig = Json::writeString(wbuilder, root);
	f_path_name = from_jsonfolderPath + "\\" + json_file_name;
	std::ofstream file_json;
	file_json.open(f_path_name.c_str());
	file_json << outputConfig;
	
	file_json.close();
	return 1;
}