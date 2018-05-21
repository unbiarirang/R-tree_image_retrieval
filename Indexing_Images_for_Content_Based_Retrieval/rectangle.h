#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "point.h"

enum rectangle_type { POINT = 0, RECTANGLE };

class rectangle
{
public:
	rectangle(rectangle_type t);
	~rectangle();

	//��m��ż��������ʱ������m+1���㰴����ά������֮��������С��m/2������m/2�ֱ���Ϊ�µĽڵ㣬�м�һ�����븸�ڵ�
	//ˢ�´˾������������
	void renovate();
	//��������������Ҫ������һ�����εĻ�����Ҫ�����Ŵ��ۣ��˲�����������ģ�ͣ�
	int expand_cost(rectangle *new_rect);
	int expand_cost(rectangle *rect1, rectangle *rect2); // ������������ʱ�Ĵ���
	//�����µ�Ӧ���������ײ��м�ڵ�
	rectangle* search_insert_position(rectangle &new_rect);
	//����һ���µĽڵ�Ϊ�˽ڵ���ӽڵ�
	void insert(rectangle &new_rect);
	void insert_2(rectangle &new_rect);
	//�򵥵�ɾ���������Ƕ��������ֻ����insert�ڲ�����
	void delete_child(int index);

	//�����ݼ���
	void init_point_data(std::string _image_path, std::string _image_name, std::vector<int>* _data);
	// ѡ��һ������θ�����/��������ʱ�˷ѵ��Ŀռ�������/�������Ρ�nΪ����seed����
	std::vector<rectangle*> find_seed(rectangle & new_rect, int n);
	//���η���
	void split(rectangle & new_rect);
	void split_2to3(rectangle & sibling, rectangle & new_rect);
	//���򵥵ģ�����
	void naive_search(rectangle & target, std::vector<rectangle*>* result);
	// knn�������������need_count���ڵ�
	// max_dist����expand�����ݹ�ʱ��ֵ����knn_search����û��
	void knn_search(rectangle & target, std::vector<rectangle*>* result, int need_count, float &max_dist);
	void expand(rectangle & target, std::vector<rectangle*>* result, int need_count, float &max_dist);
	// ����Euclidean distance
	float get_min_distance(rectangle & node);
	float get_max_distance(rectangle & node);
	//�ж����������Ƿ��غ�
	bool overlap(rectangle & target);
	
	//�������ࣨ����߾��Σ�
	rectangle_type type;
	//��Ϊһ��������ݣ������þ���Ϊһ����ʱ�����Ϊnull��
	point* point_data;
	//��ɫά��
	int color_count;
	//���У��ýڵ�ĸ��ڵ�
	rectangle* parent;
	//���У��ýڵ���ӽڵ㣨�ǣ�
	std::vector<rectangle*> child;
	//��С������ݣ��൱�ڶ�ά�ľ������½ǣ�
	std::vector<int>* min_point;
	//��������ݣ��൱�ڶ�ά�ľ������Ͻǣ�
	std::vector<int>* max_point;
	//�����������ά������֮�˻���
	int volume;
};

