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

	//（m是偶数）分裂时，将这m+1个点按所有维度数据之和排序，最小的m/2和最大的m/2分别作为新的节点，中间一个插入父节点
	//刷新此矩形体积、坐标
	void renovate();
	//计算这个矩形如果要容纳另一个矩形的话，需要的扩张代价（此步骤依赖方形模型）
	int expand_cost(rectangle *new_rect);
	int expand_cost(rectangle *rect1, rectangle *rect2); // 覆盖三个矩形时的代价
	//查找新点应当插入的最底层中间节点
	rectangle* search_insert_position(rectangle &new_rect);
	//插入一个新的节点为此节点的子节点
	void insert(rectangle &new_rect);
	void insert_2(rectangle &new_rect);
	//简单的删除，不考虑额外情况，只用于insert内部调用
	void delete_child(int index);

	//点数据加载
	void init_point_data(std::string _image_path, std::string _image_name, std::vector<int>* _data);
	// 选用一个大矩形覆盖两/三个矩形时浪费掉的空间最大的两/三个矩形。n为所找seed个数
	std::vector<rectangle*> find_seed(rectangle & new_rect, int n);
	//矩形分裂
	void split(rectangle & new_rect);
	void split_2to3(rectangle & sibling, rectangle & new_rect);
	//（简单的）搜索
	void naive_search(rectangle & target, std::vector<rectangle*>* result);
	// knn搜索，搜索最近need_count个节点
	// max_dist用于expand函数递归时传值，对knn_search本身没用
	void knn_search(rectangle & target, std::vector<rectangle*>* result, int need_count, float &max_dist);
	void expand(rectangle & target, std::vector<rectangle*>* result, int need_count, float &max_dist);
	// 计算Euclidean distance
	float get_min_distance(rectangle & node);
	float get_max_distance(rectangle & node);
	//判断两个矩形是否重合
	bool overlap(rectangle & target);
	
	//方形种类（点或者矩形）
	rectangle_type type;
	//作为一个点的数据（仅当该矩形为一个点时，此项不为null）
	point* point_data;
	//颜色维数
	int color_count;
	//树中，该节点的父节点
	rectangle* parent;
	//树中，该节点的子节点（们）
	std::vector<rectangle*> child;
	//最小点的数据（相当于二维的矩形左下角）
	std::vector<int>* min_point;
	//最大点的数据（相当于二维的矩形右上角）
	std::vector<int>* max_point;
	//矩形体积（各维度数据之乘积）
	int volume;
};

