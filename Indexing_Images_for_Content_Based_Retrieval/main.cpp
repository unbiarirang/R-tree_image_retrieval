
#include <iostream>
#include "rectangle.h"

//记录R树的根
rectangle *root;
//记录所有图的数据
std::vector<rectangle*> image_data;

int main()
{
	root = new rectangle(RECTANGLE);
	//create_tree_from_file("color_feature.txt");
	//create_tree_from_file2("RGB4D.txt");
	create_tree_from_file3("RGB4D.txt");
	std::vector<rectangle*>* result = new std::vector<rectangle*>;

	rectangle* target = image_data[0];
	root->naive_search(*image_data[0], result, 1000);
	std::cout << target->visit_count << std::endl;
	root->knn_search(*target, result, 10);
	std::cout << target->visit_count << std::endl;

	//输出结果
	int correct_count = 0;
	std::string target_label = target->point_data->image_name.substr(0, target->point_data->image_name.find("_"));
	for (int i = 0; i < result->size(); i++)
	{
		if ((*result)[i]->point_data->image_name.substr(0, (*result)[i]->point_data->image_name.find("_")) == target_label)
			correct_count++;

		std::cout << (*result)[i]->point_data->ID << " " << (*result)[i]->point_data->image_name << " ";
		for (int j = 0; j < (*result)[i]->min_point->size(); j++)
		{
			std::cout << " " << (*(*result)[i]->min_point)[j];
		}
		std::cout << std::endl;
	}

	std::cout << "target image label: " << target_label << std::endl;
	std::cout << "准确率为" << double(correct_count) / double(result->size()) * 100 << "%\n";
	
	//std::vector<rectangle> v;
	//for (int i = 0; i < 1000; i++)
	//{
	//	v.push_back(rectangle(POINT));
	//	v[i].point_data = new point();
	//	(*v[i].point_data).ID = i + 1;
	//}
	//std::vector<std::vector<int>> data;
	//for (int i = 0; i < 1000; i++)
	//{
	//	std::vector<int>* d = new std::vector<int>;
	//	d->push_back(i + 1);
	//	d->push_back(i + 1);
	//	d->push_back(i + 1);
	//	data.push_back(*d);
	//	v[i].point_data->data = d;
	//	v[i].min_point = d;
	//	v[i].max_point = d;
	//}

	//root=new rectangle(RECTANGLE);
	//for (int i = 0; i < 1000; i++)
	//{
	//	(root->search_insert_position(v[i]))->insert(v[i]);
	//}
	////树构建完成

	//rectangle *target = new rectangle(RECTANGLE);
	//(*target->min_point)[0] = 20;
	//(*target->min_point)[1] = 20;
	//(*target->min_point)[2] = 20;
	//(*target->max_point)[0] = 30;
	//(*target->max_point)[1] = 30;
	//(*target->max_point)[2] = 30;

	//std::vector<rectangle*>* result = new std::vector<rectangle*>;
	////root->naive_search(*target, result);
	//root->knn_search(v[500], result, 10);

	system("pause");
	return 0;
}