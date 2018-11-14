#include "stdafx.h"
#include "json_read.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "extern.h"
#include "set_result_path.h"

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
		cout << setw(3) << i << " : " << g_paint_area_scale[i] << endl;
	}
}
int  set_global(string src_name,string deploy_name) {
	
	g_INDEX_BRUSH_SIZE_WIDTH = 20;//changed by cwlee from 10 to same as thumbnail
	g_INDEX_BRUSH_SIZE_HEIGHT = 20; //changed by cwlee from 10 to same as thumbnail
	g_BrushNumber = 64;						//basic:64    expansion:48
	g_brush_thumbnail_size = g_INDEX_BRUSH_SIZE_HEIGHT;

	

	int ret=json_deployment(deploy_name);
	if (ret < 0) return -1;

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

	g_para = string("s") + to_string(g_QT_avgSThreshold) + "d" + to_string(g_depth_limit)
		+ "g" + to_string(g_min_gridsize) + "_b" + to_string(g_BrushMinSize) +
		"_N" + to_string(g_QT_method_N);// + "_ps" + to_string(g_paint_area_scale[0]) ;
		// + "_gth" + to_string(g_grid_threshold);

	 cout << "g_para " << g_para << endl;

	g_para_method = g_para + "_m" + g_saliency_method //+ g_Render_method +
	//	+"_p" + g_paint_method//+"_q"+to_string(g_merge_method)
		;

	g_para_path = g_root_path + string("/") + g_para;
	g_para_method_path = g_root_path + string("/") + g_para + "/" + g_para_method;
	g_para_method_image_path = g_para_method_path + "/" + g_image_name;
	g_root_image_path = g_root_path + "/" + g_image_name;

	cout << "g_para_method [" << g_para_method << "]"<<endl;										 //	g_win_path = g_root_path_win + string("\\") + g_method + string("\\") + g_image_name;
	cout << "g_para_path [" << g_para_path <<"]"<< endl;
	cout << "g_image_path [" << g_root_image_path << "]" << endl;

	check_and_create(g_root_path_win , false);
	check_and_create(g_root_path_win +string("\\cout"), false);
	check_and_create(g_root_path_win +string("\\clog"), false);
	check_and_create(g_root_path_win + string("\\cerr"), false);
	check_and_create(g_root_path_win + string("\\cstat"), false);
	check_and_create(g_root_path_win + string("\\")+g_image_name, false);
	check_and_create(g_root_path_win + string("\\cout\\") + g_para, false);
	check_and_create(g_root_path_win + string("\\clog\\") + g_para, false);
	check_and_create(g_root_path_win + string("\\cerr\\") + g_para, false);
	//g_method_path_win=g_root_path_win + string("\\") + g_method;// "\\rst\\org";
	//waitKey();
	//std::exit(1);

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
	string f_path_log = g_root_path_win + string("\\clog\\")+g_para+ "\\clog_"+  g_para_method + "_"+g_image_name+ string(".txt");
	cout << f_path_log << endl;
	
	g_file_clog.open(f_path_log.c_str(), ios::out);
	// Get the streambuffer of the file

	g_stream_buffer_file_clog = g_file_clog.rdbuf();

	// Redirect cout to file
	clog.rdbuf(g_stream_buffer_file_clog);
	

	string f_path_stat = g_root_path_win + string("\\cstat\\cstat_")+g_image_name+".csv";
	cout << f_path_stat << endl;
		
	g_file_cstat.open(f_path_stat.c_str(), ios::app);
		// Get the streambuffer of the file
		g_stream_buffer_file_cstat = g_file_cstat.rdbuf();

	
		cout_func(src_name, deploy_name);

	g_unique_id = 0;//QT->no
	g_debug_method = "";
	g_debug_method.append("org");
	//g_merge_skip_count = 1;
	//g_merge_method = DEF_MERGE;
		set_result_path(g_para,g_para_method,g_depth_limit);

	if (g_paint_method == "alpha")
		g_brush_style = BRUSH_ALPHA;
	else if (g_paint_method == "copy")
		g_brush_style = BRUSH_COPY;
	else
		g_brush_style = BRUSH_COPY;

	return 0;
}
