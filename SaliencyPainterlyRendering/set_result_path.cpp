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

