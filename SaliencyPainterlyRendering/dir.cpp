#include "stdafx.h"
#include <comdef.h> 
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include "dir.h"
using namespace std;
bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}
bool fileExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)//0x10
		cout << " FILE_ATTRIBUTE_DIRECTORY)" << endl;
	if (ftyp & FILE_ATTRIBUTE_NORMAL)//0x80
		return true;   // this is a directory!
	if (ftyp & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)//0x2000
		cout << "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" << endl;
	if (ftyp & FILE_ATTRIBUTE_HIDDEN)//0x01
		cout << "FILE_ATTRIBUTE_HIDDEN" << endl;//0x02
	if (ftyp & FILE_ATTRIBUTE_READONLY)
		cout << "FILE_ATTRIBUTE_READONLY" << endl;//0x01

	if (ftyp & FILE_ATTRIBUTE_ARCHIVE) {
		cout << " FILE_ATTRIBUTE_ARCHIVE" << endl;//0x20
		return true;
	}
		if (ftyp & FILE_ATTRIBUTE_DEVICE)
		cout << " FILE_ATTRIBUTE_DEVICE" << endl;//0x40
	return false;    // this is not a directory!
}
std::string file_attributes(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return "INVALID_FILE_ATTRIBUTES";  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return "FILE_ATTRIBUTE_DIRECTORY";   // this is a directory!
	
	return false;    // this is not a directory!
}








/*



기존 디렉토리가 있을경우 안만들어지고 없으면 만든다. 부모디렉토리가 없어도 생성가능



*/

// char to LPCWSTR
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	size_t cn;
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs_s(&cn,pwstrDest,nLen, pstrSrc, nLen);
}


// LPCWSTR to char
void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[])
{
	size_t cn;
	int nLen = (int)wcslen(pwstrSrc);
	wcstombs_s(&cn,pstrDest, nLen,pwstrSrc, nLen + 1);
}

int  CreateDir(char* Path)

{

	char DirName[256];  //생성할 디렉초리 이름

	char* p = Path;     //인자로 받은 디렉토리

	char* q = DirName;

	int  s_or_f;

	wchar_t dir_lp[256];
	while (*p)

	{

		if (('\\' == *p) || ('/' == *p))   //루트디렉토리 혹은 Sub디렉토리

		{

			if (':' != *(p - 1))//drive qualifier?? 

			{
				CharToWChar(DirName, dir_lp);
				s_or_f=CreateDirectory(dir_lp, NULL);
				if (s_or_f == 0) {
					int errcode=GetLastError();
					if (errcode == ERROR_ALREADY_EXISTS) {
						cout << "ERROR_ALREADY_EXISTS";
					}
					else if (errcode == ERROR_PATH_NOT_FOUND) {
						cout << "ERROR_PATH_NOT_FOUND";
					}
					return errcode;
				}

			}

		}

		*q++ = *p++;

		*q = '\0';

	}

	CharToWChar(DirName, dir_lp);
	s_or_f = CreateDirectory(dir_lp, NULL);
//	s_or_f = CreateDirectory(DirName, NULL);
	if (s_or_f == 0) {
		int errcode = GetLastError();
		if (errcode == ERROR_ALREADY_EXISTS) {
			cout << "ERROR_ALREADY_EXISTS";
		}
		else if (errcode == ERROR_PATH_NOT_FOUND) {
			cout << "ERROR_PATH_NOT_FOUND";
		}
		return errcode;
	}

return 0;
}





/*



하위디렉토리를 제외한 해당 디렉토리 모든 파일들을 제거



*/
void er_message(int ercode) {
	wchar_t buf[256];

	size_t size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, ercode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	char a[512];
	size_t cnt;
	sprintf_s(a, "%ls", buf);
	wcstombs_s(&cnt, &a[0], 512, &buf[0], size);
	cout << a << endl;
}

void err_message(int hresult) {
	LPSTR errorText = NULL;

	int size = FormatMessageA(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		(short)hresult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note 


	if (NULL != errorText)
	{
		// ... do something with the string `errorText` - log it, display it to the user, etc.
	//	static char buffer[1024];
	//	sprintf_s(buffer, sizeof(buffer), "ERROR: %s",  errorText);
	//	cout << hresult << " : " << buffer << endl;

//		CString s;
//		s = errorText;
//		sprintf_s(buffer, sizeof(buffer), "ERROR: %s", s.GetBuffer());
//		cout << hresult<< " : " << buffer << endl;
	//	cout << hresult << " : " << s << endl;
		std::string message(errorText, size);
		cout << hresult << " : " << message.c_str() << endl;
		// release memory allocated by FormatMessage()
		LocalFree(errorText);
		errorText = NULL;
	}
}

void DeleteAllFiles(string folderPath)

{

	char fileFound[256];

	WIN32_FIND_DATA info;

	HANDLE hp;


	wchar_t fileFound_lp[256];
	wchar_t file_delete_lp[256];
	sprintf_s(fileFound, "%s\\*.*", folderPath.c_str());
	CharToWChar(fileFound, fileFound_lp);

	hp = FindFirstFileW(fileFound_lp, &info); //디렉토리에 파일이 있는지 첫번째 파일만.
	if (hp == INVALID_HANDLE_VALUE) {
		int ercode = GetLastError();

		err_message(ercode);
		return;
	}

	int res;
	do

	{

		_bstr_t b(info.cFileName);
		const char* c = b;
		//	cout << "cFilename : " << c << endl;

		sprintf_s(fileFound, "%s\\%s", folderPath.c_str(), c);
		CharToWChar(fileFound, file_delete_lp);
		//	cout << fileFound << endl;
		if ((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			//	cout << "skipped directory "<< b<<" "<<c << endl;
		}
		else {
			if (!strcmp(c, ".")) {// cout << "skipped . " << endl;
				continue;
			}
			if (!strcmp(c, "..")) {// cout << "skipped .. " << endl; 
				continue;
			}


			res = DeleteFileW(file_delete_lp);
			if (res == NULL) {
				int ercode = GetLastError();

				err_message(ercode);
			}

		}//else directory
	} while (FindNextFileW(hp, &info));  //다른 파일이 있을때 까지



	FindClose(hp);

}


//create_batch(string("..\\x64\\Debug\\SaliencyPainterlyRendering"), string("\\render\\deployment"), string("\\render\\method"), string("source(512)"), string(".\\src"), string("_render.bat"));
//create_batch(string("\\render\\deployment"), string("\\render\\method"), string("\\render\\batch"),
	//string("..\\..\\x64\\Debug\\SaliencyPainterlyRendering"), string(".\\render\\deployment"), string(".\\render\\method"), string("\\render\\src"), string("source(512)"), string("_render.bat"));
//create_batch_a(
	//exe_path, deploy_folderPath, batch_dir + string("\\") + deploy_folderPath, image_file_path + "\\" + img_fname + string(".ppm"), img_fname + fname);


/*



해당 하는 디렉토리에 파일이 존재해도  디렉토리가 비어있지 않아도 지울수 있다 .



*/


/*
void EmptyDirectory(char* folderPath)

{

	char fileFound[256];

	WIN32_FIND_DATAW info;

	HANDLE hp;

	wchar_t fileFound_lp[256];

	sprintf(fileFound, "%s\\*.*", folderPath);
	CharToWChar(fileFound, fileFound_lp);
	
	hp = FindFirstFileW(fileFound_lp, &info);   //디렉토리에 파일이 있는지 첫번째 파일만.

	wchar_t cFilename_lp[256];
	do

	{

		//CharToWChar(info.cFileName, cFilename_lp);
		if (!((strcmp(info.cFileName, ".") == 0) || (strcmp(info.cFileName, "..") == 0)))

		{

			if ((info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)  //Sub디렉토리가 존재하는경우

			{

				string subFolder = folderPath;

				subFolder.append("\\");

				subFolder.append(info.cFileName);

				EmptyDirectory((char*)subFolder.c_str()); /// {return (_Ptr == 0 ? _Nullstr() : _Ptr); }

				RemoveDirectory(subFolder.c_str());

			}

			else

			{

				sprintf(fileFound, "%s\\%s", folderPath, info.cFileName);
				CharToWChar(fileFound, fileFound_lp);
				BOOL retVal = DeleteFile(fileFound_lp);

			}

		}



	} while (FindNextFile(hp, &info));



	FindClose(hp);

}
*/