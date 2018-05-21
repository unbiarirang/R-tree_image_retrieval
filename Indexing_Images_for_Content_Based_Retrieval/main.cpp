
#include <iostream>
#include "rectangle.h"

rectangle *root;

int main()
{
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
	for (int i = 0; i < 100; i++)
	{
		(root->search_insert_position(v[i]))->insert(v[i]);
	}

	rectangle *target = new rectangle(RECTANGLE);
	(*target->min_point)[0] = 20;
	(*target->min_point)[1] = 20;
	(*target->min_point)[2] = 20;
	(*target->max_point)[0] = 30;
	(*target->max_point)[1] = 30;
	(*target->max_point)[2] = 30;

	std::vector<rectangle*>* result=new std::vector<rectangle*>;
	root->knn_search(v[0], result, 7);
	//root->naive_search(*target, result);
	system("pause");
	return 0;
}