#pragma once
#include "stdafx.h"

#include <iostream>
#include <fstream>
using namespace std;
#include "extern.h"
int make_batches(string image_liStrk_csv_path, string  deploy_path, string liStrk_cfg_path,
	string batch_render_method_path,
	string batch_render_para_path,
	string batch_render_para_method_path,
	string batch_render_file_path,
	string batch_render_file_para_path,
	string batch_render_file_method_path,
	string render_src,
	string exe_path);
