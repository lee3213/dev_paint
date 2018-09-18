#include "stdafx.h"
#include "dir.h"

#include "extern.h"
	void check_and_create(string path,bool flag ){
	cout << "check_and_create  :: "<<flag<<" :: "<< path<<endl;
	if (dirExists(path)) {
		//cout << "dir  " << path << " Exists" << endl;
		if ( flag )
			DeleteAllFiles(path);
	}
	else {
		wchar_t dir_lp[256];
		//cout << "dir  " << path << " Not Exists" << endl;
		CharToWChar(path.c_str(), dir_lp);
		if (CreateDirectoryW(dir_lp, NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			//cout << dir_lp << "Created || Existsed" << endl;
		}
		else
		{
			cout << " Failed to create directory." << " " << path.c_str() << endl;
		}
	}
}

	void set_result_path(std::string para_path,string method_path,int depth) {
		string dirs[] = { "ing","sobel","saliency" ,"br","grid","br2"};
		string sub_dirs;
		string dir;
		dir = g_root_path_win + string("\\") + para_path ;
		cout << "dir " << g_root_path_win << " : " << dir << endl;
		check_and_create(dir,false);

		dir = g_root_path_win + string("\\") + para_path+"\\"+method_path;
		cout << "dir " << g_root_path_win << " : " << dir << endl;
		check_and_create(dir, false);
		
		dir = g_root_path_win + string("\\") + para_path + "\\" + method_path+"\\"+g_image_name;
		cout << "dir " << g_root_path_win << " : " << dir << endl;
		check_and_create(dir, true);

		for (int i = 0; i < sizeof(dirs)/sizeof(string) ; i++) {
			sub_dirs = dir;
			sub_dirs.append("\\");
			sub_dirs.append(dirs[i]);
			cout << i << " : " << sub_dirs << endl;
			check_and_create(sub_dirs,true);
		}
		for (int i = 0; i < depth; i++) {
			sub_dirs = dir;
			
			sub_dirs.append("\\p");
			sub_dirs.append(to_string(i));
			cout << i << " : " << sub_dirs << endl;
			check_and_create(sub_dirs,true);
		}



	}