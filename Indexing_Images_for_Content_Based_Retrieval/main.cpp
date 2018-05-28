
#include <iostream>
#include "rectangle.h"

//��¼R���ĸ�
rectangle *root;
//��¼����ͼ������
std::vector<rectangle*> image_data;
//�����㷨
split_method_type split_method;
//����R��ʱ�ڵ���ѵĴ��������ڲ���
int split_count = 0;

int main()
{
	root = new rectangle(RECTANGLE);
	/*ѡ��1������ɫ������*/
	//create_tree_from_file("color_feature.txt");
	/*ѡ��2����RGB����*/
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
		/*ѡ��1��naive search*/
		//root->naive_search(*image_data[num-1], result, 5000);
		/*ѡ��2��knn search*/
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

	std::cout << "ƽ��׼ȷ��Ϊ " << sum_accuracy / (float)query_count << "%\n";
	std::cout << "ƽ�����ʴ���Ϊ " << sum_visit_count / (float)query_count << "\n";
	std::cout << "����R��ʱ�ڵ���ѵĴ���Ϊ " << split_count << "\n";
	
	system("pause");
	return 0;
}