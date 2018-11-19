#pragma once
#include "stdafx.h"
#include "debug_image.h"
#include "brush.h"
#include <random>
#include "define.h"
#include "render_.h"
#include <ios>
#include <iostream>
#include <streambuf>
//CWinApp theApp;
//debug_image_single *g_debug_brush;
//debug_image_single *g_debug_brush_thumbnail;
//debug_image_single *g_debug_brush_resized;
//int g_brush_called_cnt = 0;
int g_image_divider;// BrushMaxSize = g_src_image_height / g_image_divider;
bool g_first_time = true;
//FILE *g_log;
//vector <Brush*> g_brush_set;//Brush set
int g_unique_id ;
int g_merge_method;
//cv::Mat g_grid_map[MAX_DEPTH]; //
//cv::Mat g_saliency_grid_map[MAX_DEPTH];
//int g_saliency_Sgrid_painting_try[MAX_DEPTH];
//int  m_brush_size[MAX_DEPTH]; // painting gris size for every depth //calculated in PainterlyRendering depends on depth and max/min brush size
//int  g_saliency_brush_size[MAX_DEPTH];
//int g_brush_grid_size[MAX_DEPTH];// brush grid size for every depth
//Size g_QT_grid_size[MAX_DEPTH]; //QuadTree grid size for every depth//calculated QT.cpp
//int g_QT_grid_count[MAX_DEPTH];
//int g_saliency_QT_grid_count[MAX_DEPTH];
//////////////////Threshhold 조절////////////////////
//#define avgSThreshold 23
int g_QT_avgSThreshold;
int g_depth_limit;
//int g_saliency_depth;
	// 1024 : 7
int g_min_gridsize;
/////////////////////////////////////////////////////

string g_debug_file;
//////////////////Threshhold 조절////////////////////

//int g_RetryTime;
int g_brush_Ts;
/////////////////////////////////////////////////////
//int g_debug_level;

string g_method_revision;

string g_debug_method;//"org"
string g_root_path;//usually "/rst";
string g_root_path_win;

				   // g_method_path_win;// "\\rst\\org";
string g_para;
string g_para_path;;
string g_root_saliency_path;

string g_method;
//string g_method_path;// "/rst//org";
string g_para_method;
string g_para_method_path; // g_root_path & g_method_path
string g_para_method_image_path;
string g_image_name;
//string g_win_path;
string g_root_image_path;
int g_brush_style;

//cv::Mat g_fetched_color;
//unsigned char * g_fetched_color_data;

int  g_BrushNumber ;						//basic:64    expansion:48
int g_brush_thumbnail_size;
int g_BrushMinSize;
int g_BrushAttachSize;
unsigned char *g_org_DensityMap_data;
int g_INDEX_BRUSH_SIZE_WIDTH ;
int g_INDEX_BRUSH_SIZE_HEIGHT;

//Mat Sgrid_grid_map_1c[MAX_DEPTH];
Mat gradient_Map_C_8UC1;
Mat gradient_Map_G_8UC1;
std::string tag[] = { "0_sobel","1_saliency","2_union","3_merge","4_attach" };
std::string tag_[] = { "0_sobel_", "1_saliency_", "2_union_", "3_merge_","4_attach_" };
std::string _tag[] = { "_0_sobel", "_1_saliency", "_2_union", "_3_merge_","_4_attach_" };
string _t[] = { "_0so","_1sa","_2un","_3me","_4at" };


#ifdef G_DEBUG_TOUCH
cv::Mat g_touch;
unsigned char * g_touch_data;
#endif

int g_src_image_width;
int g_src_image_height;
int g_src_image_channels;
int g_src_image_step1;

vector <Brush*> g_brush_set;

int g_QT_method_N;//N



streambuf* g_stream_buffer_file_cout;
streambuf* g_stream_buffer_file_clog;
streambuf* g_stream_buffer_file_cstat;
streambuf* g_stream_buffer_file_cerr;
fstream g_file_cout;
fstream g_file_clog;
fstream g_file_cstat;
fstream g_file_cerr;

int g_alpha_TH = 240;//used in alpha
int g_ET = 1;//positive ET=-1 negative


int g_qt_s_scale=2;//
int g_brush_choice = 1; //if (g_brush_choice == 0) {nth = rand() % 5;} else nth = 0;
int g_trace_depth=7;

string g_saliency_method;//Sobel, Itti, pregraph, Residual blackandwhite,fine_grained Perazzi
string g_paint_mode_str;//alhpa  stencil

string g_saliency_method_str[MAX_SALIENCY] = { "Sobel", "Itti","Pregraph","Residual","Blackandwhite","Fine_grained","Perazzi" };
string g_sa_str[MAX_SALIENCY] = { "so","It","Pr","Re","Bw","Fg","Pe" };

//int g_unique_id;
//int g_first_layer = 0;
//float  g_BrushMax_scale = 1.5;
int g_paint_try_scale[MAX_DEPTH] ;
int g_brush_scale[MAX_DEPTH];

render_ *_render[RENDER_MAX];
Mat g_srcImg_RO;
int g_rendering_method;