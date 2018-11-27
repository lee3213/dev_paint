// Saliencyrender_image.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "thread"
//#include "Saliencyrender_image.h"
#include "util.h"
using namespace std;
using namespace cv;
#include "extern.h"
#include "debug_image.h"

// 유일한 응용 프로그램 개체입니다.
#include "util.h"
#include "time_stamp.h"
#include "render_.h"

void run_render(render_ * _render) {
	//for (int j = 0; j < _render->render_depth; j++)
		//if (_render->render_region_set[j].Region_list.size() == 0) {
			//cout << _render->m_tag << " stroke_size==0" << endl;
		//	return ;
	//	}

	_render->success_or_fail = _render->render_image();
//	image_save(g_image_name + _render->m_tag_, _render->result_image, _render->grid_map_1c[MAX_DEPTH]);
	}


int do_rendering(){
		
		//return 997788;

#ifdef RUN_THREAD	
		std::thread *p_render[RENDER_MAX];

		p_render[0] = new thread(run_render, _render[RENDER_SOBEL]);
		cout << "p_sobel  t_id : " << p_render[RENDER_SOBEL]->get_id() << endl;
		std::cout << "Number of threads = " 
			<< std::thread::hardware_concurrency() << std::endl;
		p_render[RENDER_SOBEL]->join();
		_render[RENDER_SOBEL]->post_process();

		if (g_saliency_method != SALIENCY_STR_SOBEL) {
			p_render[RENDER_SALIENCY] = new thread(run_render, _render[RENDER_SALIENCY]);
			p_render[RENDER_SALIENCY]->join();
			p_render[RENDER_UNION] = new thread(run_render, _render[RENDER_UNION]);
			p_render[RENDER_UNION]->join();
			p_render[RENDER_TWOPASS_MERGE] = new thread(run_render, _render[RENDER_TWOPASS_MERGE]);
		
			p_render[RENDER_TWOPASS_MERGE]->join();
			
			cout << "p_saliency t_id: " << p_render[RENDER_SALIENCY]->get_id() << endl;
			cout << "p_union    t_id: " << p_render[RENDER_UNION]->get_id() << endl;
			cout << "p_twopass MERGE t_id: " << p_render[RENDER_TWOPASS_MERGE]->get_id() << endl;
			
			std::cout << "Number of threads Twopass = "
				<< std::thread::hardware_concurrency() << std::endl;
			
			
			
			p_render[RENDER_TWOPASS_ENHANCE] = new thread(run_render, _render[RENDER_TWOPASS_ENHANCE]);
		
			p_render[RENDER_TWOPASS_ENHANCE]->join();
			cout << "p_twopass  ENHANCE t_id: " << p_render[RENDER_TWOPASS_ENHANCE]->get_id() << endl;
			_render[RENDER_SALIENCY]->post_process();
			_render[RENDER_UNION]->post_process();
			_render[RENDER_TWOPASS_MERGE]->post_process();
			_render[RENDER_TWOPASS_ENHANCE]->post_process();
		}

#else

		std::thread *p_render[RENDER_MAX];

		p_render[0] = new thread(run_render, _render[RENDER_SOBEL]);
		p_render[RENDER_SOBEL]->join();
		_render[RENDER_SOBEL]->post_process();
		if (g_saliency_method != SALIENCY_STR_SOBEL) {
			p_render[RENDER_SALIENCY] = new thread(run_render, _render[RENDER_SALIENCY]);
			p_render[RENDER_SALIENCY]->join();
			p_render[RENDER_UNION] = new thread(run_render, _render[RENDER_UNION]);
			p_render[RENDER_UNION]->join();
			p_render[RENDER_TWOPASS_MERGE] = new thread(run_render, _render[RENDER_TWOPASS_MERGE]);
			p_render[RENDER_TWOPASS_MERGE]->join();
			p_render[RENDER_TWOPASS_ENHANCE] = new thread(run_render, _render[RENDER_TWOPASS_ENHANCE]);
			p_render[RENDER_TWOPASS_ENHANCE]->join();
				
			

			_render[RENDER_SALIENCY]->post_process();
			_render[RENDER_UNION]->post_process();
			_render[RENDER_TWOPASS_MERGE]->post_process();
			_render[RENDER_TWOPASS_ENHANCE]->post_process();
		}
		
#endif

	return 0;
}

