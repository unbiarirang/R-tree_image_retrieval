
#include <iostream>
#include "rectangle.h"
#include <fstream>
#include <sstream>
rectangle *root;

int main()
{
	char buffer[201];
    char temp[100];
	std::ifstream inputStream;
	char Filename[10]={"RGB"};
	int dim;
	std::cin>>dim;
	strcat(Filename,itoa(dim,temp,10));
	strcat(Filename,"D.txt");
	int ct=0;
	int j=0;

	std::vector<rectangle> v;
	v.push_back(rectangle(POINT));
	rectangle *IMG;
	std::vector<std::vector<int>> data;
	std::vector<int>* d = new std::vector<int>;
	inputStream.open(Filename);
		  if (!inputStream.is_open())
          { std::cout << "Error opening file"; exit (1); }
          while (!inputStream.eof())
          {		
			inputStream.getline(buffer,100);
			v.push_back(rectangle(POINT));
			for(int i=0;i<strlen(buffer);i++)
            {
                temp[j]=buffer[i];
                j++;
                if(temp[i+1]==':')
                {
                    strcpy(v[ct].name,temp);
                    memset(temp,NULL,sizeof(temp));
                    j=0;
                    i++;
                }
                if(temp[i+1]==' ')
                {
                    d->push_back(atoi(temp));
                    memset(temp,NULL,sizeof(temp));
                    j=0;
                    i++;
                }
            }			
				std::cout<<"1"<<std::endl;
				v[ct].point_data->data = d;
				std::cout<<"2"<<std::endl;
				v[ct].min_point = d;
				std::cout<<"3"<<std::endl;
				v[ct].max_point = d;
				d->erase(d->begin(),d->end());
				ct++;
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

	std::vector<rectangle*>* result=new std::vector<rectangle*>;
	root->knn_search(v[0], result, 2);
	//root->naive_search(*target, result);
	system("pause");
	return 0;
}