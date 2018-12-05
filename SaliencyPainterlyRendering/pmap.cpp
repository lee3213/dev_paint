#include "stdafx.h"

#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"
#include "define_local.h"
int render_::func_render_a_pixel(int src_x, int src_y, int astroke_depth) {
	Point centered_SrtPoint, centered_EndPoint;
	Point centered_SrtPoint_canvas, centered_EndPoint_canvas;
	int brush_area_h_size_half;
	int brush_area_w_size_half;
	int adjusted_h_size;
	int adjusted_w_size;
	Point fetch_color_Point, fetch_color_canvas_point;
	int color_BGR_B, color_BGR_G, color_BGR_R;
	//	int times;
	int brush_area_w_size, brush_area_h_size;
	Size Brush_size;
	Mat before_canvas_ROI;
	Mat before_canvas_ROI_clone;

	Mat changed_canvas_ROI;
	Mat changed_canvas_ROI_clone;

	Mat ing_ROI_canvas;

	cv::Mat src_canvas_ROI;
	Mat mat_accu;

	Mat mat_ing;
	brush_area_h_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
	brush_area_w_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
	Brush_size = Size(brush_area_w_size, brush_area_h_size);

	brush_area_h_size_half = brush_area_h_size / 2 + brush_area_h_size % 2;
	brush_area_w_size_half = brush_area_w_size / 2 + brush_area_w_size % 2;

	fetch_color_Point.x = src_x;
	fetch_color_Point.y = src_y;
	int fetch_Index = (fetch_color_Point.x + (fetch_color_Point.y*x_src_image_size.width))*(x_src_channels);
	int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*x_src_image_size.width);

	int fetch_canvas_Index = ((fetch_color_Point.x + x_canvas_bezel_size)
		+ (fetch_color_Point.y + x_canvas_bezel_size)*(x_canvas_size_width))*x_src_channels;

	p_peek(x_src_ptr, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);

	p_poke(r_try_map_1c_data[astroke_depth], fetch_Index_1c, 0);
	if (brush_area_w_size_half > x_canvas_bezel_size)
		adjusted_w_size = x_canvas_bezel_size;
	else
		adjusted_w_size = brush_area_w_size_half;

	if (brush_area_h_size_half > x_canvas_bezel_size)
		adjusted_h_size = x_canvas_bezel_size;
	else
		adjusted_h_size = brush_area_w_size_half;

	centered_SrtPoint.x = fetch_color_Point.x - adjusted_w_size;
	centered_SrtPoint.y = fetch_color_Point.y - adjusted_h_size;

	centered_EndPoint.x = fetch_color_Point.x + (adjusted_w_size);// to overcome odd brush size
	centered_EndPoint.y = fetch_color_Point.y + (adjusted_h_size);// to overcome odd brush size

	centered_SrtPoint_canvas.x = centered_SrtPoint.x + x_canvas_bezel_size;
	centered_SrtPoint_canvas.y = centered_SrtPoint.y + x_canvas_bezel_size;
	centered_EndPoint_canvas.x = centered_EndPoint.x + x_canvas_bezel_size;
	centered_EndPoint_canvas.y = centered_EndPoint.y + x_canvas_bezel_size;

	Rect centered_ROI_canvas_Rect = Rect(centered_SrtPoint_canvas, centered_EndPoint_canvas);

	src_canvas_ROI = x_src_canvas(centered_ROI_canvas_Rect);
	//	src_canvas_ROI_clone = src_canvas(centered_ROI_canvas_Rect).clone();

	mat_accu = accu_canvas[astroke_depth];
	mat_ing = ing_canvas[astroke_depth];
	//before_canvas_ROI = mat_accu(centered_ROI_canvas_Rect);
	before_canvas_ROI_clone.release();
	before_canvas_ROI_clone = mat_accu(centered_ROI_canvas_Rect).clone();

	changed_canvas_ROI = mat_accu(centered_ROI_canvas_Rect);
	changed_canvas_ROI_clone.release();
	changed_canvas_ROI_clone = mat_accu(centered_ROI_canvas_Rect).clone();

	ing_ROI_canvas = mat_ing(centered_ROI_canvas_Rect);
	int result = P_Rendering(
		src_canvas_ROI,
		before_canvas_ROI_clone,
		changed_canvas_ROI_clone,
		ing_ROI_canvas,
		changed_canvas_ROI,
		mat_accu,
		centered_ROI_canvas_Rect,
		fetch_color_Point,
		centered_SrtPoint,
		centered_SrtPoint_canvas,
		centered_EndPoint_canvas,
		adjusted_w_size, adjusted_h_size,

		astroke_depth, render_try,
		color_BGR_B, color_BGR_G, color_BGR_R,

		x_src_step1,
		x_canvas_step1,
		x_src_ptr,
		accu_canvas_data[astroke_depth],
		ing_canvas_data[astroke_depth],
		1,
		0
	);
	if (result == CHANGED_BETTER)
		r_s_changed_count[astroke_depth]++;

	before_canvas_ROI.release();
	before_canvas_ROI_clone.release();

	changed_canvas_ROI.release();
	changed_canvas_ROI_clone.release();

	ing_ROI_canvas.release();

	src_canvas_ROI.release();
	return result;
}

int  render_::pad_p_map(Mat & a_pmap_accu_canvas_8UC1, Rect r_given_depth,string _pmap_path, int _depth)
{
	//int times;
	//partition_Node* region_p;
	int canvas_x, canvas_y, astroke_depth = _depth;
	int _r_width;
	int _r_height;
//	int sum_x;
	//int sum_y;
	int c;
	int ret;
	int _x, _y;
	
	Mat nonZeroCoordinates;
	Mat nonzero_ROI;
	Rect r_canvas;

	int k_skip = 300;
	int tot_cnt = 0;
	int effective_try = 0;
//	int __saved_depth;
	int saved_c;
	int erased = 0;
	Mat rendered_accu_canvas_clone;
	//int ssize;
	Mat diff;
	int val;
	unsigned char * pmap_accu_ptr = paint_map_accu_canvas_8UC1[astroke_depth].data;
	Mat a_pmap_accu_canvas_8UC1_clone_inv = 255 - a_pmap_accu_canvas_8UC1.clone();
	int region_layer = x_changed_depth ;
	int ssize;
	do {
		ssize = (int)render_region_set[region_layer].Region_list.size();
		if (ssize == 0) {
			region_layer = 2;
		}
	} while (ssize == 0);

	list<partition_Node*>::iterator partition_it = render_region_set[region_layer].Region_list.begin();
	//__saved_depth = (*partition_it)->depth;
	//Rect r_;
	Rect r_((*partition_it)->srtPoint, (*partition_it)->endPoint);
	int errcnt = 0;
	_r_width = r_.width;
	_r_height = r_.height;
	int p_x, p_y;
	int src_x, src_y;
	int x_divider = x_src_image_size.width / _r_width;

	int y_divider = x_src_image_size.height / _r_height;

	int x_remainder = x_src_image_size.width - (_r_width * x_divider);
	int y_remainder = x_src_image_size.height-(_r_height * y_divider);
	int layer = _depth;

	debug_image(_pmap_path + "_i_pmap_" + to_string(layer) + "_",
		a_pmap_accu_canvas_8UC1_clone_inv);
	rendered_accu_canvas_clone = accu_canvas[_depth].clone();//rendered image
	debug_image(_pmap_path + "_r_ac_b_" + to_string(layer), rendered_accu_canvas_clone);
	random_device rand_x;
	int random_x;
	int rand_err = 0;
	int turns = 0;
	Mat before_pmap_accu;
	int cnz;
	Mat prev_pmap_accu;
	Mat diff2;
	int cnz2;
	before_pmap_accu = paint_map_accu_canvas_8UC1[layer].clone();
	do {
		tot_cnt = 0;
		for (_y = 0; _y < y_divider-1; _y++) {
			rendered_accu_canvas_clone = accu_canvas[_depth].clone();

			r_.y = _y*_r_height;
		//	if (_y == (y_divider - 2)) {
		//		r_.height += y_remainder;
		//	}
			
			prev_pmap_accu = paint_map_accu_canvas_8UC1[layer].clone();
			for (_x = 0; _x < x_divider-1; _x++) {
				r_.x = _x*_r_width;
		////		if (_x == (x_divider - 2)) {
				//	r_.width += x_remainder;
			//	}

				r_canvas = r_;//size(width, height)
				r_canvas.x = r_.x + x_canvas_bezel_size;
				r_canvas.y = r_.y + x_canvas_bezel_size;

				rectangle(rendered_accu_canvas_clone, r_canvas, 0);
				rectangle(a_pmap_accu_canvas_8UC1_clone_inv, r_canvas, 0);

				nonzero_ROI = a_pmap_accu_canvas_8UC1(r_canvas);
				//	debug_image(_pmap_path + "_nz_" + to_string(_y) + "_" + to_string(_x),nonzero_ROI);
				cv::findNonZero(nonzero_ROI, nonZeroCoordinates);
				c = (int)nonZeroCoordinates.total();
				cnz2 = countNonZero(nonzero_ROI);
				if (c != cnz2) {
					r_cout << "c= " << c << " cnz2 = "<<cnz2<<endl;
				}
				tot_cnt += c;
				//sum_x = 0; sum_y = 0;
				saved_c = c;
				if (c > k_skip) {
					uniform_int_distribution<int> diStrk_x(0, (int)(c - 1));

					do {
						random_x = diStrk_x(rand_x) % c;
						if (random_x > c)
							rand_err++;
					} while (random_x > c);
					p_x = nonZeroCoordinates.at<Point>(random_x).x;
					p_y = nonZeroCoordinates.at<Point>(random_x).y;

					canvas_x = r_canvas.x + p_x;

					canvas_y = r_canvas.y + p_y;
					src_x = r_.x + p_x;
					src_y = r_.y + p_x;

					rectangle(rendered_accu_canvas_clone, Rect(canvas_x, canvas_y, 2, 2), 0);


					int index = x_canvas_size_width*canvas_y + canvas_x;
					ret = func_render_a_pixel(r_.x, r_.y, astroke_depth);
					effective_try++;
					p_peek_1c(pmap_accu_ptr, index, val);
					if (val != 0) {//skip when nothing changed 
						p_poke_1c(pmap_accu_ptr, index, 0);
						errcnt++;
					}//if nothing changed

				}//if c > skip
			}//x
		}//y
		turns++;
		diff = abs(before_pmap_accu - paint_map_accu_canvas_8UC1[layer]);
		diff2 = abs(prev_pmap_accu - paint_map_accu_canvas_8UC1[layer]);
		 cnz = countNonZero(diff2);
		debug_image(_pmap_path + "_diff_"  + "_" + to_string(layer) + "_" + to_string(turns), diff);
		debug_image(_pmap_path + "_accu_" + "_" + to_string(_depth) + "_" + to_string(turns), accu_canvas[_depth]);
	}
	while (cnz > 100);
	//	diff = abs(before_pmap_accu - paint_map_accu_canvas_8UC1[layer]);
	//	debug_image(_pmap_path + "_diff_"  + "_" + to_string(layer) + "_" + to_string(_y), diff);
	//	debug_image(_pmap_path +"__pmap_a5_"  + to_string(layer) + "_" + to_string(_y) + "_" + to_string(_x), paint_map_accu_canvas_8UC1[layer]);
	//	debug_image(_pmap_path + "_ing_r_acu_"  + "_" + to_string(layer)+"_"+to_string(_y), rendered_accu_clone);
	//	debug_image(_pmap_path  + "__i_pmap_" + "_" + to_string(layer) + "_" + to_string(_y), a_pmap_accu_canvas_8UC1_clone_inv);

	
//	debug_image(_pmap_path + "/_o_pmap_" + m_t + "_" + to_string(layer) + "_" , overlay_droppeda_pmap_accu_canvas_8UC1_clone_inv);
	debug_image(_pmap_path + + "__r_ac_a_" + "_" + to_string(layer), rendered_accu_canvas_clone);
	//debug_image("pmap/r" + to_string(render_method) + "/_fd" + m_t, layer, overlay_dropped_map[layer]);
		/*accu = accu_canvas[_depth].clone();
		for (int mlayer = layer; mlayer == layer; mlayer++) {
			int ssize = (int)render_dropped_set[mlayer].Region_list.size();
			//r_cout << "z : stroke " << i<<" : "<<__saved_depth <<" , "<<aRegion_set[i].size() << endl;
			if (ssize == 0)
				continue;
			for (list<partition_Node*>::iterator partition_it = render_dropped_set[mlayer].Region_list.begin();
				partition_it != render_dropped_set[mlayer].Region_list.end(); partition_it++)
			{
				__saved_depth = (*partition_it)->depth;

				//cv::rectangle(render_::overlay_dropped_map[__saved_depth], Rect((*partition_it)->srtPoint,
				//		(*partition_it)->endPoint), RGB(0, 0, 0));
				r_ = Rect((*partition_it)->srtPoint, (*partition_it)->endPoint);
				r_canvas = r_;
				r_canvas.x = r_.x + x_canvas_bezel_size;
				r_canvas.y = r_.y + x_canvas_bezel_size;
				rectangle(accu, r_canvas, 0);
			}
		}
	*/
		debug_image(_pmap_path+"__acvs_a_" + to_string(_depth) + m_t, accu_canvas[_depth]);
	r_cout << "turns " << turns <<
		" effective_try " << effective_try << " saved " << saved_c << "  "

		<< fixed << setw(5) << setprecision(1) <<
		(float)(((float)saved_c / (float)tot_cnt)*(float)100.0) << 
		"errocnt "<<errcnt<<"rand_err "<<rand_err<<endl;
	//times = paint_a_stroke(region_p, 1, 1, Point(x, y), astroke_depth);
	return tot_cnt;
}
int render_::pmap_count_zero(Mat & a_map_canvas_8UC1, string tag, Rect x_src_canvas_Rect_full) {
	//	double minval, maxval;
	//	Point minloc, maxloc;
	//int i_minval, i_maxval;
	Mat ROI = a_map_canvas_8UC1(x_src_canvas_Rect_full);

	//cv::minMaxLoc(ROI, &minval, &maxval, &minloc, &maxloc);
	//i_minval = (int)minval;
	//i_maxval = (int)maxval;



	//int diff = i_maxval - i_minval;
	//if (diff == 0) diff = 1;
	//int step = (int)256 / diff;
	//	Mat p_map;
	//	a_map_canvas_8UC1.convertTo(p_map, -1, step);
	//	debug_image("ing/" + tag, p_map);

	//r_cout <<tag<< " minLoc " << minloc.x << ", " << minloc.y << " : " << (int)i_minval ;
	//	r_cout << " maxLoc " << maxloc.x << ", " << maxloc.y << " : " << (int)i_maxval ;
	//r_cout << " diff = " << diff << ", step = " << step ;

	int h = x_src_canvas_Rect_full.height;
	int w = x_src_canvas_Rect_full.width;
	//Mat paint_ROI = a_map_canvas_8UC1(x_src_canvas_Rect_full);
	int zero_pixel_cnt = (h*w) - cv::countNonZero(ROI);
	float ratio = (float)((float)zero_pixel_cnt / (float)(h*w))*(float)100.0;
	r_cout << tag << " zero pixel = [" << h << "," << w << "] " << zero_pixel_cnt << " / " << (h*w) <<
		" = " << std::fixed << setprecision(2) << ratio << endl;
	return zero_pixel_cnt;
}
void render_::pmap_overlay_fill(int from_uu_depth, Mat a_map_canvas_8UC1, int color) {
	Point Region_srtPoint, Region_endPoint;
	Size Region_size;
	int Region_w_size, Region_h_size;
	Point Strk_point_canvas;
	partition_Node* region_p;
	for (list<partition_Node*>::iterator partition_it = render_region_set[from_uu_depth].Region_list.begin();
		partition_it != render_region_set[from_uu_depth].Region_list.end(); partition_it++) {

		region_p = (*partition_it);

		Region_srtPoint = (region_p)->srtPoint;
		Region_endPoint = (region_p)->endPoint;
		Region_w_size = Region_endPoint.x - Region_srtPoint.x;
		Region_h_size = Region_endPoint.y - Region_srtPoint.y;

		Region_size = Size(Region_w_size, Region_h_size);

		Strk_point_canvas = Point(Region_srtPoint.x + x_canvas_bezel_size, Region_srtPoint.y + x_canvas_bezel_size);
		Rect Strk_canvas_ROI_rect(Strk_point_canvas, Region_size);
		Mat a_map_canvas_8UC1_ROI = a_map_canvas_8UC1(Strk_canvas_ROI_rect);

		a_map_canvas_8UC1_ROI.setTo(color);
	}
}

/*
for (int layer = x_changed_depth; layer < (render_depth); layer++) {
	ssize = (int)render_dropped_set[layer].Region_list.size();
	if (ssize == 0)
		continue;
	list<partition_Node*>::iterator partition_it = render_dropped_set[layer].Region_list.begin();
	__saved_depth = (*partition_it)->depth;

	r_ = Rect((*partition_it)->srtPoint, (*partition_it)->endPoint);
	break;
}
*/