
#include <iostream>
#include "rectangle.h"

//记录R树的根
rectangle *root;
//记录所有图的数据
std::vector<rectangle*> image_data;
//分裂算法
split_method_type split_method;
//建立R树时节点分裂的次数，用于测试
int split_count = 0;

int main()
{
	root = new rectangle(RECTANGLE);
	/*选择1：读颜色矩特征*/
	//create_tree_from_file("color_feature.txt");
	/*选择2：读RGB特征*/
	create_tree_from_file2("RGB16D.txt");

	int query_count = 0;
	int num;
	std::cout << "Number of images to saerch? \n";
	std::cin >> query_count;
	std::cout << "ID of the first image? \n";
	std::cin >> num;

	int temp;
	std::cout << "What kind of split method ? \n1.Naive split 2.Guttman's quadratic split 3.Guttman's quadratic split improved 4.2to3 split\n";
	std::cin >> temp;
	split_method = split_method_type(temp);

	std::vector<rectangle*>* result = new std::vector<rectangle*>;
	std::vector<float> accuracy;
	std::vector<int> visit_counts; // efficiency
	std::string target_label;
	rectangle* target;

	int correct_count = 0;

	for (int i = 0; i < query_count; i++) {
		target = image_data[num - 1];
		/*选择1：naive search*/
		//root->naive_search(*image_data[num-1], result, 5000);
		/*选择2：knn search*/
		root->knn_search(*target, result, 10);

		target_label = target->point_data->image_name.substr(0, target->point_data->image_name.find("_"));
		for (int i = 0; i < result->size(); i++) {
			if ((*result)[i]->point_data->image_name.substr(0, (*result)[i]->point_data->image_name.find("_")) == target_label)
				correct_count++;
		}

		accuracy.push_back(float(correct_count) / float(result->size()) * 100);
		visit_counts.push_back(target->visit_count);

		result->clear();
		correct_count = 0;
		num += 1;
	}

	float sum_accuracy = 0;
	float sum_visit_count = 0;
	for (int i = 0; i < query_count; i++) {
		sum_accuracy += accuracy[i];
		sum_visit_count += visit_counts[i];
	}

	std::cout << "平均准确率为 " << sum_accuracy / (float)query_count << "%\n";
	std::cout << "平均访问次数为 " << sum_visit_count / (float)query_count << "\n";
	std::cout << "建立R树时节点分裂的次数为 " << split_count << "\n";
	
	system("pause");
	return 0;
}