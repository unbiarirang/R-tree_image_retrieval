#pragma once

#include <iostream>
#include <vector>
#include <string>

//�����ڲ����洢���Ӿ�����
#define RECTANGLE_CAPABILITY 15
//��ɫά��
#define FEATURE_COUNT 16
class point
{
public:
	point();
	~point();

	//ͼƬ���
	int ID;
	//ͼƬ·��
	std::string image_path;
	//ͼƬ����
	std::string image_name;
	//��ɫά��
	int color_count;
	//������ɫ��������
	std::vector<int>* data;

};

