#pragma once

#include <iostream>
#include <vector>
#include <string>

//�����ڲ����洢���Ӿ�����
#define RECTANGLE_CAPABILITY 5
//��ɫά��
#define COLOR_COUNT 3

class point
{
public:
	point();
	~point();

	//ͼƬ·��
	std::string image_path;
	//ͼƬ����
	std::string image_name;
	//��ɫά��
	int color_count;
	//������ɫ��������
	std::vector<int>* data;

};

