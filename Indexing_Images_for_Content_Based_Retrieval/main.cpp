
#include <iostream>
#include "rectangle.h"

//记录R树的根
rectangle *root;
//记录所有图的数据
std::vector<rectangle*> image_data;

int main()
{
	root = new rectangle(RECTANGLE);
	create_tree_from_file("color_feature.txt");
	std::vector<rectangle*>* result = new std::vector<rectangle*>;

	root->naive_search(*image_data[0], result, 1000);

	//输出结果
	for (int i = 0; i < result->size(); i++)
	{
		std::cout << (*result)[i]->point_data->ID;
		for (int j = 0; j < (*result)[i]->min_point->size(); j++)
		{
			std::cout << " " << (*(*result)[i]->min_point)[j];
		}
		std::cout << std::endl;
	}

	/*
	std::vector<rectangle> v;
	for (int i = 0; i < 100; i++)
	{
	v.push_back(rectangle(POINT));
	}
	std::vector<std::vector<int>> data;
	for (int i = 0; i < 100; i++)
	{
	std::vector<int>* d = new std::vector<int>;
	d->push_back(i + 1);
	d->push_back(i + 1);
	d->push_back(i + 1);
	data.push_back(*d);
	v[i].point_data->data = d;
	v[i].min_point = d;
	v[i].max_point = d;
	}

	root=new rectangle(RECTANGLE);
	rectangle *temp;
	for (int i = 0; i < 100; i++)
	{
	(root->search_insert_position(v[i]))->insert(v[i]);
	}
	//树构建完成

	rectangle *target = new rectangle(RECTANGLE);
	(*target->min_point)[0] = 20;
	(*target->min_point)[1] = 20;
	(*target->min_point)[2] = 20;
	(*target->max_point)[0] = 30;
	(*target->max_point)[1] = 30;
	(*target->max_point)[2] = 30;

	std::vector<rectangle*>* result = new std::vector<rectangle*>;
	root->naive_search(*target, result);
	*/

	system("pause");
	return 0;
}