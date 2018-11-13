#include "stdafx.h"
#include "json/json.h"
#include <iostream>
#include <fstream>
using namespace std;
#include "extern.h"
#include <comdef.h> 
#include <windows.h>
#include "dir.h"
#include "extern.h"
// ...
// At application shutdown to make the new configuration document:
// Since Json::Value has implicit constructor for all value types, it is not
// necessary to explicitly construct the Json::Value object:
//	root["encoding"] = getCurrentEncoding();
//	root["indent"]["length"] = getCurrentIndentLength();
//	root["indent"]["use_space"] = getCurrentIndentUseSpace();
int sthresh[] = { 60,70,80 };
int _grid[] = { 10,20 };
int depth[] = { 10 };
int _br[] = { 10 }; //g_BrushMinSize
int _at[] = { 5 }; //attach brush size

//string _re[MAX_DEPTH] = { "Sobel","Saliency","Twopass" };
//string _str_mm[MAX_DEPTH] = { "Attach","Merge" };

//int saliency_method_cnt = sizeof(g_saliency_method_str) / sizeof(string);
int QT_N[] = { 1};
//int _mm[] = { 1,2 };// "attach", tag_union
;
int _ps[] = { 10 };//"g_paint_area_scale"



//int _bts[] = {250 };//brush transparecy threshold
int br, th, de, gr, sm, qt, re, mm, gth, ps;// , bts;

void get_json_name(string & file_name) {
	 file_name = "M" + g_saliency_method_str[sm]+"_s" + to_string(sthresh[th]) + "_d" + to_string(depth[de]) +
		"_g" + to_string(_grid[gr]) + "_b" + to_string(_br[br]) + "_N" + to_string(QT_N[qt])
		+ "_ps" + to_string(_ps[ps])
		//	 + "_ts" + to_string(_bts[bts])
		 + "_copy.json";
	
}
#define MAX_PARA 100
int howmany;
string json_file_name;
int f_count=0;
int write_json_content(string from_jsonfolderPath) {
	Json::StreamWriterBuilder wbuilder;
	Json::Value root;
	
	//int f_count;
	string f_path_name;
	
	root["root_path"] = "/rst";
	root["root_path_win"] = "\\rst";
	root["g_paint_method"] = "copy";//copy alpha
	root["g_BrushMinSize"] = _br[0]; //15
	root["g_BrushAttachSize"] = _at[0];//5
									   //		root["g_brush_Ts"] = _bts[bts];//brush tranparency threshold brush
	root["end"] = "end";
	root["g_paint_area_scale"] = _ps[ps];//
										 //root["g_merge_method"] = _str_mm[mm];
										 //	root["g_grid_threshold"] = _gth[gth];
	root["QT_avgSThreshold"] = sthresh[th];//15, 25  used in divide for  AvgS
	root["g_depth_Threshhold"] = depth[de]; //5,7
	root["g_gridsize"] = _grid[gr];	//5,10 used in divide

	root["g_saliency_method"] = g_saliency_method_str[sm];//wo saliency pregraph 

	root["g_QT_method_N"] = QT_N[qt]; //1,2,3,4...6
	std::string outputConfig = Json::writeString(wbuilder, root);
	f_path_name = from_jsonfolderPath + "\\" + json_file_name;
	std::ofstream file_json;
	file_json.open(f_path_name.c_str());
	file_json << outputConfig;
	f_count++;
	if (!(f_count % 100)) {
		std::cout << g_saliency_method_str[sm] << " " << f_count << " / " << howmany << " : " << f_count / howmany*100. << endl;
	}//percent
	file_json.close();
	return 1;
}
//int br, th, de, gr, sm, qt, re, mm, gs, ps;
int  json_write_method(string from_jsonfolderPath,//render/deployument
	string list_cfg_path)
 {
	string cfg_file_name;

	int fail_flag = 0;

	int n_bth = sizeof(_br) / sizeof(int);
	int n_sth = sizeof(sthresh) / sizeof(int);
	int n_de = sizeof(depth) / sizeof(int);
	int n_gr = sizeof(_grid) / sizeof(int);

	int n_qt = sizeof(QT_N) / sizeof(int);
	//int n_re = _re_cnt;
//	int n_mm = 1;//_str_mm_cnt;
	int n_ps = sizeof(_ps) / sizeof(int);

	howmany = MAX_SALIENCY* n_sth * n_de * n_gr*n_qt*n_ps*n_bth;
	int Max_PARA = n_sth*n_gr;
	if (Max_PARA > MAX_PARA) {
		cout << "Max PAra " << Max_PARA << "Exceeded " << MAX_PARA << endl;
		return -1;


	}
	std::ofstream file_all_cfg;
	string f_name_all_cfg;
	
	std::ofstream file_para_meta_cfg;
	string f_name_para_meta_cfg;

	std::ofstream file_method_meta_cfg;
	string f_name_method_meta_cfg;

	std::ofstream file_para_method_meta_cfg;
	string f_name_method_para_meta_cfg;

	f_name_all_cfg = list_cfg_path + "\\" + string("para_all_cfg.cfg");
	file_all_cfg.open(f_name_all_cfg.c_str());
	if (!file_all_cfg.is_open()) {
		return -1;
	}
	f_name_para_meta_cfg = list_cfg_path + "\\"+string("para_meta.cfg");
	file_para_meta_cfg.open(f_name_para_meta_cfg.c_str());
	if (!file_para_meta_cfg.is_open()) {
		return - 1;
	}

	
	f_name_method_meta_cfg = list_cfg_path + "\\" + string("method_meta.cfg");
	file_method_meta_cfg.open(f_name_method_meta_cfg.c_str());
	if (!file_method_meta_cfg.is_open()) {
		return -1;
	}
	int method_para_cnt = 0;
	f_name_method_para_meta_cfg = list_cfg_path + "\\method_para_meta.cfg";
	file_para_method_meta_cfg.open(f_name_method_para_meta_cfg.c_str());
	if (!file_para_method_meta_cfg.is_open()) {
		return -1;
	}

	int f_loc;

	std::ofstream file_para_cfg[MAX_PARA];
	string f_name_para_cfg[MAX_PARA];

	std::ofstream file_para_method_cfg[MAX_PARA];
	string f_name_para_method_cfg[MAX_PARA];

	int f_max = n_sth*n_gr;//

	for (th = 0; th < n_sth; th++) //avg S thresh
		for (gr = 0; gr < n_gr; gr++) {
			f_loc = th*n_gr + gr;
			f_name_para_cfg[f_loc] ="s_" + to_string(sthresh[th]) +
				"_g" + to_string(_grid[gr]) + ".cfg";
			file_para_cfg[f_loc].open(list_cfg_path +"\\"+ f_name_para_cfg[f_loc]);
			cout << setw(5)<<f_loc<<" : "<<list_cfg_path + "\\" + f_name_para_cfg[f_loc] << endl;
			if (!file_para_cfg[f_loc].is_open())  // operator! is used here
			{
				std::cout << f_name_para_cfg << ":: File opening failed\n";
				return -1;
			}
			else file_para_meta_cfg << f_name_para_cfg[f_loc] << endl;


			for (sm = 0; sm < MAX_SALIENCY; sm++) {
				int mf_loc = th*n_gr*MAX_SALIENCY + gr*MAX_SALIENCY+sm;
				f_name_para_method_cfg[mf_loc] = "M"+g_saliency_method_str[sm]+"s_" + to_string(sthresh[th]) +
					"_g" + to_string(_grid[gr]) + ".cfg";
				file_para_method_cfg[mf_loc].open(list_cfg_path + "\\" + f_name_para_method_cfg[mf_loc]);
				cout << setw(5) << mf_loc << " : " << list_cfg_path + "\\" + f_name_para_method_cfg[mf_loc] << endl;
				if (!file_para_method_cfg[mf_loc].is_open())  // operator! is used here
				{
					std::cout << f_name_para_method_cfg << ":: File opening failed\n";
					return -1;
				}
				else file_para_method_meta_cfg << f_name_para_method_cfg[mf_loc] << endl;


			}





	}

	for (th = 0; th < n_sth; th++) //avg S thresh
		for (gr = 0; gr < n_gr; gr++)  //grid min size
		{
			f_loc = th*n_gr + gr;
			for (de = 0; de < n_de; de++) //depth
				for (br = 0; br < n_bth; br++)//brush min size
					for (qt = 0; qt < n_qt; qt++) //N QTn{
						for (ps = 0; ps < n_ps; ps++) {//ps //scale
							for (sm = 0; sm < MAX_SALIENCY; sm++) { //saliency method
								get_json_name(json_file_name);
								int mf_loc = th*n_gr*MAX_SALIENCY + gr*MAX_SALIENCY + sm;

								if (!file_para_method_cfg[mf_loc].is_open())  // operator! is used here
								{
									std::cout << f_name_para_method_cfg << ":: File opening failed\n";
									return -1;
								}

								else file_para_method_cfg[mf_loc] << from_jsonfolderPath + "\\" + json_file_name << endl;
								
								if ( file_para_cfg[f_loc].is_open()) {
									file_para_cfg[f_loc] << from_jsonfolderPath + "\\" + json_file_name << endl;
								}
								else {
									cout << "[" << f_name_para_cfg[f_loc] << "] not open" << endl;
									fail_flag--;
									break;
								}

								if (file_para_cfg[f_loc].is_open()) {
									file_para_cfg[f_loc] << from_jsonfolderPath + "\\" + json_file_name << endl;
								}
								else {
									cout << "[" << f_name_para_cfg[f_loc] << "] not open" << endl;
									fail_flag--;
									break;
								}

							}
						}

		}
	for (th = 0; th < n_sth; th++) //avg S thresh
		for (gr = 0; gr < n_gr; gr++) {
			f_loc = th*n_gr + gr;
			
			file_para_cfg[f_loc].close();
			for (sm = 0; sm < MAX_SALIENCY; sm++) {
				int mf_loc = th*n_gr*MAX_SALIENCY + gr*MAX_SALIENCY + sm;
				file_para_method_cfg[mf_loc].close();
			}
		}
	std::ofstream file_method_cfg;
	//clock deployment 
	for (sm = 0; sm < MAX_SALIENCY; sm++) { //saliency method
		string f_name_method_cfg = list_cfg_path + "\\" + g_saliency_method_str[sm] + ".cfg";
	
		file_method_cfg.open(f_name_method_cfg.c_str());

		file_method_meta_cfg << g_saliency_method_str[sm] +".cfg"<< endl;//MAY BE REMOED
		for (th = 0; th < n_sth; th++) //avg S thresh
			for (gr = 0; gr < n_gr; gr++) { 
			
				for (de = 0; de < n_de; de++) //depth
						for (br = 0; br < n_bth; br++)//brush min size
						for (qt = 0; qt < n_qt; qt++) //N QTn{
							for (ps = 0; ps < n_ps; ps++) {//ps //scale
								
								get_json_name(json_file_name);
							
								if (file_all_cfg.is_open()) {
									file_all_cfg << from_jsonfolderPath+"\\"+json_file_name << endl;
									
								}
								else {
									cout << "[" << f_name_all_cfg << "] not open" << endl;
									fail_flag--;
									break;
								}
								
								get_json_name(json_file_name);
								if (file_method_cfg.is_open()) {
									file_method_cfg << from_jsonfolderPath+"\\"+json_file_name << endl;
								}
								else {
									cout << "[" << f_name_method_cfg << "] not open" << endl;
									fail_flag--;
									break;
								}
								
								write_json_content(from_jsonfolderPath);
					

							
							}// rest of parameter
			}//for dth and gid min
			file_method_cfg.close();
		}//for sm
			//file_all_para.close();
			//file_para_meta.close();
	file_all_cfg.close();
	file_para_meta_cfg.close();
	file_method_meta_cfg.close();
	std::cout << __FUNCTION__<<"    method " <<f_count<<endl;
	return fail_flag;
}
void add_batch_a(string batch_path, string b_render_path,string  list_name2, string para[], int para_cnt) {

	std::ofstream file_id;
	string bat_f_name;
	for (int i = 0; i < para_cnt;i++) {
		bat_f_name = batch_path + string("\\") + para[i]+ string("_") +
			list_name2+".bat";
		file_id.open(bat_f_name.c_str());
		file_id << "call " + b_render_path + para[i] + ".bat "+list_name2;
		file_id.close();
	}
}

void create_batch_a(string from_jsonfolderPath, string batch_dir,
	string exe_path, string file_folderPath, string image_file_name, string fname)

{

	char fileFound[256];

	WIN32_FIND_DATA info;

	HANDLE hp;


	wchar_t fileFound_lp[256];
	wchar_t file_delete_lp[256];
	sprintf_s(fileFound, "%s\\*.*", from_jsonfolderPath.c_str());
	CharToWChar(fileFound, fileFound_lp);

	hp = FindFirstFileW(fileFound_lp, &info); //디렉토리에 파일이 있는지 첫번째 파일만.
	if (hp == INVALID_HANDLE_VALUE) {
		int ercode = GetLastError();

		err_message(ercode);
		return;
	}
	string str;

	//int res;
	string f_name = batch_dir + "\\" + fname;
	cout << fname << endl;
	std::ofstream file_id, file_method;
	file_id.open(f_name.c_str());


	do

	{

		_bstr_t b(info.cFileName);
		const char* c = b;
		//	cout << "cFilename : " << c << endl;

		sprintf_s(fileFound, "%s\\%s", file_folderPath.c_str(), c);
		CharToWChar(fileFound, file_delete_lp);



		//	cout << fileFound << endl;
		if ((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			//	cout << "skipped directory "<< b<<" "<<c << endl;
		}
		else {
			if (!strcmp(c, ".")) {// cout << "skipped . " << endl;
				continue;
			}
			if (!strcmp(c, "..")) {// cout << "skipped .. " << endl; 
				continue;
			}

			str = exe_path + " " + image_file_name + " " + file_folderPath + "\\" + c;
			file_id << str << endl;

			/*
			res = DeleteFileW(file_delete_lp);
			if (res == NULL) {
			int ercode = GetLastError();

			err_message(ercode);
			}
			*/
		}//else directory
	} while (FindNextFileW(hp, &info));  //다른 파일이 있을때 까지



	FindClose(hp);
	file_id.close();
}

