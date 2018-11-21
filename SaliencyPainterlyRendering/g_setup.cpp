#include "stdafx.h"

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "extern.h"
#include "set_result_path.h"
#include "json_read.h"


void
cout_func(string src_name, string deploy_name){
	cout << "deployname = " << deploy_name << endl;
	cout << "src = " << src_name << endl;
	cout << "image_name " << g_image_name << endl << endl;
	cout << "g_root_path = " << g_root_path << endl;
	cout << "g_root_path_win = " << g_root_path_win << endl;
	cout << "g_para " << g_para << endl;
	cout << "g_para_method = " << g_para_method << endl;
	cout << "g_para_path = " << g_para_path << endl;
	cout << "g_para_method_path= " << g_para_method_path << endl;
	cout << "g_para_method_image_path= " << g_para_method_image_path << endl;
	cout << endl;
	cout << "g_saliency_method " << g_saliency_method << endl;

	cout << "g_Depth_Limit = " << g_depth_limit << endl;
	cout << "g_min_gridsize = " << g_min_gridsize << endl;
	cout << "g_QT_avgSThreshold = " << g_QT_avgSThreshold << endl;
	
	cout << "g_BrushMinSize = " << g_BrushMinSize << endl;
	cout << "g_brush_thumbnail_size = " << g_brush_thumbnail_size << endl;

	cout << "g_QT_method_N = " << g_QT_method_N << endl;
	for (int i = 0; i < MAX_DEPTH; i++) {
		cout << setw(3) << i << " : " << setw(5)<<g_paint_try_scale[i]
			<<setw(10)<<g_brush_scale[i]<< endl;
	}
}


void set_result_path(std::string para_path, string method_path, int depth) {
	string dirs[] = { "ing","sobel","saliency" ,"br","grid","br2","grad_ori" };
	string sub_dirs;
	string dir;

	dir = g_root_path_win + string("\\") + para_path;
	cout << "dir " << g_root_path_win << " : " << dir << endl;
	check_and_create(dir, false);

	dir = g_root_path_win + string("\\") + para_path + "\\" + method_path;
	cout << "dir " << g_root_path_win << " : " << dir << endl;
	check_and_create(dir, false);

	dir = g_root_path_win + string("\\") + para_path + "\\" + method_path + "\\" + g_image_name;
	cout << "dir " << g_root_path_win << " : " << dir << endl;
	check_and_create(dir, true);

	for (int i = 0; i < sizeof(dirs) / sizeof(string); i++) {
		sub_dirs = dir;
		sub_dirs.append("\\");
		sub_dirs.append(dirs[i]);
		cout << i << " : " << sub_dirs << endl;
		check_and_create(sub_dirs, true);
	}

	for (int i = 0; i < MAX_DEPTH; i++) {
		sub_dirs = dir;

		sub_dirs.append("\\p");
		sub_dirs.append(to_string(i));
		cout << i << " : " << sub_dirs << endl;
		check_and_create(sub_dirs, true);
	}
	for (int i = 0; i < MAX_DEPTH; i++) {
		sub_dirs = dir;

		sub_dirs.append("\\br\\p");
		sub_dirs.append(to_string(i));
		cout << i << " : " << sub_dirs << endl;
		check_and_create(sub_dirs, true);
	}
}

void filename_without_ext(string src_name, string &g_image_name) {
	char buf2[100];
	memset(buf2, 0, 100);
	char * s_p, *p;
	//s_p= p = src_name.data;
	strcpy_s(buf2, src_name.c_str());
	char * s, *e;
	s_p = s = e = p = &buf2[0];
	while (*p) {
		//cout << *p << ": "<<s<<" : "<<e<<" : "<<s_p<<endl;
		if (*p == '\\') s = e = p + 1;
		else if (*p == '.' && s_p != p) { e = p; }
		//	else e = p;
		p++;
	}
	char buf[100];
	memset(buf, 0, 100);
	memcpy_s(buf, 100, s, e - s);
	g_image_name = string(buf);
	cout << g_image_name << endl;
}
int  set_global(string src_name,string deploy_name) {
	
	g_INDEX_BRUSH_SIZE_WIDTH = 20;//changed by cwlee from 10 to same as thumbnail
	g_INDEX_BRUSH_SIZE_HEIGHT = 20; //changed by cwlee from 10 to same as thumbnail
	g_BrushNumber = 48;						//basic:64    expansion:48
	g_brush_thumbnail_size = g_INDEX_BRUSH_SIZE_HEIGHT;

	g_brush_choice = 1;// one of 5 random 

	enhance_brush[0].create(2, 2, CV_8UC1);
	enhance_brush[0].setTo(255);
	enhance_brush_ptr[0] = enhance_brush[0].data;
	enhance_brush[1].create(3, 3, CV_8UC1);
	enhance_brush[1].setTo(255);
	enhance_brush_ptr[1] = enhance_brush[1].data;
	enhance_brush[2].create(4, 4, CV_8UC1);
	enhance_brush[2].setTo(255);
	enhance_brush_ptr[2] = enhance_brush[2].data;
	
	unsigned char * enhance_brush_ptr[3];

	int ret=json_deployment(deploy_name);
	if (ret < 0) return -1;
	filename_without_ext( src_name,g_image_name);
	
	g_para = string("s") + to_string(g_QT_avgSThreshold) + "d" + to_string(g_depth_limit)
		+ "g" + to_string(g_min_gridsize) + "_b" + to_string(g_BrushMinSize) +
		"_N" + to_string(g_QT_method_N)+"_bn"+to_string(g_BrushNumber);
	
	g_unique_id = 0;//QT->no
	g_debug_method = "";
	g_debug_method.append("org");
	 cout << "g_para " << g_para << endl;

	g_para_method = g_para + "_m" + g_saliency_method //+ g_Render_method +

		;

	g_para_path = g_root_path + string("/") + g_para;
	g_para_method_path = g_root_path + string("/") + g_para + "/" + g_para_method;
	g_para_method_image_path = g_para_method_path + "/" + g_image_name;
	g_root_image_path = g_root_path + "/" + g_image_name;
	g_root_saliency_path = g_root_path + "/saliency";
	cout << "g_para_method [" << g_para_method << "]"<<endl;										 //	g_win_path = g_root_path_win + string("\\") + g_method + string("\\") + g_image_name;
	cout << "g_para_path [" << g_para_path <<"]"<< endl;
	cout << "g_image_path [" << g_root_image_path << "]" << endl;

	check_and_create(g_root_path_win , false);
	check_and_create(g_root_path_win +string("\\cout"), false);
	check_and_create(g_root_path_win +string("\\csv_log"), false);
	check_and_create(g_root_path_win + string("\\cerr"), false);
	check_and_create(g_root_path_win + string("\\cstat"), false);
	check_and_create(g_root_path_win + string("\\")+g_image_name, false);
	check_and_create(g_root_path_win + string("\\cout\\") + g_para, false);
	check_and_create(g_root_path_win + string("\\csv_log\\") + g_para, false);
	check_and_create(g_root_path_win + string("\\cerr\\") + g_para, false);
	check_and_create(g_root_saliency_path,false);
	

	cout_func(src_name,deploy_name);
	string f_path_cerr = g_root_path_win + string("\\cerr\\") + g_para + "\\cerr_" + g_para_method + "_" + g_image_name + string(".txt");
	cout << f_path_cerr << endl;

	g_file_cerr.open(f_path_cerr.c_str(), ios::out);
	// Get the streambuffer of the file
	g_stream_buffer_file_cerr = g_file_cerr.rdbuf();
	cerr.rdbuf(g_stream_buffer_file_cerr);

	string f_path_cout = g_root_path_win + string("\\cout\\")+g_para+ "\\cout_"+g_para_method +"_"+ g_image_name+string(".txt");
	cout << f_path_cout << endl;
	
	//g_stream_buffer_file_cout_backup=cout.rdbuf();
	//g_file_cout_backup.tie = cout.tie;
	//g_file_cout_backup.rdbuf(g_stream_buffer_file_cout_backup);
	//g_file_cout_backup << f_path_cout << endl;
	g_file_cout.open(f_path_cout.c_str(), ios::out);
	// Get the streambuffer of the file
	 g_stream_buffer_file_cout = g_file_cout.rdbuf();
	cout.rdbuf(g_stream_buffer_file_cout);

	//file_redirection_cout(g_para_method_path+"/cout.txt");
	//fstream file;
	string f_path_log = g_root_path_win + string("\\csv_log\\")+g_para+ "\\clog_"+  g_para_method + "_"+g_image_name+ string(".txt");
	cout << f_path_log << endl;
	
	g_file_clog.open(f_path_log.c_str(), ios::out);
	// Get the streambuffer of the file

	//g_stream_buffer_file_clog = g_file_clog.rdbuf();

	// Redirect cout to file
	//csv_log.rdbuf(g_stream_buffer_file_clog);
	

	string f_path_stat = g_root_path_win + string("\\cstat\\cstat_")+g_image_name+".csv";
	cout << f_path_stat << endl;
		
	g_file_cstat.open(f_path_stat.c_str(), ios::app);
		// Get the streambuffer of the file
		g_stream_buffer_file_cstat = g_file_cstat.rdbuf();

	
		cout_func(src_name, deploy_name);
	
	
		set_result_path(g_para,g_para_method,g_depth_limit);

	if (g_paint_mode_str == BRUSH_ALPHA_STR)
		g_brush_style = BRUSH_ALPHA;
	else if (g_paint_mode_str == BRUSH_STENCIL_STR)
		g_brush_style = BRUSH_STENCIL;
	else
		g_brush_style = BRUSH_STENCIL;

	return 0;
}
