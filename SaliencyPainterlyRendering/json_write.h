#pragma once
#include "stdafx.h"
#include "json/json.h"
using namespace std;
#include "extern.h"
int  json_write(string path);
int  json_write_method(string deploy_path, string batch_path, string exe,string list_path);
void add_batch_a(string batch_path ,string b_render_path,string  list_name2, string para[], int para_cnt);