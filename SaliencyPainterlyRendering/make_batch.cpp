#include "stdafx.h"

#include <iostream>
#include <fstream>
using namespace std;
#include "extern.h"

int  file_read(string f_name_list_csv, string str_file[], int &file_cnt,int EOF_FLAG=0) {
	file_cnt = 0;
	fstream file_list_csv;

	file_list_csv.open(f_name_list_csv.c_str(),ios::in);

	if (!file_list_csv)  // operator! is used here
	{
		std::cout << f_name_list_csv<<":: File opening failed\n";
		return -1;
	}

	if (file_list_csv.fail()) {
		 cout << f_name_list_csv << ":: file_ fail " << endl;
		return -1;
	}
	while (!file_list_csv.eof()) {

		string str_csv_name = "";
		file_list_csv >> str_csv_name;
		if (str_csv_name.size() != 0) {
			str_file[file_cnt] = str_csv_name;
			file_cnt++;
		}
		else {
			if (!file_list_csv.eof()) {
				cout << "[" << str_csv_name.size() << "]" << endl;
				cout << __FUNCTION__ << " Error size == 0) " << f_name_list_csv << "::" << file_cnt << endl;
			}
			continue;
		}
	}

	file_list_csv.close();
	//for (int i = 0; i < file_cnt; i++)
	//{
	//	cout << setw(4) << i << " : " << str_file[i] << endl;
//	}
	return file_cnt;
}//end of function
int  file_write(string f_name, string _file_str[], int _file_cnt) {
	fstream file_;
	
	file_.open(f_name.c_str(), ios::out);

	if (!file_)  // operator! is used here
	{
		std::cout << f_name << ":: File opening failed\n";
		return -1;
	}

	if (file_.fail()) {
		cout << f_name << ":: file_ fail " << endl;
		return -1;
	}
	for(int i=0;i<_file_cnt;i++){
	
		
		file_<< _file_str[i]<<endl;
		
	}

	file_.close();
	
	return _file_cnt;
}//end of function
string f_str[MAX_F_CNT];
int bat_file_write(string f_name, string image_file[], int file_image_cnt, string  exe_path, 
	int  file_meta_cnt, string  meta_cfg_file[],string render_src) {
	int f_cnt = 0;
	int f_cnt_writed = 0;
	for(int i_f=0; i_f<file_image_cnt;i_f++)
		for (int m_f = 0; m_f < file_meta_cnt; m_f++) {
			f_str[f_cnt] = exe_path + " " + render_src+"\\"+image_file[i_f] + " " + meta_cfg_file[m_f];
			f_cnt++;
			f_cnt_writed++;
			if (MAX_F_CNT <= f_cnt) {
				
				file_write(f_name, f_str, f_cnt);
				f_cnt = 0;
			}
		}
	
	return f_cnt_writed;
}
int call_file_write(string f_name, string image_file[], int file_image_cnt,
	string  call_path, int  file_meta_cnt, string  meta_cfg_file[],string render_src) {
	int f_cnt = 0;

	for (int i_f = 0; i_f<file_image_cnt; i_f++)
		for (int m_f = 0; m_f < file_meta_cnt; m_f++) {
			f_str[f_cnt] = call_path + " " + render_src+"\\"+image_file[i_f] + " " + meta_cfg_file[m_f];
			f_cnt++;
			if (MAX_F_CNT <= f_cnt) {
				break;
			}
		}

	file_write(f_name, f_str, f_cnt);
	return f_cnt;
}


string image_csv_file[100];
int image_csv_cnt = 0;
int find_image_idx(string lst_name)
{
	int i;
	for (i = 0; i < image_csv_cnt; i++) {
		if (lst_name == image_csv_file[i])
			break;
	}
	if (i == image_csv_cnt) return -1;
	return i;
}
string method_para_cfg_file[1000];
int make_batches(string image_list_csv_path, string  deploy_path, string list_cfg_path,
	string batch_render_method_path,
	string batch_render_para_path,
	string batch_render_para_method_path,
	string batch_render_file_path,
	string batch_render_file_para_path,
	string batch_render_file_method_path,
	string _render_src,
	string exe_path)

 {
	int ret;

	string f_name_list_csv = image_list_csv_path + "\\image_lst.csv";
	ret=file_read( f_name_list_csv, image_csv_file,image_csv_cnt);
	
	string meta_cfg_file[100];
	int file_meta_cnt=0;
	string f_name_meta_cfg = list_cfg_path + "\\para_meta.cfg";
	ret = file_read(f_name_meta_cfg, meta_cfg_file,file_meta_cnt);
	
	string method_cfg_file[100];
	int file_method_cnt = 0;
	string f_name_method_cfg = list_cfg_path + "\\method_meta.cfg";
	ret = file_read(f_name_method_cfg, method_cfg_file, file_method_cnt);

	
	int method_para_cnt = 0;
	string f_name_method_para_cfg = list_cfg_path + "\\method_para_meta.cfg";
	
	ret = file_read(f_name_method_para_cfg, method_para_cfg_file, method_para_cnt);

	int meta_json_cnt =0;
	string f_name_json;
	string meta_json_file[1000];
//	for (int json_file_idx = 0; json_file_idx < file_meta_cnt; json_file_idx++) {
//		f_name_json = list_cfg_path + "\\"+meta_cfg_file[json_file_idx]; 
//		ret = file_read(f_name_json, meta_json_file,meta_json_cnt);
//	}
	int image_file_cnt = 0;
	string f_name_image;
	string image_file[1000];
//	for (int image_idx = 0; image_idx < file_image_csv_cnt; image_idx++) {
//		f_name_image = image_list_csv_path + "\\" + image_csv_file[image_idx];
//		ret = file_read(f_name_image, image_file, image_file_cnt);
//	}
	
	int method_json_cnt = 0;
	string f_name_method_json;
	string method_json_file[1000];
//	for (int method_json_file_idx = 0; method_json_file_idx < file_meta_cnt; method_json_file_idx++) {
//		f_name_method_json = list_cfg_path + "\\" + method_cfg_file[method_json_file_idx];
//		ret = file_read(f_name_method_json, method_json_file, file_method_json_cnt);
//	}
	int para_method_json_cnt = 0;
	string f_name_para_method_json;
	string para_method_json_file[1000];
	//image_file, image_file_cnt
	//meta_json_file,meta_json_cnt
	//method_json_file, file_method_json_cnt

	string f_name_para_bat;
	for (int meta_file_idx = 0; meta_file_idx < file_meta_cnt; meta_file_idx++) {
		
					f_name_json = list_cfg_path + "\\"+meta_cfg_file[meta_file_idx]; 

					ret = file_read(f_name_json, meta_json_file,meta_json_cnt);

					for (int image_idx = 0; image_idx < image_csv_cnt; image_idx++) {
						f_name_image = image_list_csv_path + "\\" + image_csv_file[image_idx];
			
						ret = file_read(f_name_image, image_file, image_file_cnt);

						f_name_para_bat = batch_render_para_path + "\\" + image_csv_file[image_idx] + "_" + meta_cfg_file[meta_file_idx] + ".bat";
						int flag = bat_file_write(f_name_para_bat, image_file, image_file_cnt, exe_path,
							meta_json_cnt, meta_json_file,
							_render_src);
					
					}
	
		}
	

	string f_name_method_bat;

	for (int method_file_idx = 0; method_file_idx < file_method_cnt; method_file_idx++) {
		f_name_method_json = list_cfg_path + "\\" + method_cfg_file[method_file_idx];
				ret = file_read(f_name_method_json, method_json_file, method_json_cnt);
		
			for (int image_idx = 0; image_idx < image_csv_cnt; image_idx++) {
				f_name_image = image_list_csv_path + "\\" + image_csv_file[image_idx];
				ret = file_read(f_name_image, image_file, image_file_cnt);

				f_name_method_bat = batch_render_method_path + "\\" + image_csv_file[image_idx] + "_" + method_cfg_file[method_file_idx] + ".bat";


				int flag = bat_file_write(f_name_method_bat, image_file, image_file_cnt, exe_path,
					method_json_cnt, method_json_file,
					_render_src);
			}
	
	}

	string f_name_para_method_cfg;
	for (int para_method_idx = 0; para_method_idx < method_para_cnt; para_method_idx++) {

		f_name_para_method_cfg = list_cfg_path + "\\" + method_para_cfg_file[para_method_idx];

		ret = file_read(f_name_para_method_cfg, para_method_json_file, para_method_json_cnt);

		for (int image_idx = 0; image_idx < image_csv_cnt; image_idx++) {
			f_name_image = image_list_csv_path + "\\" + image_csv_file[image_idx];

			ret = file_read(f_name_image, image_file, image_file_cnt);

			f_name_para_bat = batch_render_para_method_path + "\\" + image_csv_file[image_idx] + "_" + method_para_cfg_file[para_method_idx] + ".bat";
			int flag = bat_file_write(f_name_para_bat, image_file, image_file_cnt, exe_path,
				para_method_json_cnt, para_method_json_file,
				_render_src);

		}
		int find = find_image_idx("paper.lst");
		if (find != -1) {
			f_name_image = image_list_csv_path + "\\" + image_csv_file[find];
			ret = file_read(f_name_image, image_file, image_file_cnt);
			string image_file_1[1];
			
			string f_name_para_method_cfg;
			for (int para_method_idx = 0; para_method_idx < method_para_cnt; para_method_idx++) {

				f_name_para_method_cfg = list_cfg_path + "\\" + method_para_cfg_file[para_method_idx];

				ret = file_read(f_name_para_method_cfg, para_method_json_file, para_method_json_cnt);

		
				for (int k = 0; k < image_file_cnt; k++) {
					f_name_para_bat = batch_render_file_path + "\\" + image_file[k] 
						+ "_" + method_para_cfg_file[para_method_idx] + ".bat";
					image_file_1[0] = image_file[k];
					int flag = bat_file_write(f_name_para_bat, image_file_1, 1, exe_path,
						para_method_json_cnt, para_method_json_file,
						_render_src);

				}

			}
		//	string f_name_para_method_cfg;
			for (int para_idx = 0; para_idx < file_meta_cnt; para_idx++) {

				f_name_json = list_cfg_path + "\\" + meta_cfg_file[para_idx];

				ret = file_read(f_name_json, meta_json_file, meta_json_cnt);
				

				for (int k = 0; k < image_file_cnt; k++) {
					f_name_para_bat = batch_render_file_para_path + "\\" + image_file[k]
						+ "_" + meta_cfg_file[para_idx] + ".bat";
					image_file_1[0] = image_file[k];
					int flag = bat_file_write(f_name_para_bat, image_file_1, 1, exe_path,
						meta_json_cnt, meta_json_file,
						_render_src);

				}

			}
			for (int method_idx = 0; method_idx < file_method_cnt; method_idx++) {

				f_name_json = list_cfg_path + "\\" + method_cfg_file[method_idx];

				ret = file_read(f_name_json, method_json_file, method_json_cnt);


				for (int k = 0; k < image_file_cnt; k++) {
					f_name_method_bat = batch_render_file_method_path + "\\" + image_file[k]
						+ "_" + method_cfg_file[method_idx] + ".bat";
					image_file_1[0] = image_file[k];
					int flag = bat_file_write(f_name_method_bat, image_file_1, 1, exe_path,
						method_json_cnt, method_json_file,
						_render_src);

				}

			}
		}//find 
	}
	

	return 1;
}
/*
int make_a_batch(string lst_f_path, string list_cfg_path, string b_render_path, int para_cnt, string para[], int k_depth, string tag) {

	std::ofstream file_meta_cfg;
	string f_name_meta_cfg;
	f_name_meta_cfg = list_cfg_path + string("meta_cfg.cfg");
	file_meta_cfg.open(f_name_meta_cfg.c_str());


	string lst_f_name = lst_f_path + string("image_lst.lst");
	std::ifstream file_lst_name;
	std::ifstream file_name;
	string lst_list_name;
	string list_name2[100];
	string f_name[100];
	string lst_f_name_2;
	//int i = 0;
	int f_cnt = 0;
	int list_cnt;
	string cmd;
	string img_name;
	file_lst_name.open(lst_f_name.c_str());
	if (!file_lst_name)  // operator! is used here
	{
		std::cout << "file_lst_name File opening failed\n";
		return -1;
	}

	if (file_lst_name.fail()) {
		cout << "file_lst_name fail " << endl;
		return -1;
	}
	while (!file_lst_name.eof()) {
		f_cnt = 0;
		lst_list_name = "";
		file_lst_name >> lst_list_name;
		if (lst_list_name.size() > 1) {
			f_name[f_cnt] = lst_list_name;
			f_cnt++;
		}
		else {
			cout << "size <= 1 : 1) " << lst_f_name << "  " <<  "   " << b_render_path << "  " << lst_list_name.size() << "::::" << f_cnt << " " << tag << endl;
			continue;
		}
		//	cout << setw(5) << i << list_name << endl;
		//i++;
		lst_f_name_2 = lst_f_path + lst_list_name;
		file_name.open(lst_f_name_2.c_str());
		if (!file_name)  // operator! is used here
		{
			std::cout << "file_name File opening failed\n";
			return -1;
		}

		if (file_name.fail()) {
			cout << "file_name fail " << endl;
			return -1;
		}
		std::ofstream bat_call;
		string bat_cmd;
		string bat_f;
		bat_f = "\\batch\\" + to_string(k_depth) + "_" + lst_list_name + "_Itti_Sobel.bat";
		bat_call.open(bat_f.c_str());
		if (!bat_call)  // operator! is used here
		{
			std::cout << "bat_call File opening failed\n";
			return -1;
		}

		if (bat_call.fail()) {
			cout << "bat_call fail " << endl;
			return -1;
		}
		list_cnt = 0;
		while (!file_name.eof()) {
			img_name = "";
			file_name >> img_name;
			if (img_name.size() > 1) {
				list_name2[list_cnt] = img_name;
				list_cnt++;
			}
			else {
				cout << "size <= 1 : 2) " << bat_f << "  " << img_name.size() << "::::" << list_cnt << " " << tag << endl;
				continue;
			}

			//cout << list_name2[list_cnt]<<endl;
			//add_batch_a("\\batch", b_render_path, list_name2[list_cnt], para, para_cnt);
			std::ofstream file_call;
			string bat_fcall_name;
			if (img_name == lst_list_name) {
				cout << img_name << ", " << list_cnt << endl;
				continue;
			}
			bat_fcall_name = "\\batch\\" + to_string(k_depth) + "_" + lst_list_name + "_" +
				img_name + ".bat";
			file_call.open(bat_fcall_name.c_str());
			string T_bat_fcall_name = "\\batch\\" + to_string(k_depth) + "_" + lst_list_name + "_" +
				img_name + "_smItti_Sobel.bat";
			std::ofstream T_file_call;
			if (k_depth == 0)
				T_file_call.open(T_bat_fcall_name.c_str());
			for (int k = 0; k < para_cnt; k++) {
				if (k_depth == 1)
					cmd = "call " + b_render_path + "\\" + tag + para[k] + ".bat \\render\\src\\" + img_name;
				else cmd = "call " + b_render_path + "\\" + para[k] + ".bat " + img_name;
				file_call << cmd << endl;
				if (k_depth == 0) {
					bat_cmd = "call " + b_render_path + "\\" + para[k] + "_smItti.bat \\render\\src\\" + img_name;
					bat_call << bat_cmd << endl;
					T_file_call << bat_cmd << endl;

					bat_cmd = "call " + b_render_path + "\\" + para[k] + "_smSobel.bat \\render\\src\\" + img_name;
					bat_call << bat_cmd << endl;
					T_file_call << bat_cmd << endl;
				}
			}
			file_call.close();
			if (k_depth == 0)
				T_file_call.close();
		}
		file_name.close();
		bat_call.close();
	}
	file_lst_name.close();
	return 1;
}
*/
int  get_para(string lst_f_para, string para[], int &para_cnt) {
	std::ifstream file_para;
	file_para.open(lst_f_para.c_str());
	if (!file_para)  // operator! is used here
	{
		std::cout << "File opening failed\n" << lst_f_para << endl;
		return -1;
	}

	if (file_para.fail()) {
		cout << "fail " << endl;
		return -1;
	}
	string str;
	while (!file_para.eof()) {
		if (file_para.fail()) {
			cout << "fail " << endl;
			return -1;
		}
		str = "";
		file_para >> str;
		if (str.size() > 1) {
			para[para_cnt] = str;
			para_cnt++;
		}
		else {
			cout << "size <= 1 : 3) " << lst_f_para << "   " << para[para_cnt].size() << "::::" << para_cnt << endl;
		}
	}
	file_para.close();
	return para_cnt;
}