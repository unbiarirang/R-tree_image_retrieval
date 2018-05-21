
#include <iostream>
#include "rectangle.h"

//记录R树的根
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
	rectangle *temp;
	for (int i = 0; i < 100; i++)
	{
		(root->search_insert_position(v[i]))->insert(v[i]);
	}
	//树构建完成
	system("pause");
	return 0;
}