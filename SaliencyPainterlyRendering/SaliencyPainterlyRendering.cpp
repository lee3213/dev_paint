// SaliencyPainterlyRendering.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "SaliencyPainterlyRendering.h"
#include "util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define CREATE_BATCH 1
#include "use_json.h"
#include "set_result_path.h"
#include "g_setup.h"
// 유일한 응용 프로그램 개체입니다.


#include "opencv2/saliency.hpp"
#include "saliency_blackandwhite.h"
using namespace std;
using namespace cv;

using namespace saliency;
#include "RenderingImage.h"
#include "global.h"

#include "saliency_main.h"
#include "dir.h"

#include "json_write.h"
#include "set_result_path.h"

int Brush_adjust(vector <Brush*> &brush_set);
/*
void create_batch(string from_dep, string from_method,string batch_dir,
	string exe_path, string deploy_folderPath, string method_folderPath, string image_file_path,string img_fname, string extension_name)
{
	create_batch_a( from_dep, batch_dir,
		exe_path, deploy_folderPath,  image_file_path+"\\"+img_fname + string(".jpg"), img_fname + extension_name);
	create_batch_a( from_method,batch_dir,
		exe_path, method_folderPath, image_file_path+"\\"+img_fname + string(".jpg"), img_fname +string("_method")+ extension_name);

}
*/

void func_a(string lst_f_path, string lst_f_para, string b_render_path,int para_cnt,string para[], int k_depth,string tag) {
	string lst_f_name = lst_f_path + string("image_lst.lst");
	std::ifstream file_lst_name;
	std::ifstream file_name;
	string lst_list_name;
	string list_name2[100];
	string f_name[100];
	string lst_f_name_2;
	//int i = 0;
	int f_cnt = 0;
	int list_cnt;
	string cmd;
	string img_name;
	file_lst_name.open(lst_f_name.c_str());
	
	while (!file_lst_name.eof()) {
		f_cnt = 0;
		lst_list_name = "";
		file_lst_name >> lst_list_name;
		if (lst_list_name.size() > 1) {
			f_name[f_cnt] = lst_list_name;
			f_cnt++;
		}
		else {
			cout << "size <= 1 : 1) "<< lst_f_name<<"  "<<lst_f_para<<"   " << b_render_path<<"  "<<lst_list_name.size() << "::::" << f_cnt<<" "<<tag << endl;
			continue;
		}
		//	cout << setw(5) << i << list_name << endl;
		//i++;
		lst_f_name_2 = lst_f_path + lst_list_name;
		file_name.open(lst_f_name_2.c_str());
		std::ofstream bat_call;
		string bat_cmd;
		string bat_f;
		bat_f = "\\batch\\"+to_string(k_depth)+"_" + lst_list_name + "_Itti_Sobel.bat";
		bat_call.open(bat_f.c_str());
		list_cnt=0;
		while (!file_name.eof()) {
			img_name = "";
			file_name >> img_name;
			if (img_name.size() > 1) {
				list_name2[list_cnt] = img_name;
				list_cnt++;
			}
			else {
				cout << "size <= 1 : 2) " << bat_f<<"  "<<img_name.size() << "::::" << list_cnt << " "<<tag<<endl;
				continue;
			}
			
			//cout << list_name2[list_cnt]<<endl;
			//add_batch_a("\\batch", b_render_path, list_name2[list_cnt], para, para_cnt);
			std::ofstream file_call;
			string bat_fcall_name;
			if (img_name == lst_list_name) {
				cout << img_name << ", " << list_cnt << endl;
				continue;
			}
			bat_fcall_name = "\\batch\\" + to_string(k_depth)+"_"+ lst_list_name +"_"+
				img_name + ".bat";
			file_call.open(bat_fcall_name.c_str());
			string T_bat_fcall_name = "\\batch\\" + to_string(k_depth) + "_" + lst_list_name + "_" +
				img_name + "_smItti_Sobel.bat";
			std::ofstream T_file_call;
			if (k_depth == 0)
			T_file_call.open(T_bat_fcall_name.c_str());
			for (int k = 0; k < para_cnt; k++) {
				if ( k_depth==1)
					cmd = "call " + b_render_path + "\\" + tag+para[k] + ".bat \\render\\src\\" + img_name;
				else cmd = "call " + b_render_path + "\\" + para[k] + ".bat " + img_name;
					file_call << cmd << endl;
				if (k_depth == 0) {
					bat_cmd = "call " + b_render_path + "\\" + para[k] + "_smItti.bat \\render\\src\\" + img_name;
					bat_call << bat_cmd << endl;
					T_file_call << bat_cmd << endl;

					bat_cmd = "call " + b_render_path + "\\" + para[k] + "_smSobel.bat \\render\\src\\" + img_name;
					bat_call << bat_cmd << endl;
					T_file_call << bat_cmd << endl;
				}
			}
			file_call.close();
			if (k_depth == 0)
			T_file_call.close();
		}
		file_name.close();
		bat_call.close();
	}
	file_lst_name.close();

}
void get_para(string lst_f_para, string para[], int &para_cnt) {
	std::ifstream file_para;
	file_para.open(lst_f_para.c_str());
	string str;
	while (!file_para.eof()) {
		str = "";
		file_para >> str;
		if (str.size() > 1) {
			para[para_cnt] = str;
			para_cnt++;
		}
		else {
			cout << "size <= 1 : 3) " << lst_f_para<<"   "<<para[para_cnt].size() << "::::" << para_cnt << endl;
		}
	}
	file_para.close();
}
int main(int argc, char *argv[], char *envp[]) {

	int nRetCode = 0;
//	bool exists;
	string src_name, saliency_name,deploy_name;


		


#if CREATE_BATCH ==  0
	
	string exe_path("C:\\dev_painterly_render_u_saliency\\x64\\Debug\\SaliencyPainterlyRendering.exe");
		

	check_and_create("\\render", false);
	check_and_create("\\render\\deployment",true);

	check_and_create("\\batch",true);
	check_and_create("\\batch\\render", true);
	string b_render_path("\\batch\\render");
	string list_path("\\render\\list\\");
	
	json_write_method(string("\\render\\deployment"),
		b_render_path,
		exe_path,
		list_path
	);

	string para[200];
	int para_cnt=0;
	string lst_f_path("\\render\\list\\");
	string lst_f_para = lst_f_path + string("para.lst");

	get_para(lst_f_para, para, para_cnt);
	func_a( lst_f_path, lst_f_para, b_render_path,para_cnt,para,0,"");

	para_cnt=0;
	lst_f_para = lst_f_path + string("para_meta.lst");
	get_para(lst_f_para, para, para_cnt);
	func_a(lst_f_path, lst_f_para,b_render_path, para_cnt, para,1,"para_");

#else
	if ( argc >2 ){
			bool exists;
			src_name = argv[1];
			cout << src_name << endl;
			exists=fileExists(src_name);
			if (exists == false) {
				cout << " NOT EXIST " << src_name << endl;
				return -1;
			}
			deploy_name = argv[2];
			cout << deploy_name << endl;
			exists = fileExists(src_name);
			if (exists == false) {
				cout << " NOT EXIST " << deploy_name << endl;
				return -2;
			}
			nRetCode=RenderingImage(argv[1], argv[2]);

		}
		
		else {
			nRetCode = -12345678;
			cout << "Error" << endl;
/*		//	cv::String f_name;
			HMODULE hModule = ::GetModuleHandle(nullptr);

			if (hModule != nullptr)
			{
				// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
				if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
				{
					// TODO: 오류 코드를 필요에 따라 수정합니다.
					wprintf(L"심각한 오류: MFC를 초기화하지 못했습니다.\n");
					nRetCode = 1;
				}
				else
				{
					// TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
					
					//sourceImage load
					//string szFilter = "All Image Files|*.BMP;*.GIF;*.JPG;*.JPEG;*.PNG;*.TIF;*.TIFF|All Files (*.*)|*.*||";
					LPCTSTR szFilter = _T( "All Image Files|*.BMP;*.GIF;*.JPG;*.JPEG;*.PNG;*.TIF;*.TIFF|All Files (*.*)|*.*||");
					CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
					fileDlg.m_ofn.lpstrTitle = _T("Load Source Image File");

					if (IDOK == fileDlg.DoModal())
					{
						CString name = fileDlg.GetPathName();
						CT2CA pszConvertedAnsiString(name);
						std::string s(pszConvertedAnsiString);
						src_name=s;
					

						}
					else

					{
						// TODO: 오류 코드를 필요에 따라 수정합니다.
						wprintf(L"심각한 오류: Get source image  실패\n");
						nRetCode = 1;
						return nRetCode;
					}
					//Saliency Image load
				
					CFileDialog fileDlg2(FALSE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
					fileDlg2.m_ofn.lpstrTitle = _T("Load Saliency Image File");

					if (IDOK == fileDlg2.DoModal())
						{
							CString name = fileDlg2.GetPathName();
							CT2CA pszConvertedAnsiString(name);
							std::string s(pszConvertedAnsiString);
							cv::String deploy_name(s);
						//	saliImg = cv::imread(f_name, CV_LOAD_IMAGE_GRAYSCALE);
						//	resize(srcImg, srcImg, cv::Size(saliImg.size().width, saliImg.size().height));
						//	//Brush initialization
						//	g_brush_set.resize(g_BrushNumber);
						//	BrushInitialization(g_brush_set);

							//Rendering
						
							 nRetCode=RenderingImage((char*)src_name.c_str(), (char*)deploy_name.c_str());
						//	imwrite("/rst/rstImg.ppm", rstImg);
					}
					else
					{
							// TODO: 오류 코드를 필요에 따라 수정합니다.
							wprintf(L"심각한 오류: get saliency file  실패\n");
							nRetCode = 1;
							return nRetCode;
					}
				}
				
			}//afx MFC
			else
			{
				// TODO: 오류 코드를 필요에 따라 수정합니다.
				wprintf(L"심각한 오류: GetModuleHandle 실패\n");
				nRetCode = 1;
			}//win init
			*/
		} // argc
#endif
    return nRetCode;
}
