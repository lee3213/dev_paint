#pragma once
#include "stdafx.h"

#include <iostream>
#include <fstream>
using namespace std;
#include "extern.h"
int make_batches(string image_list_csv_path, string  deploy_path, string list_cfg_path,
	string batch_render_method_path,
	string batch_render_para_path,
	string batch_render_para_method_path,
	string batch_render_call_path,
	string exe_path);
