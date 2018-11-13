#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "json/json.h"
using namespace std;
#include "extern.h"
int  json_deployment(string path) {
//	Json::Value root;
	cout << "path" << path<< " : "<<path.c_str()<<endl;
	std::ifstream config_doc(path.c_str());

	//config_doc >> root;

	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(config_doc, root);
	if (!parsingSuccessful)
	{
		// report to the user the failure and their locations in the document.
		std::cout << "Failed to parse configuration\n"
			<< reader.getFormattedErrorMessages();
		return -1 ;
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
	g_root_path = root.get("root_path", "/rst").asString();
	g_root_path_win = root.get("root_path_win", "\\rst").asString();

	//g_method_path = root.get("method_path", "rst").asString();

	g_depth_limit = root.get("g_depth_Threshhold", 6).asInt();

	g_min_gridsize = root.get("g_gridsize", 10).asInt();
	g_QT_avgSThreshold =root.get("QT_avgSThreshold",23).asInt();
//	g_grid_threshold = root.get("g_grid_threshold",20).asInt();


	g_BrushMinSize = root.get("g_BrushMinSize", 10).asInt();
	g_saliency_method = root.get("g_saliency_method", "wo").asString();
	g_brush_Ts = root.get("g_Ts", 230).asInt();
	
	g_paint_method = root.get("g_paint_method", "copy").asString();
	g_paint_area_scale = root.get("g_paint_area_scale", 8).asInt();
	cout << "g_paint_area_scale  "<<g_paint_area_scale << endl;
	//g_paint_area_scale = 8;
	g_paint_area_scale_0=g_paint_area_scale*2;

	g_QT_method_N= root.get("g_QT_method_N",1).asInt();
//	g_Render_method = root.get("g_Render_method", "Only").asString();
//	string str_g_merge_method = root.get("g_merge_method","attach").asString();
//	if ( str_g_merge_method == "Attach" ||
//		str_g_merge_method == "attach")
//	g_merge_method = DEF_ATTACH;
//	else
	//	g_merge_method = DEF_MERGE;
//
	g_BrushAttachSize = root.get("g_BrushAttachSize", 5).asInt();
//	g_merge_skip_count = root.get("g_merge_skip_count", 1).asInt();
	// And you can write to a stream, using the StyledWriter automatically.
	//std::cout << root<<endl;
	return 0;
}