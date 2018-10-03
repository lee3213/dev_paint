#pragma once
#include "debug_image.h"
#include "brush.h"
#include <random>
#include "define.h"
#include "render_.h"
//CWinApp theApp;
//debug_image_single *g_debug_brush;
//debug_image_single *g_debug_brush_thumbnail;
//debug_image_single *g_debug_brush_resized;
//int g_brush_called_cnt = 0;
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


//////////////////Threshhold 조절////////////////////

//int g_RetryTime;
int g_brush_Ts;
/////////////////////////////////////////////////////
int g_debug_level;

string g_method_revision;

string g_debug_method;//"org"
string g_root_path;//usually "/rst";
string g_root_path_win;

				   // g_method_path_win;// "\\rst\\org";
string g_para;
string g_para_path;;
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
unsigned char *g_org_DensityMap_data;
int g_INDEX_BRUSH_SIZE_WIDTH ;
int g_INDEX_BRUSH_SIZE_HEIGHT;

//int m_brush_size_x;
//int m_brush_size_y;
//random_device rand_x[5], rnd_y[5];
//mt19937 g_engine_x[5];// (GetTickCount());
//uniform_int_distribution<int> *g_distribution[5];
//int g_debug_called_cnt;
//int g_debug_min_level;
//int g_grid_sum;//sum of g_Sgrid_painting_try[]
#ifdef G_DEBUG_TOUCH
cv::Mat g_touch;
unsigned char * g_touch_data;
#endif

int g_src_image_width;
int g_src_image_height;
int g_src_image_channels;
int g_src_image_step1;

//cv::Mat grid_map[5]; //
//cv::Mat fetched_map[5]; // randominized fetched point
//cv::Mat try_map[5]; // randominized try grid map
string g_saliency_method;//without_saliency, saliency, pregraph, blackandwhite,resudual,fine_sobelined
string g_paint_method;
//string g_Render_method;//Union, Only

int g_paint_area_scale;
int g_QT_method_N;//N





//mt19937 g_engine_x[MAX_DEPTH];// (GetTickCount());
//mt19937 g_engine_y[MAX_DEPTH];// (GetTickCount());
//uniform_int_distribution<int> *g_distribution[MAX_DEPTH];

streambuf* g_stream_buffer_file_cout;
streambuf* g_stream_buffer_file_clog;
streambuf* g_stream_buffer_file_cstat;
streambuf* g_stream_buffer_file_cerr;
fstream g_file_cout;
fstream g_file_clog;
fstream g_file_cstat;
fstream g_file_cerr;

int g_TH = 240;
int g_ET = 1;//positive ET=-1 negative
int g_merge_skip_count;

string saliency_method[] = { "Itti","Pregraph","Residual","Blackandwhite","Fine_grained" };
//string _re[MAX_DEPTH] = { "Sobel","Saliency","Twopass" };
//string _str_mm[MAX_DEPTH] = { "Attach","Merge" };

int saliency_method_cnt = 5;
//int _re_cnt = 3;
//int _str_mm_cnt = 2;
