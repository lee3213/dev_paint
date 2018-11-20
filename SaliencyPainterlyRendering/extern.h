#pragma once
//#include "debug_image.h"
#include "stdafx.h"
#include <ios>
#include <iostream>
#include <streambuf>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <random>
#include "define.h"
#include "brush.h"
#include "render_.h"

using namespace std;
using namespace cv;

extern string g_debug_file;

extern int g_merge_method;

extern int g_QT_avgSThreshold;
//extern int g_grid_threshold;
extern int g_depth_limit;
// int g_saliency_depth;
// int g_union_depth;
extern int g_min_gridsize;

extern int g_brush_Ts;
//extern int g_debug_level;
extern int g_unique_id;
extern string g_debug_method;
extern string g_method_revision;
extern string g_saliency_method;//without_saliency, saliency, pregraph, blackandwhite,resudual,fine_sobelined
extern string g_paint_mode_str;//

extern string g_root_path;//usually "/rst";

extern string g_para_method_path; // g_root_path & g_method_path


extern string g_para_method;
extern string g_root_path_win; //"\\org"
extern string g_image_name;
extern string g_para;
extern string g_para_path;
extern string g_para_method_image_path;

extern string g_root_image_path;
extern string g_root_saliency_path;


extern int g_QT_method_N;//N


extern int  g_BrushNumber;							//basic:64    expansion:48
extern int  g_BrushMinSize;
extern int g_BrushAttachSize;

extern int g_brush_style;
extern int g_INDEX_BRUSH_SIZE_WIDTH;
extern int g_INDEX_BRUSH_SIZE_HEIGHT;
extern int g_brush_thumbnail_size;

#ifdef G_DEBUG_TOUCH
extern cv::Mat g_touch;
extern unsigned char * g_touch_data;
#endif

extern fstream g_file_cout;
extern streambuf* g_stream_buffer_file_cout;
extern streambuf* g_stream_buffer_file_clog;
extern fstream g_file_clog;
extern streambuf* g_stream_buffer_file_cstat;
extern fstream g_file_cstat;
extern streambuf* g_stream_buffer_file_cerr;
extern fstream g_file_cerr;
//extern ostream g_file_cout_backup;
//extern streambuf *g_stream_buffer_file_cout_backup;
extern int g_src_image_width;
extern int g_src_image_height;
extern int g_src_image_channels;
extern int g_src_image_step1;


extern int g_alpha_TH;
extern int g_ET;//positive ET=-1 negative

//extern Mat Sgrid_grid_map_1c[];
extern Mat gradient_Map_C_8UC1;
extern Mat gradient_Map_G_8UC1;
extern std::string tag[];// 
extern std::string tag_[]; //
extern std::string _tag[];//
extern string _t[];
extern string _t_[];
extern int g_image_divider;// BrushMaxSize = g_src_image_height / g_image_divider;
//extern float g_BrushMax_scale;
extern int g_qt_s_scale;
extern int g_brush_choice;
extern int g_trace_depth ;
extern string g_saliency_method_str[MAX_SALIENCY];// = { "Sobel", "Itti","Pregraph","Residual","Blackandwhite","Fine_grained","Perazzi" };
//extern int g_unique_id;
extern string g_sa_str[MAX_SALIENCY];
//extern int g_first_layer;
//
extern int g_paint_try_scale[MAX_DEPTH];
extern int g_brush_scale[MAX_DEPTH];

extern vector <Brush*> g_brush_vector_set;

extern Mat g_srcImg_RO;
extern int g_rendering_method;


extern render_ *_render[RENDER_MAX];
extern int g_no;