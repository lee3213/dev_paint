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
int thresh[] = { 10,15,25 };
int depth[] = { 5,7 };
int _grid[] = { 10 };

int QT_N[] = { 1 };
//int _mm[] = { 1,2 };// "attach", tag_union
;
int _ps[] = { 6 };//"g_paint_grid_scale"
int _br[] = { 10 }; //g_BrushMinSize
int _gs[] = { 1 };//grid skip 
int br, th, de, gr, sm, qt, re, mm, gs, ps;
void get_f_name(string & file_name) {
	 file_name = "deploy_s" + to_string(thresh[th]) + "_d" + to_string(depth[de]) +
		"_g" + to_string(_grid[gr]) + "_b" + to_string(_br[br]) + "_N" + to_string(QT_N[qt])
		+ "_ps" + to_string(_ps[ps])
		+ "_" + _re[re] + "_" + saliency_method[sm]
		+ "_m" + _str_mm[mm] + "_gs" + to_string(_gs[gs]) + "_copy.json";
	
}


//int br, th, de, gr, sm, qt, re, mm, gs, ps;
int  json_write_method(string path) {
	string f_name;
	string file_name;
	Json::StreamWriterBuilder wbuilder;
	Json::Value root;
	int n_br = sizeof(_br) / sizeof(int);
	int n_th = sizeof(thresh) / sizeof(int);
	int n_de = sizeof(depth) / sizeof(int);
	int n_gr = sizeof(_grid) / sizeof(int);
	int n_sm = saliency_method_cnt;;
	int n_qt = sizeof(QT_N) / sizeof(int);
	int n_re = _re_cnt;
	int n_mm = _str_mm_cnt;
	int n_ps = sizeof(_ps) / sizeof(int);
	int n_gs= sizeof(_gs) / sizeof(int);
	int howmany = n_br  *n_th * n_de * n_gr*n_sm*n_qt*n_re*n_mm*n_ps*n_gs;
	int f_count = 0;
	for ( br = 0; br < n_br; br++) {
	for ( th = 0; th < n_th; th++) {
		for ( de = 0; de < n_de; de++) {
			for ( gr = 0; gr< n_gr; gr++) {
				for( sm=0;sm<n_sm ; sm++){
					for (qt = 0; qt < n_qt; qt++) {
						for (re = 0; re < n_re; re++) 
							for ( mm = 0; mm< n_mm; mm++)
								for ( ps = 0; ps<n_ps; ps++)
								for ( gs = 0; gs<n_gs; gs++){
									
							//root.append("revision","1");
							root["root_path"] = "/rst";
							root["root_path_win"] = "\\rst";
							root["g_paint_method"] = "copy";//copy alpha
							root["g_BrushMinSize"] = _br[br]; //5,10
							root["g_Ts"] = 230;
							root["end"] = "end";
							root["g_paint_grid_scale"] = _ps[ps];//
							root["g_merge_method"] = _str_mm[mm];
							root["g_merge_skip_count"] = _gs[gs];
							root["QT_avgSThreshold"] = thresh[th];//15, 25
							root["g_depth_Threshhold"] = depth[de]; //5,7
							root["g_gridsize"] = _grid[gr];	//5,10

							root["g_saliency_method"] = saliency_method[sm];//wo saliency pregraph 

							root["g_QT_method_N"] = QT_N[qt]; //1,2,3,4...6
							root["g_Render_method"] = _re[re];// Union or Only

						//	cout << "path" << path << " = " << root << endl;
							
							if (saliency_method[sm] == "Sobel" &&  _re[re] == "Saliency") { continue; }
							if (saliency_method[sm] == "Sobel" &&  _re[re] == "Twopass") { continue; }
							get_f_name(file_name);
							// Make a new JSON document for the configuration. Preserve original comments.
								//std::string outputConfig = writer.write(root);
							std::string outputConfig = Json::writeString(wbuilder, root);
							//	cout << "outputConfig " << outputConfig << endl;

							f_name = path + "\\" + file_name;
							std::ofstream file_id;
							file_id.open(f_name.c_str());
							file_id << outputConfig;
							f_count++;
							if (!(f_count % 100)) {
								std::cout << "method " << f_count<<" / "<< howmany <<" : " << f_count/howmany*100. <<endl;
							}
							file_id.close();
						}
					}
			}
		}
	}
}
	}
// You can also use streams.  This will put the contents of any JSON
// stream at a particular sub-value, if you'd like.
//std::cin >> root["subtree"];
	cout << "method " << f_count<<endl;
	return 0;
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

