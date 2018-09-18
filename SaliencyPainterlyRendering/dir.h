#pragma once

#include "stdafx.h"

#include <windows.h>
#include <string>
bool dirExists(const std::string& dirName_in);
bool fileExists(const std::string& dirName_in);
void DeleteAllFiles(string folderPath);
bool delete_file(string fileName)
; 
void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
void WCharToChar(const wchar_t* pwstrSrc, char pstrDest[]);
void create_batch_a(string , string batch_dir,string exe,string file_folderPath, string img_fname, string fname_deploy);
void er_message(int ercode);
void err_message(int hresult);