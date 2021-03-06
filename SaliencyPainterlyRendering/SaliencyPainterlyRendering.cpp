// SaliencyPainterlyRendering.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "SaliencyPainterlyRendering.h"
#include "util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define CREATE_BATCH 0
#include "debug_image.h"
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

int Brush_adjust(list<Brush> &brush_set);

void create_batch(string from_dep, string from_method,string batch_dir,
	string exe_path, string deploy_folderPath, string method_folderPath, string image_file_path,string img_fname, string extension_name)
{
	create_batch_a( from_dep, batch_dir,
		exe_path, deploy_folderPath,  image_file_path+"\\"+img_fname + string(".jpg"), img_fname + extension_name);
	create_batch_a( from_method,batch_dir,
		exe_path, method_folderPath, image_file_path+"\\"+img_fname + string(".jpg"), img_fname +string("_method")+ extension_name);

}
int main(int argc, char *argv[], char *envp[]) {

	int nRetCode = 0;
	bool exists;
	string src_name, saliency_name,deploy_name;
#if CREATE_BATCH ==  1
	exists = fileExists("C:\\dev_painterly_rendering_u_saliency\\SaliencyPainterlyRendering\\x64\\Debug\\SaliencyPainterlyRendering.exe");
	cout << exists << endl;
	string exe("C:\\dev_painterly_rendering_u_saliency\\SaliencyPainterlyRendering\\x64\\Debug\\SaliencyPainterlyRendering.exe");
		
		//C:\\study_painter\\painter_saliency\\painter_saliency_180611\\SaliencyPainterlyRendering\\x64\\Debug\\SaliencyPainterlyRendering");
	check_and_create("\\render", false);
	check_and_create("\\render\\deployment",true);
	check_and_create("\\render\\method",true);
	check_and_create("\\render\\batch",false);
	//json_write(string("\\render\\deployment"));
	json_write_method(string("\\render\\deployment"));

	create_batch( string("\\render\\deployment"),string("\\render\\method"),string("\\render\\batch"),
		exe, string("\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("source(512)"),string("_render.bat"));
	create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
		exe, string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("source(1024)"), string("_render.bat"));
	create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
		exe, string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("100"), string("_render.bat"));
	create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
		exe, string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("998"), string("_render.bat"));
	create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
		exe, string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("photo"), string("_render.bat"));
	create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
		exe, string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("sy"), string("_render.bat"));
	create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
		exe, string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("cat_1920_1280"), string("_render.bat"));
#else
	if ( argc >2 ){
		if (argc > 1) {
			src_name = argv[1];
			cout << src_name << endl;
			exists=fileExists(src_name);
			if (exists == false) {
				cout << " NOT EXIST " << src_name << endl;
				return -1;
			}
			if (argc > 2)
				deploy_name = argv[2];

			else deploy_name = "deployment.json";
			cout << deploy_name << endl;
			exists = fileExists(deploy_name);
			if (exists == false) {
				cout << " NOT EXIST " << deploy_name << endl;
				return -2;
			}

	}
	

	//int iNumberLines = 0;    // Default is no line numbers.  

							 // If /n is passed to the .exe, display numbered listing  
							 // of environment variables.  

	
	

	
			nRetCode=RenderingImage(argv[1], argv[2]);

			//if ( nRetCode < 0 ) waitKey();
		}
		
		else {
		//	cv::String f_name;
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
		} // argc
#endif
    return nRetCode;
}
