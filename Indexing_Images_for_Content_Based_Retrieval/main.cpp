#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "rectangle.h"
#include <fstream>
#include <sstream>
rectangle *root;

int main()
{
	char buffer[201] = { 0 };
	char temp[100] = { 0 };
	char Filename[10]={"RGB"};
	std::ifstream inputStream;

	int dim;
	std::cin >> dim;
	strcat(Filename,_itoa(dim,temp,10));
	strcat(Filename,"D.txt");
	inputStream.open(Filename);

	int j=0;
	std::vector<rectangle> image_points;
	if (!inputStream.is_open())
		{ std::cout << "Error opening file"; exit (1); }
    while (!inputStream.eof()) {		
		inputStream.getline(buffer, 100);
		rectangle new_image(POINT);
		point *p = new point;
		std::vector<int>* features = new std::vector<int>;
		for(int i = 0; i < strlen(buffer); i++) {
		    temp[j]=buffer[i];
		    j++;
		    if (buffer[i + 1] == ':') {
				strcpy(p->image_name, temp);
		        memset(temp,NULL,sizeof(temp));
		        j=0;
				i++;
				continue;
			}
		    if(buffer[i + 1]==' ') {
				features->push_back(atoi(temp));
		        memset(temp,NULL,sizeof(temp));
		        j=0;
				i++;
				continue;
			}
		}		
		p->data = features;
		new_image.point_data = p;
		image_points.push_back(new_image);
	}
      
	
	/*for (int i = 0; i < 100; i++)
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
	*/
	//root=new rectangle(RECTANGLE);
	//for (int i = 0; i < 100; i++)
	//{
	//	(root->search_insert_position(v[i]))->insert(v[i]);
	//}

	//rectangle *target = new rectangle(RECTANGLE);
	//(*target->min_point)[0] = 20;
	//(*target->min_point)[1] = 20;
	//(*target->min_point)[2] = 20;
	//(*target->max_point)[0] = 30;
	//(*target->max_point)[1] = 30;
	//(*target->max_point)[2] = 30;

	//std::vector<rectangle*>* result=new std::vector<rectangle*>;
	//root->knn_search(v[0], result, 2);
	//root->naive_search(*target, result);
	system("pause");
	return 0;
}