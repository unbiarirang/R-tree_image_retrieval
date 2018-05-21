#pragma once
class color
{
public:
	color();
	~color();



	//颜色数据
	int r;
	int g;
	int b;
};

//判断两个颜色是否能够区分（解析图片时用到）
bool color_distinguish(color color_1, color color_2);