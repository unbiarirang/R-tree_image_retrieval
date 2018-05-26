#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "point.h"

enum rectangle_type { POINT = 0, RECTANGLE };

class rectangle
{
public:
	rectangle(rectangle_type t, std::vector<int> * min_point_data = nullptr, std::vector<int> * max_point_data = nullptr);
	~rectangle();

	//��m������������ʱ������m+1���㰴����ά������֮��������С��m/2������m/2�ֱ���Ϊ�µĽڵ㣬�м�һ�����븸�ڵ�
	//ˢ�´˾������������
	void renovate();
	//��������������Ҫ������һ�����εĻ�����Ҫ�����Ŵ��ۣ��˲�����������ģ�ͣ�
	double expand_cost(rectangle *new_rect);
	double expand_cost(rectangle *rect1, rectangle *rect2); // ������������ʱ�Ĵ���
	//�����µ�Ӧ���������ײ��м�ڵ�
	rectangle* search_insert_position(rectangle &new_rect);
	//����һ���µĽڵ�Ϊ�˽ڵ���ӽڵ�
	void insert(rectangle &new_rect);
	//�򵥵�ɾ���������Ƕ��������ֻ����insert�ڲ�����
	void delete_child(int index);

	//�����ݼ���
	void init_point_data(std::string _image_path, std::string _image_name, std::vector<int>* _data);

	//���η���
	void split(rectangle & new_rect);		// �ѵ�ֳ����½Ǻ����Ͻ���������					
	void split_quadratic(rectangle & new_rect);	// Guttman's quadratic split,�ֳ���������
	void split_2to3(rectangle & new_rect);	// �������ֵ������ڵ�ֳ������ڵ㣬�ֵĴ����٣�װ�ر�����
	// ѡ��һ������θ�����/��������ʱ�˷ѵ��Ŀռ�������/�������Ρ�nΪ����seed����
	std::vector<rectangle*> find_seed(rectangle* new_rect);
	std::vector<rectangle*> find_seed(rectangle* sibling, rectangle* new_rect);
	//Ѱ�Ҹýڵ���ֵܽڵ㣨�����ж��Ƿ�ʹ��2-3split������
	rectangle * find_sibling(int & sibling_count, bool & use);

	// �ϲ�
	void merge(std::vector<rectangle*>* r1, std::vector<rectangle*>* r2);
	void merge(std::vector<rectangle*>* r1, std::vector<rectangle*>* r2, std::vector<rectangle*>* r3, rectangle* sibling);

	// ����
	// ������
	void naive_search(rectangle & target, std::vector<rectangle*>* result, int interval = -1);
	// knn�������������need_count���ڵ�
	// max_dist����expand�����ݹ�ʱ��ֵ����knn_search����û��
	void knn_search(rectangle & target, std::vector<rectangle*>* result, int need_count);
	void _knn_search(rectangle & target, std::vector<rectangle*>* result,
		int need_count, double &max_dist, std::vector<rectangle*>* overlap_node);
	void expand(rectangle & target, std::vector<rectangle*>* result, int need_count, double &max_dist);
	void get_leaf_nodes(std::vector<rectangle*>* res);
	// ����Euclidean distance
	double get_min_distance(rectangle & node);
	double get_max_distance(rectangle & node);
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
	double volume;
	//�������ĵ�����ԭ��ģ�ŷ�ϣ�����
	double midpoint_distance;
	// ��target����
	double cost;
	int visit_count;
};

void create_tree_from_file(std::string file_name);
void create_tree_from_file2(std::string file_name);
void create_tree_from_file3(std::string file_name);
