#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "json/json.h"
using namespace std;
#include "extern.h"

int  json_deployment(string path_fname) {
	//	Json::Value root;
	cout << "path" << path_fname << " : " << path_fname.c_str() << endl;
	std::ifstream config_doc(path_fname.c_str());

	//config_doc >> root;

	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(config_doc, root);
	if (!parsingSuccessful)
	{
		// report to the user the failure and their locations in the document.
		std::cout << "Failed to parse configuration\n"
			<< reader.getFormattedErrorMessages();
		return -1;
	}


	// Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
	// such member.
	//std::string encoding = root.get("encoding", "UTF-8").asString();
	// Get the value of the member of root named 'encoding', return a 'null' value if
	// there is no such member.
	//const Json::Value plugins = root["plug-ins"];
	//for (int index = 0; index < plugins.size(); ++index)  // Iterates over the sequence elements.
	//	loadPlugIn(plugins[index].asString());

	//setIndentLength(root["indent"].get("length", 3).asInt());
	//setIndentUseSpace(root["indent"].get("use_space", true).asBool());

	//std::cout << root<<endl;
	//	g_method=root.get("method", "org").asString();
	int g_ts_count_x;
	int g_ts_count_y;
	
	int ts[20][MAX_DEPTH];
	string a;
	g_ts_count_y = root.get("count_y", 3).asInt();
	g_ts_count_x = root.get("count_x", MAX_DEPTH).asInt();
	for (int ts_y = 0; ts_y < g_ts_count_y; ts_y++) {
		for (int ts_x = 0; ts_x < g_ts_count_x; ts_x++) {
		a = "a" + to_string(ts_x);
		ts[ts_x][ts_y] = root.get(a, 1).asInt();
	}

	return 0;
}