#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "define.h"
#include "extern.h"

#include "brush.h"
#include "QuadTree.h"
#include "debug_image.h"

#include "render_.h"
using namespace std;
using namespace cv;
void image_save(string image_name, Mat result_image, Mat Quad_TreeMap) {
	debug_image_abs(g_root_image_path, g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_root_image_path, g_para_method + string("_") + image_name+"_Q" , Quad_TreeMap);

	debug_image_abs(g_para_path + string("/"), g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_path + string("/"), g_para_method + string("_") + image_name + string("_Q"), Quad_TreeMap);

	debug_image_abs(g_para_method_path + string("/"), g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_method_path + string("/"), g_para_method + string("_") + image_name + string("_Q"), Quad_TreeMap);

		debug_image_abs(g_para_method_image_path, g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_method_image_path, g_para_method + string("_") + image_name + string("_Q"), Quad_TreeMap);



	debug_image(g_para_method + string("_") + g_image_name, result_image);
	debug_image(g_para_method + string("_") + g_image_name, Quad_TreeMap);
}
/*
void render_::brush_delete(render_Brush* r_brush_array[]) {
	int nth = 0;
	for (std::vector <render_Brush*>::iterator i = brush_set.begin(), endI = brush_set.end(); i != endI; ++i, nth++)
	{

		(*i)->brush.release();
		(*i)->brush_thumbnail.release();
		(*i)->bump.release();
		(*i)->index_brush.release();
		(*i)->brush_8UC1.release();
	}

	brush_set.clear();

}
*/
int render_ ::region_dump(Region_set _aRegion_set[], string tag__, int  depth) {
	int sum = 0;
	
	for (int i = 0; i < depth; i++) {
		sum += (int)_aRegion_set[i].Region_list.size();
			r_cout << tag__ << " " << setw(5) << i << " : " << setw(5) << _aRegion_set[i].Region_list.size() << endl;
		
	}
	r_cout << _tag[render_method] <<tag__ << " " << setw(5) << depth << " : " << setw(5)<<sum << endl;
/*	int __saved_depth = -1;
	int  _grid[MAX_DEPTH];

	r_cout << tag << endl;
	for (int i = 0; i <MAX_DEPTH; i++) {
		_grid[i] = 0;
	}

	for (int i = 0; i < depth; i++) {
		for (list<Img_node*>::iterator partition_it = _aRegion_set[i]->begin(); partition_it != _aRegion_set[i]->end(); partition_it++)
		{

			_grid[(*partition_it)->depth]++;
			__saved_depth = (*partition_it)->depth;
		}//end of QuadTree dump
	}
		for (int i = 0; i < depth; i++) {
			r_cout << setw(4) << i << ", " << _grid[i] << endl;
	}
	*/
	return depth;
}

int render_::draw_grid_2(Mat _Quad_TreeMap,
	Region_set aRegion_set[], string ftag, int  depth, 
	//int draw_depth, 
	int c,string _tag) {
	int __saved_depth = -1;
	Mat overlay_grid_map[MAX_DEPTH];
	
	static int called_cnt = 0;
	if (aRegion_set->Region_list.size() == 0) {
		r_cout << "aStroke_Set size() = 0 " + _tag << +": " << ftag << " : "// << to_string(draw_depth) 
			<< endl;
		return depth;
	}

	for (int i = 0; i <MAX_DEPTH; i++) {
		overlay_grid_map[i] = _Quad_TreeMap.clone();
	}
	for (int i = 0; i < depth; i++) {

		for (list<partition_Node *>::iterator partition_it = render_region_set[i].Region_list.begin(); partition_it != render_region_set[i].Region_list.end(); partition_it++)
		{
		//	if (draw_depth != -1 && __saved_depth != draw_depth) {
		//		continue;
		//	}
			__saved_depth = (*partition_it)->depth;

			//if (draw_depth == -1 || __saved_depth == draw_depth)
			rectangle_canvas(overlay_grid_map[__saved_depth], Rect((*partition_it)->srtPoint,
				(*partition_it)->endPoint), Scalar(c, c, c));

		}//end of QuadTree dump
	}// for depth
	for (int i = 0; i <= __saved_depth; i++) {
	//	if (draw_depth == -1 || i == draw_depth) {
			debug_image("ing/"+to_string(called_cnt)+"_o_" + ftag, i, overlay_grid_map[i]);
			called_cnt++;
	//	}
	}
	 depth = __saved_depth + 1;
	r_cout << tag[depth] << " " << __FUNCTION__ << " finalized Depth : " << depth << " : " << grid_map_sum << endl;

	called_cnt++;
	return depth;
}
int render_::draw_grid_depth(Mat  _grid_map_1c[], Mat _grid_map_1c_accu,
	Region_set aRegion_set[], string tag, int & grid_map_sum,
	 int _QT_grid_count[]//, bool do_grid_cnt//, int draw_depth, int c
	) {
	int __saved_depth = -1;
	
	static int called_cnt = 0;
	list<partition_Node*>::iterator outbox = aRegion_set[0].Region_list.begin();
	//Point p;
//	p.x=(*outbox)->info.endPoint.x - 1;
	//p.y=(*outbox)->info.endPoint.y - 1;
	for (int i = 0; i < MAX_DEPTH; i++) {
		overlay_grid_map[i] = gradient_inverse.clone();
		cv::rectangle(_grid_map_1c[i], cv::Rect((*outbox)->srtPoint,
			(*outbox)->endPoint)
			, RGB(0, 0,0));//outer box
		overlay_dropped_map[i] = gradient_inverse.clone();
		//	debug_image("ing/_ok_grid_" + tag +
			//	"_" + to_string(called_cnt) + "_", i, overlay_grid_map[i]);
		_QT_grid_count[i] = 0;
	}

	for (int i = 0; i < MAX_DEPTH; i++) {
		int ssize=(int) aRegion_set[i].Region_list.size();
		//r_cout << "z : stroke " << i<<" : "<<__saved_depth <<" , "<<aRegion_set[i].size() << endl;
		if (ssize == 0) 
			break;
		
		for (list<partition_Node*>::iterator partition_it = aRegion_set[i].Region_list.begin(); 
			partition_it != aRegion_set[i].Region_list.end(); partition_it++)
		{
			__saved_depth = (*partition_it)->depth;

			cv::rectangle(_grid_map_1c_accu, Rect((*partition_it)->srtPoint,
				(*partition_it)->endPoint), RGB(255, 255, 255));
			cv::rectangle(_grid_map_1c[__saved_depth], Rect((*partition_it)->srtPoint,
				(*partition_it)->endPoint), RGB(0,0, 0));
			cv::rectangle(overlay_grid_map[__saved_depth], Rect((*partition_it)->srtPoint,
				(*partition_it)->endPoint), RGB(0, 0, 0));

				_QT_grid_count[__saved_depth]++;
		}//end of for list
	}//end of for i


	for (int i = 0; i < MAX_DEPTH; i++) {
		int ssize = (int)render_dropped_set[i].Region_list.size();
		//r_cout << "z : stroke " << i<<" : "<<__saved_depth <<" , "<<aRegion_set[i].size() << endl;
		if (ssize == 0)
			continue;

		for (list<partition_Node*>::iterator partition_it = render_dropped_set[i].Region_list.begin();
			partition_it != render_dropped_set[i].Region_list.end(); partition_it++)
		{
			__saved_depth = (*partition_it)->depth;
			cv::rectangle(overlay_dropped_map[__saved_depth], Rect((*partition_it)->srtPoint,
				(*partition_it)->endPoint), RGB(0, 0, 0));
		}//end of for list
		debug_image("pmap/r" + to_string(render_method) + "/_od" + m_t, i, overlay_dropped_map[i]);
	}//end of for i

	for (int i = 0; i <= __saved_depth; i++) {
		
			debug_image("ing/_og" +m_t, i, overlay_grid_map[i]);
			debug_image("_g" + m_t, i, _grid_map_1c[i]);
			debug_image("ing/_g" + m_t, i, _grid_map_1c[i]);
			
			r_cout << tag<<" "<<setw(6) << i << " : " << setw(10)<<_QT_grid_count[i] << endl;
				
			grid_map_sum += _QT_grid_count[i];
	
	}

	debug_image("QT" + m_t + "_" + to_string(__saved_depth) + "_f", _grid_map_1c_accu);

	//r_cout << "z depth " + tag << setw(5) << __saved_depth << " : QT_" << _QT_grid_count[__saved_depth] << endl;
	
	int depth = __saved_depth + 1;
	r_cout << tag <<" "<<__FUNCTION__<<"finalized Depth : " << depth << " : " << grid_map_sum<< endl;
	
	called_cnt++;
	return depth;
}
void render_::func_(){
//void func_(int _depth, int _changed_count[], int _QT_grid_count[], int _Sgrid_painting_try[], int _m_render_brush_size[], long int _painting_area[]) {
	int Qt_sum = 0;
	int try_sum = 0;
	int c_sum = 0;
	int t_sum = 0;
	//_painting_area[astroke_depth] += Region_w_size*Region_h_size;
	
	r_cout << endl <<
		setw(3) << "[]" <<
		setw(9)<<"tag "<<
		setw(6) << "QT_cnt" <<
		setw(6)<<"times"<<
		setw(5) << "try" <<
		setw(8)<<"extend"<<
		setw(7) << "try/QT" <<
		setw(8) << "changed" <<
		setw(9) << "ch ratio " <<
		setw(4) << "b_size" <<
	//	setw(8) << "b_scale" <<
		//setw(10) << "p_Area" <<
		//setw(14) << "p_area/try" <<
		//setw(16) << "p_area/p_Area" <<
		setw(4)<<"try_scale"<<
		endl;
	for (int i = 0; i < render_depth; i++) {
		
		if (grid_try_sum[i] != 0 && QT_grid_count[i] !=0){
		r_cout << setw(2) << i << "," <<setw(8)<<m_tag<<
			setw(6) << QT_grid_count[i] << "," <<
			setw(6)<<x_strk_times[i]<<
			setw(5) << grid_try_sum[i] << "," <<
			setw(7)<< extended_try[i]<<
			fixed<<setw(7) << setprecision(1)<<(float)((float)grid_try_sum[i] / (float)QT_grid_count[i]) <<
			setw(8) << r_s_changed_count[i] << ",  " <<
			std::fixed <<
			std::setw(6) << fixed<<std::setprecision(1) <<
			(float)((float)r_s_changed_count[i] / (float)(grid_try_sum[i])* 100.) <<
			setw(6) << render_brush_size[i] <<
			//setw(6) << render_brush_size[i] * render_brush_size[i] <<
			//setw(10) << render_brush_size[i] * render_brush_size[i] * grid_try_sum[i]<<
			//<< setw(12) << r_s_painting_area[i]
		//	<< setw(12) << (int)((r_s_painting_area[i] / grid_try_sum[i])*100.)

		//	<< setw(12) << (int)(render_brush_size[i] * render_brush_size[i] * grid_try_sum[i] / r_s_painting_area[i])<<
			"  " <<g_paint_try_scale[i] << endl; 
		
		}
		else {
			r_cout << setw(15) << to_string(i)+","+m_tag<< "," <<
				setw(6) << QT_grid_count[i] << ", " <<
				setw(6) << grid_try_sum[i] << ", " <<

				setw(8) << "**/0"//(int)(grid_painting_try[i] / QT_grid_count[i])
				<<
				setw(8) << r_s_changed_count[i] << ",  " <<
				std::fixed <<
				std::setw(9) << std::setprecision(2) <<"******/0"<<
				//(int)((float)changed_count[i] / (float)(grid_painting_try[i])* 100.) <<
				setw(6) << render_brush_size[i] <<
				setw(6) << render_brush_size[i] * render_brush_size[i] <<
				setw(10) << render_brush_size[i] * render_brush_size[i] * grid_try_sum[i]
				//<< setw(17) << r_s_painting_area[i]
				//<< setw(15) << "**/0"//nt)((painting_area[i] / grid_painting_try[i])*100.)

			//	<< setw(12) <<"**/0"// (int)(render_brush_size[i] * render_brush_size[i] * grid_painting_try[i] / painting_area[i])
				<< endl;
		}
		Qt_sum += QT_grid_count[i];
		try_sum += grid_try_sum[i];
		//	t_sum += try_count[i];
		c_sum += r_s_changed_count[i];

		g_file_cstat << g_para_method + "," << g_image_name << ", " << "depth:pr_count changed: " + to_string(i) + "," <<

			grid_try_sum[i] << "," << to_string(r_s_changed_count[i]) << endl;
	}

	r_cout << setw(12) <<"qt_sum "<< Qt_sum << setw(18) << "try sum "<<try_sum << setw(8) << "c_sum" <<c_sum << endl;

}

void  render_::rectangle_canvas(cv::Mat mat, cv::Rect  rect, Scalar s) {

	rect.x = rect.x + x_canvas_bezel_size;
	rect.y = rect.y + x_canvas_bezel_size;
	cv::rectangle(mat, rect, s);
}

void  render_::post_process() {
	int ret;
	if (success_or_fail == 0) {
		image_save(m_t+g_image_name, result_image, r_grid_map_1c_accu);

		
			ret = draw_grid_2(result_image.clone(), render_region_set, "p" + m_t, render_depth, //-1, 
				255, m_t);

		func_();
	}
	//m_depth, changed_count, QT_grid_count, grid_painting_try, render_brush_size, painting_area);
}
void  render_::add_gradient_map(int gradient_type, Mat a_map) {
	gradient_map[gradient_type] = a_map;
}

int render_::prepare() {

	int ret;
	
	int k_depth;
	int s_depth;
	if (g_src_image_width < g_src_image_height)
		x_BrushMaxSize = g_src_image_width / g_image_divider;
	else
		x_BrushMaxSize = g_src_image_height / g_image_divider;

	x_canvas_bezel_size = x_BrushMaxSize*g_brush_scale[0];

	x_src_canvas_Rect_full.width = x_src_image_size.width;
	x_src_canvas_Rect_full.height = x_src_image_size.height;
	x_src_canvas_Rect_full.x = x_canvas_bezel_size;//bezel size decided at prepare()
	x_src_canvas_Rect_full.y = x_canvas_bezel_size;
	x_canvas_size_width = g_src_image_width + x_canvas_bezel_size * 2;
	x_canvas_size_height = g_src_image_height + x_canvas_bezel_size * 2;
	x_canvas_step1 = x_canvas_size_width * x_src_channels;
		;
	x_src_canvas.create(x_canvas_size_height, x_canvas_size_width, CV_8UC3);
	x_src_canvas.setTo(255);


	Mat src_ROI_full = x_src_canvas(x_src_canvas_Rect_full);
	x_srcImg_.copyTo(src_ROI_full);
	//Rendering
	

	//paint_map = new Mat(canvas_size_height, canvas_size_width, CV_8UC3);
	//	paint_map_data = paint_map->data;
	//	paint_map->setTo(255);
	
	r_cout << "************* prepare start >" << m_tag << " <********************************************" << endl;
	
	r_grid_map_1c_accu = gradient_map[render_method].clone();
	r_grid_map_1c[MAX_DEPTH] = gradient_map[render_method].clone();
	gradient_inverse = 255-(r_grid_map_1c[MAX_DEPTH]);
	//	 cv::threshold((r_grid_map_1c[MAX_DEPTH]),gradient_inverse, (double)30, 30, 1); ;
	 debug_image("gradient_inverse", gradient_inverse);
	//m_aRegion_set = new list<Img_node*>();
	//for (int i = 0; i < MAX_DEPTH; i++)
	//	r_cout << "aRegion_set[" << setw(2) << to_string(i) + "]->size().initialize = " + m_tag << ", " << render_region_set[i].size() << endl;
	 int gradient_type;
	if (render_method != RENDER_ENHANCE && render_method != RENDER_MERGE) {
		//if (render_method != RENDER_SOBEL) {
		//	ret = QuadTree::TakeQuadTree_grid(gradient_map[render_method], render_region_set, m_tag);
		//}
		//else
		switch (render_method) {
		case RENDER_SOBEL:
			gradient_type = Gradient_Sobel;
			break;
		case RENDER_SALIENCY:
			gradient_type = Gradient_Saliency_C;
			break;
		case RENDER_UNION:
			gradient_type = Gradient_Union;
			break;
		case RENDER_MERGE:
			gradient_type = Gradient_Union;

			break;
		case RENDER_ENHANCE:
			gradient_type = Gradient_Union;
			break;
		default:
			r_cout << "render_method error in setting gradient image " << render_method<<endl;
			gradient_type = Gradient_Sobel;
		}
		ret = TakeQuadTree(gradient_map[gradient_type], render_region_set, m_tag);
	}
	else {
		int saved_depth = -1;
		partition_Node *me_node;
	//	Imginfo info;
		int  S;
	//	Region_set Region_set_sobel[MAX_DEPTH];
	//	Region_set Region_set_saliency[MAX_DEPTH];
		
	

			//region_dump(Region_set_sobel, m_tag + " : sobel depth = ", render_sobel->render_depth);

	//	for (int l = 0; l < render_saliency->render_depth; l++)
	//	region_dump(Region_set_saliency, m_tag + " : saliency depth  = ", render_saliency->render_depth);


			for (int i = 0; i < render_sobel->render_depth; i++) {
			//	r_cout << "astroke.size(_sobel)= " + m_tag << ", " << render_region_set[i].size() << endl;
			for (list<partition_Node*>::iterator partition_it = render_sobel->render_region_set[i].Region_list.begin(); 
				partition_it != render_sobel->render_region_set[i].Region_list.end(); partition_it++) {
			//	info = (*partition_it)->info;
				k_depth = (*partition_it)->depth;
				S = (*partition_it)->avgS;
				me_node = new 
					partition_Node((*partition_it)->srtPoint,(*partition_it)->endPoint, k_depth, S,g_no);
				g_no++;
				render_region_set[i].push_back(me_node);
			
			}
		}

		//r_cout << " sobel added " << kk << " strokes : to depth  " << enhance_mode_base_depth << endl;

	//	depth_sobel = k_depth;
		//render_region_set->sort();

		region_dump(render_region_set, "sobdel added",k_depth+1);

		int kk = 0;
	//	k_depth = 0;
		
			if (render_sobel->render_depth < render_saliency->render_depth) {
				enhance_mode_base_depth = render_sobel->render_depth-1;
			}
			else 	enhance_mode_base_depth = render_saliency->render_depth-1;
		
		for (s_depth = enhance_mode_base_depth; s_depth < render_saliency->render_depth; s_depth++) {
			//r_cout << "astroke.size()= " + m_tag << ", " << render_region_set[i].size() << endl;
			
			for (list<partition_Node*>::iterator partition_it = render_saliency->render_region_set[s_depth].Region_list.begin();
					partition_it != render_saliency->render_region_set[s_depth].Region_list.end(); partition_it++) {
			//	mod++;
				//if ((mod % 2) == 0) continue;
				if (render_method == RENDER_MERGE)
					k_depth = (*partition_it)->depth;
				else { //RENDER_ENHANCE
						k_depth = (*partition_it)->depth+1 ;
				}

			//	info = (*partition_it)->info;
				S = (*partition_it)->avgS;
				me_node = new partition_Node((*partition_it)->srtPoint,(*partition_it)->endPoint, k_depth, S,g_no);
				g_no++;
				render_region_set[k_depth].push_back(me_node);
				kk++;
			}
		}
		r_cout << " saliency added " << kk << ": " << k_depth << endl;
		
		//depth_saliency = render_saliency->render_depth;
		//render_depth=depth_Enhance = k_depth+1;
	//	render_depth = k_depth + 1;
		region_dump(render_region_set, "saliency added", k_depth+1);

		
	}// else render MODE 

	//m_aRegion_set->sort(Img_node::greater<Img_node*>());


//	sort(m_aRegion_set->begin(), m_aRegion_set->end());
	//m_aRegion_set->sort();
	render_depth = draw_grid_depth(r_grid_map_1c, r_grid_map_1c_accu, render_region_set, m_t,
		grid_map_sum, QT_grid_count//, true, -1, 0
	);
	/*
	k_depth = -1;
	int astroke_depth;
	int k = 0;
	
	for(int i=0;i<render_depth;i++){
		if (render_region_set[i].Region_list.size() == 0) {
			r_cout << m_tag + "Fail Sort check _aStrokeset_size() : " << k_depth << ": " << 
				render_region_set[i].Region_list.size() << endl;
		}
		
	for (list<partition_Node*>::iterator partition_it = render_region_set[i].Region_list.begin(); 
		partition_it != render_region_set[i].Region_list.end(); partition_it++, k++)
	{
		astroke_depth = (*partition_it)->depth;
		if (k_depth != -1) {
			if (astroke_depth < k_depth ||
				astroke_depth > k_depth + 1)
			{
				r_cout << "sort fail  " << k << "th : " << k_depth << " -> " << astroke_depth << endl;
			}
		}
		k_depth = astroke_depth;
	}
	r_cout << m_tag + " Sort check _aStrokeset_size() : " << k_depth<<": "<<render_region_set[i].Region_list.size() << endl;
	}
	*/
	/*for (int i = 0; i <= MAX_DEPTH; i++) {
	

		debug_image("ing/_or_grid_" + m_tag +
			"_" +  "_", i, r_grid_map_1c[i]);

	}
	*/
	
	//QT_depth = render_depth;
	
	//r_cout << "x depth " + m_tag + " = " << render_depth  << endl;
	/*
	for (int i = 0; i < render_depth ; i++)
	{
		r_cout << setw(15) << m_tag + ", " + to_string(i) <<", "<< QT_grid_count[i] << endl;
	}
	*/
	for (int i = 0; i < render_depth; i++) {
		list<partition_Node*>::iterator partition_it = render_region_set[i].Region_list.begin();
		if ((*partition_it)->region_size.width < (*partition_it)->region_size.height) 
			region_size[i] = (*partition_it)->region_size.width;
		else 
			region_size[i] = (*partition_it)->region_size.height;
	}

	calc_render_brush_size(x_BrushMaxSize, x_BrushMinSize, render_depth,
	render_brush_size, m_tag);

	
	//brush_resize( g_brush_vector_set);
	brush_pgm_resize(g__brush_pgm_list);
	////for (int i = 0; i < render_depth; i++) {
	//	r_cout << "Brush "<<i << ", " << render_brush_size[i] << endl;
	//}
//	for (int i = 0; i < render_depth; i++) {
//		r_cout << "QT stroke count(" + m_tag + ") " << QT_depth << " " << render_depth << " : " << render_region_set[i].size() << endl;
	//	r_cout.flush();
	//	if (render_region_set[i].size() == 0) {
	//		r_cout << "unexpected Stroke Tree" + m_tag + " = 0" << endl;
	//		return -20000;
	//	}
	//}
	//ret = draw_grid_2(x_srcImg_.clone(), m_aRegion_set, "src_0_" + m_tag, m_depth, -1, 0,m_tag);
	r_cout << "------------------------------------------------------------------" << endl;
	r_cout.flush();
	return 0;
}
