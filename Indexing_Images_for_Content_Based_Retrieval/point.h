#pragma once

#include <iostream>
#include <vector>
#include <string>

//矩形内部最多存储的子矩形数
#define RECTANGLE_CAPABILITY 5
//颜色维数
#define COLOR_COUNT 3

class point
{
public:
	point();
	~point();

	//图片路径
	std::string image_path;
	//图片名称
	std::string image_name;
	//颜色维数
	int color_count;
	//储存颜色具体数据
	std::vector<int>* data;

};

