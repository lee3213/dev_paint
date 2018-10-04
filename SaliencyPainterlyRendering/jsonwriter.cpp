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
int thresh[] = { 15 };
int depth[] = { 7 };
int _grid[] = { 5 };

string saliency_method[] = { "Sobel", "Itti","Pregraph","Residual","Blackandwhite","Fine_grained","Perazzi"};
//string _re[MAX_DEPTH] = { "Sobel","Saliency","Twopass" };
//string _str_mm[MAX_DEPTH] = { "Attach","Merge" };

int saliency_method_cnt = sizeof(saliency_method) / sizeof(string);
int QT_N[] = { 1};
//int _mm[] = { 1,2 };// "attach", tag_union
;
int _ps[] = { 6 };//"g_paint_area_scale"
int _br[] = { 5 }; //g_BrushMinSize
int _gs[] = { 1 };//grid skip 
int _ts[] = { 230 };
int br, th, de, gr, sm, qt, re, mm, gs, ps,ts;

void get_json_name(string & file_name) {
	 file_name = "S" + saliency_method[sm]+"_s" + to_string(thresh[th]) + "_d" + to_string(depth[de]) +
		"_g" + to_string(_grid[gr]) + "_b" + to_string(_br[br]) + "_N" + to_string(QT_N[qt])
		+ "_ps" + to_string(_ps[ps])
		//+ "_r" + _re[re] 
		 + "_gs" + to_string(_gs[gs])
		 + "_ts" + to_string(_ts[ts])
		
		//+ "_m" + _str_mm[mm]  
		 + "_copy.json";
	
}
void get_render_para_ps(string &render_name) {
	render_name = "render_s" + to_string(thresh[th]) + "_d" + to_string(depth[de]) +
		"_g" + to_string(_grid[gr]) + "_b" + to_string(_br[br]) + "_N" + to_string(QT_N[qt])
		+ "_ps" + to_string(_ps[ps]);
//		+ "_ts" + to_string(_ts[ts]);
}
void get_render_para(string &render_name) {
	render_name = "render_s" + to_string(thresh[th]) + "_d" + to_string(depth[de]) +
		"_g" + to_string(_grid[gr]) + "_b" + to_string(_br[br]) + "_N" + to_string(QT_N[qt])
		+ "_ps" + to_string(_ps[ps])
		+ "_ts" + to_string(_ts[ts]);
}
void get_render_name(string & bat_dir,string & file_name) {
	string render_name;
	get_render_para(render_name);
	file_name = bat_dir+string("\\")+render_name+
		//+ "_r" + _re[re] 
		+ "_smAll.bat";

}
void get_render1_name(string & bat_dir,int _sm_, string & file_name) {
	string render_name;
	get_render_para(render_name);
	file_name = bat_dir +string("\\") + render_name + 
	+ "_sm" + saliency_method[_sm_] 
		+ ".bat";

}
//int br, th, de, gr, sm, qt, re, mm, gs, ps;
int  json_write_method(string from_jsonfolderPath,//render/deployument
	string batch_render_dir, //batch/render 
	string exe_path, //exe
	string list_path 

	) {
	string f_name;
	string file_name;
	string render_batch_fname;

	Json::StreamWriterBuilder wbuilder;
	Json::Value root;
	int n_br = sizeof(_br) / sizeof(int);
	int n_th = sizeof(thresh) / sizeof(int);
	int n_de = sizeof(depth) / sizeof(int);
	int n_gr = sizeof(_grid) / sizeof(int);
	int n_sm = saliency_method_cnt;;
	int n_qt = sizeof(QT_N) / sizeof(int);
	//int n_re = _re_cnt;
//	int n_mm = 1;//_str_mm_cnt;
	int n_ps = sizeof(_ps) / sizeof(int);
	int n_gs= sizeof(_gs) / sizeof(int);
	int n_ts = sizeof(_ts) / sizeof(int);
	int howmany = n_br  *n_th * n_de * n_gr*n_sm*n_qt*n_ps*n_gs*n_ts;
	
	int f_count = 0;
	string deploy_name[300];
	int n_deploy=0;
	//for (re = 0; re < n_re; re++) {
	string render_para;
	std::ofstream file_para;
	string para_f_name;
	para_f_name = list_path+string("para.lst");
	file_para.open(para_f_name.c_str());
	std::ofstream file_para_meta;
	string f_name_para_meta;
	f_name_para_meta = list_path + string("para_meta.lst");
	file_para_meta.open(f_name_para_meta.c_str());
			for (th = 0; th < n_th; th++) //s
				for (de = 0; de < n_de; de++) //d
					for (gr = 0; gr < n_gr; gr++) //g
						for (br = 0; br < n_br; br++) //b
							for (qt = 0; qt < n_qt; qt++) //N QTn{
								for (ps = 0; ps < n_ps; ps++){//ps //scale
									std::ofstream file_ts;
									string f_name_ts;
									string t;
									get_render_para_ps(t);
									file_para_meta << t << endl;
									f_name_ts = batch_render_dir +"\\para_"+t+ string(".bat");
									file_ts.open(f_name_ts.c_str());

									for (ts = 0; ts < n_ts; ts++) { //
										get_render_para(render_para);
										file_para << render_para << endl;
										for (sm = 0; sm < n_sm; sm++) //saliency method
											for (gs = 0; gs < n_gs; gs++) {//grid skip

												//root.append("revision","1");
												root["root_path"] = "/rst";
												root["root_path_win"] = "\\rst";
												root["g_paint_method"] = "copy";//copy alpha
												root["g_BrushMinSize"] = _br[br]; //5,10
												root["g_Ts"] = _ts[ts];
												root["end"] = "end";
												root["g_paint_area_scale"] = _ps[ps];//
												//root["g_merge_method"] = _str_mm[mm];
												root["g_merge_skip_count"] = _gs[gs];
												root["QT_avgSThreshold"] = thresh[th];//15, 25
												root["g_depth_Threshhold"] = depth[de]; //5,7
												root["g_gridsize"] = _grid[gr];	//5,10

												root["g_saliency_method"] = saliency_method[sm];//wo saliency pregraph 

												root["g_QT_method_N"] = QT_N[qt]; //1,2,3,4...6
										//		root["g_Render_method"] = _re[re];// Union or Only

											//	cout << "path" << path << " = " << root << endl;


												get_json_name(file_name);
												// Make a new JSON document for the configuration. Preserve original comments.
													//std::string outputConfig = writer.write(root);
												std::string outputConfig = Json::writeString(wbuilder, root);
												//	cout << "outputConfig " << outputConfig << endl;

												f_name = from_jsonfolderPath + "\\" + file_name;
												std::ofstream file_id;
												file_id.open(f_name.c_str());
												file_id << outputConfig;
												f_count++;
												if (!(f_count % 100)) {
													std::cout << "method " << f_count << " / " << howmany << " : " << f_count / howmany*100. << endl;
												}
												file_id.close();
												deploy_name[n_deploy] = f_name;
												n_deploy++;
											}
										//cout << "n_deploy= " << n_deploy << endl;
									//	std::cout << batch_fname << endl;
										std::ofstream file_id;
										string bat_f_name;
										std::ofstream file_id_1;
										//string bat_f_name;
										string bat_f1_name;
										get_render_name(batch_render_dir, bat_f_name);
										//	cout << "bat " << bat_f_name << endl;

										file_id.open(bat_f_name.c_str());
										for (int i = 0; i < n_deploy; i++) {
											get_render_name(batch_render_dir, bat_f_name);
											//cout << "bat " << bat_f_name << endl;

											string str = exe_path + " %1 " + deploy_name[i];
											file_id << str << endl;
											if (i == 0)
												file_ts << str << endl;
											get_render1_name(batch_render_dir, i, bat_f1_name);
											file_id_1.open(bat_f1_name.c_str());
												file_id_1 << str << endl;
											//cout << "bat1 " << bat_f1_name << endl;
											file_id_1.close();

										}
										file_id.close();
										n_deploy = 0;
									}
									file_ts.close();
								}
			file_para.close();
			file_para_meta.close();
		std::cout << "method " << f_count<<endl;
	return 0;
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

