#include "rectangle.h"
#include "util.h"
#include <cmath>
#include <algorithm>

extern rectangle *root;

rectangle::rectangle(rectangle_type t)
{
	color_count = COLOR_COUNT;
	if (t == POINT)
	{
		type = POINT;
		point_data = new point();
		parent = nullptr;
		child.reserve(RECTANGLE_CAPABILITY + 1);
		min_point = new std::vector<int>;
		min_point->resize(color_count);
		max_point = min_point;
		volume = 0;
	}
	else if (t == RECTANGLE)
	{
		type = RECTANGLE;
		point_data = nullptr;
		parent = nullptr;
		child.reserve(RECTANGLE_CAPABILITY + 1);
		min_point = new std::vector<int>;
		min_point->resize(color_count);
		max_point = new std::vector<int>;
		max_point->resize(color_count);
		volume = 0;
	}
}

rectangle::~rectangle()
{

}

void rectangle::renovate()
{
	//ˢ��������εķ�Χ
	for (int i = 0; i < color_count; i++)
	{
		(*min_point)[i] = INT_MAX;
		(*max_point)[i] = 0;
	}
	for (int i = 0; i < child.size(); i++)
	{
		for (int m = 0; m < color_count; m++)
		{
			if ((*min_point)[m] >(*child[i]->min_point)[m])
			{
				(*min_point)[m] = (*child[i]->min_point)[m];
			}
			if ((*max_point)[m] <(*child[i]->max_point)[m])
			{
				(*max_point)[m] = (*child[i]->max_point)[m];
			}
		}
	}
	//ˢ��������ε����
	int count = 1;
	for (int i = 0; i < color_count; i++)
	{
		count *= (*max_point)[i] - (*min_point)[i];
	}
	volume = count;

}

float rectangle::expand_cost(rectangle *new_rect)
{
	float min;
	float max;
	float cost = 1;
	for (int i = 0; i < color_count; i++)
	{
		if ((*min_point)[i] > (*new_rect->min_point)[i])
			min = (*new_rect->min_point)[i];
		else
			min = (*min_point)[i];
		if ((*max_point)[i] < (*new_rect->max_point)[i])
			max = (*new_rect->max_point)[i];
		else
			max = (*max_point)[i];
		cost *= (max - min);
	}
	return cost - volume - new_rect->volume;
}

float rectangle::expand_cost(rectangle *rect1, rectangle *rect2)
{
	float min;
	float max;
	float cost = 1;
	for (int i = 0; i < color_count; i++) {
		// ��������������С��min_pointֵ
		if ((*min_point)[i] < (*rect1->min_point)[i])
			min = (*min_point)[i];
		else
			min = (*rect1->min_point)[i];
		if ((*rect2->min_point)[i] < min)
			min = (*rect2->min_point)[i];

		// ����������������max_pointֵ
		if ((*min_point)[i] > (*rect1->min_point)[i])
			max = (*min_point)[i];
		else
			max = (*rect1->min_point)[i];
		if ((*rect2->min_point)[i] > max)
			max = (*rect2->min_point)[i];

		cost *= (max - min);
	}
	return cost - volume - rect1->volume - rect2->volume;
}


rectangle * rectangle::search_insert_position(rectangle &new_rect)
{
	rectangle* result = this;
	if (child.size() == 0 || (child.size() != 0 && child[0]->type == POINT))
	{
		//����ǵײ����
		return this;
	}
	else
	{
		//����һ���ǿշǵײ���Σ��������ӽڵ���Ѱ�Ҳ���Ľڵ�
		float min_cost = FLT_MAX;
		float temp;
		for (int i = 0; i < child.size(); i++)
		{
			temp = child[i]->expand_cost(&new_rect);
			if (min_cost > temp)
			{
				min_cost = temp;
				result = child[i];
			}
		}
		return result->search_insert_position(new_rect);
	}
}

void rectangle::insert(rectangle &new_rect)
{
	if (type == RECTANGLE)
	{
		if (child.size() < RECTANGLE_CAPABILITY)
		{
			//�������
			child.push_back(&new_rect);
			new_rect.parent = this;
			renovate();
			rectangle *refresh_renovate = parent;
			while (refresh_renovate != nullptr)
			{
				refresh_renovate->renovate();
				refresh_renovate = refresh_renovate->parent;
			}
		}
		else
		{
			//��������������Ҫ����
			int index[RECTANGLE_CAPABILITY + 1];
			int num[RECTANGLE_CAPABILITY + 1];
			for (int i = 0; i < RECTANGLE_CAPABILITY; i++)
			{
				num[i] = child[i]->volume;
				index[i] = i;
				//Ȼ������ǰ��RECTANGLE_CAPABILITY + 1��/2��һ�ѣ�ʣ�µ�newһ�ѣ�����һ�ѵ���parent->insert(new����
				//�������ǵÿ��Ǹ��ڵ�����������
			}
			num[RECTANGLE_CAPABILITY] = new_rect.volume;
			index[RECTANGLE_CAPABILITY] = RECTANGLE_CAPABILITY;
			//��num��������,��index������
			unsigned int temp1 = 0;
			int temp2 = 0;
			int j;
			for (int i = 1; i < RECTANGLE_CAPABILITY + 1; i++)
			{
				temp1 = num[i];//�Ӵ�������ȡ����һ��Ԫ�ء� 
				temp2 = index[i];
				j = i - 1; //i-1��Ϊ���������һ��Ԫ�أ��������Ԫ�����ڣ����±�   
				while (j >= 0 && temp1 < num[j])  //�ж�����Ϊ������j>=0������б߽����ơ��ڶ���Ϊ�����ж�����
				{
					num[j + 1] = num[j];//�����Ǻ���λ�ã�������Ԫ������ƶ�
					index[j + 1] = index[j];
					j--;
				}
				num[j + 1] = temp1;//�ҵ�����λ�ã���Ԫ�ز��롣
				index[j + 1] = temp2;
			}
			for (int i = (RECTANGLE_CAPABILITY + 1) / 2; i < RECTANGLE_CAPABILITY + 1; i++)
			{
				temp2 = index[i];//�Ӵ�������ȡ����һ��Ԫ�ء� 
				j = i - 1; //i-1��Ϊ���������һ��Ԫ�أ��������Ԫ�����ڣ����±�   
				while (j >= (RECTANGLE_CAPABILITY + 1) / 2 && temp2 < index[j])  //�ж�����Ϊ������j>=0������б߽����ơ��ڶ���Ϊ�����ж�����
				{
					index[j + 1] = index[j];//�����Ǻ���λ�ã�������Ԫ������ƶ�
					j--;
				}
				index[j + 1] = temp2;//�ҵ�����λ�ã���Ԫ�ز��롣
			}
			//�������
			//���һ�½������һ��
			std::cout << "�����ڵ���ѣ���ʱ��Ӧ���ǣ�ǰһ����size�����������С��Ԫ�أ���һ����index������Ԫ�أ�size���������Ϊ��" << std::endl;
			for (int i = 0; i < RECTANGLE_CAPABILITY + 1; i++)
			{
				std::cout << num[i] << " " ;
			}
			std::cout << std::endl << "index���������Ϊ��";
			for (int i = 0; i < RECTANGLE_CAPABILITY + 1; i++)
			{
				std::cout << index[i] << " ";
			}
			std::cout << std::endl;
			//��ʱ��Ӧ���ǣ�ǰһ����size�����ҽ�С��Ԫ�أ���һ����index������Ԫ��
			rectangle *temp_rect = new rectangle(RECTANGLE);
			for (int i = RECTANGLE_CAPABILITY; i >= (RECTANGLE_CAPABILITY + 1) / 2; i--)
			{
				if (index[i] != RECTANGLE_CAPABILITY)
				{
					//��ԭ�ڵ����е�Ԫ��
					temp_rect->insert(*(child[index[i]]));
					//��ԭ�ڵ�ɾȥ
					delete_child(index[i]);
				}
				else
				{
					//�Ǹռӵ���Ԫ��
					temp_rect->insert(new_rect);
				}
			}
			renovate();
			temp_rect->renovate();
			//�½ڵ㴴����ϣ���������Լ��ĸ��ڵ㣬ʹ֮���Լ���λ��ͬ
			//����������
			if (parent != nullptr)
			{
				//�ⲻ�Ǹ��ڵ�
				temp_rect->parent = parent;
				parent->insert(*temp_rect);
			}
			else
			{
				//���ڵ���ѳ���2���������Ǹ��ڵ�
				rectangle *new_root = new rectangle(RECTANGLE);
				new_root->child.push_back(temp_rect);
				temp_rect->parent = new_root;
				new_root->child.push_back(this);
				parent = new_root;
				new_root->renovate();
				root = new_root;
				std::cout << "�����������ߣ����ڵ�ı䣡" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "���Խ��ڵ����Ϊ����ӽڵ㣡" << std::endl;
	}
}

void rectangle::insert_2(rectangle & new_rect)
{
	if (type == RECTANGLE)
	{
		if (child.size() < RECTANGLE_CAPABILITY)
		{
			//�������
			child.push_back(&new_rect);
			new_rect.parent = this;
			renovate();
		}
		else
		{
			//��������������Ҫ����
			split(new_rect);
		}
	}
	else
		std::cout << "���Խ��ڵ����Ϊ����ӽڵ㣡" << std::endl;
}

void rectangle::delete_child(int index)
{
	child.erase(child.begin() + index);
}

void rectangle::init_point_data(std::string _image_path, std::string _image_name, std::vector<int>* _data)
{
	point_data->color_count = color_count;
	point_data->image_path = _image_path;
	point_data->image_name = _image_name;
	point_data->data = _data;
}

// ѡ��һ������θ�����������ʱ�˷ѵ��Ŀռ�������������
std::vector<rectangle*> rectangle::find_seed(rectangle &new_rect)
{
	float max_waste_volume = 0;
	float v = 0;

	std::vector<rectangle*> seed;
	seed.resize(2);

	for (int i = 0; i < child.size(); i++) {
		for (int j = i + 1; j < child.size(); j++) {
			v = child[i]->expand_cost(child[j]);
			if (v > max_waste_volume) {
				v = max_waste_volume;
				seed[0] = child[i];
				seed[1] = child[j];
			}
		}

		// ��new_rectҲ���бȽ�
		v = child[i]->expand_cost(&new_rect);
		if (v > max_waste_volume) {
			v = max_waste_volume;
			seed[0] = child[i];
			seed[1] = &new_rect;
		}
	}

	return seed;
}

// ѡ���������Ӿ���
std::vector<rectangle*> rectangle::find_seed(rectangle & sibling, rectangle & new_rect) {
	float max_waste_volume = 0;
	float v = 0;

	std::vector<rectangle*> seed;
	seed.resize(3);

	// ��Ҫ���ѵ�����������ĺ��ӽڵ���¼ӵĽڵ����nodes����
	std::vector<rectangle*> nodes;
	for (int i = 0; i < this->child.size(); i++)
		nodes.push_back(this->child[i]);
	for (int i = 0; i < sibling.child.size(); i++)
		nodes.push_back(sibling.child[i]);
	nodes.push_back(&new_rect);

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = i + 1; j < nodes.size(); j++) {
			for (int k = j + 1; k < nodes.size(); k++) {
				v = nodes[i]->expand_cost(nodes[j], nodes[k]);
				if (v > max_waste_volume) {
					v = max_waste_volume;
					seed[0] = nodes[i];
					seed[1] = nodes[j];
					seed[2] = nodes[k];
				}
			}

			// ��new_rectҲ���бȽ�
			v = nodes[i]->expand_cost(nodes[j], &new_rect);
			if (v > max_waste_volume) {
				v = max_waste_volume;
				seed[0] = nodes[i];
				seed[1] = nodes[j];
				seed[2] = &new_rect;
			}
		}
	}
}

void rectangle::split(rectangle & new_rect)
{
	// ��new_rect��indexΪchild.size()��seed[1]_index���ܵ���child.size
	std::vector<rectangle*> seed = find_seed(new_rect);

	// ���εĽڵ���¼ӵĽڵ�ϲ�����2���µľ��Σ����ӽڵ�ֱ��ݴ���r1��r2
	std::vector<rectangle*> r1, r2;
	int total_node_num = RECTANGLE_CAPABILITY + 1;
	float cost1, cost2;
	int max_r1_size = total_node_num / 2, max_r2_size= total_node_num - max_r1_size;
	
	// ��һ�����εĽڵ���¼ӵĽڵ���䵽������
	// �Ƚ��¼ӵĵ����
	if (&new_rect != seed[1]) { //  �¼ӵĵ㲻��seed
		cost1 = new_rect.expand_cost(seed[0]);
		cost2 = new_rect.expand_cost(seed[1]);

		// �ֵ�ǰ�벿�ֻ��벿��
		if (cost1 < cost2)
			r1.push_back(&new_rect);
		else
			r2.push_back(&new_rect);
	}
	else // �¼ӵĵ���seed
		r2.push_back(&new_rect);

	// ������������ĵ����
	for (int i = 0; i < child.size(); i++) {
		if (child[i] == seed[0]) {
			r1.push_back(child[i]); continue;
		}
		if (child[i] == seed[1]) {
			r2.push_back(child[i]); continue;
		}

		// һ���������,�Զ��ֵ���һ����
		if (r1.size() >= max_r1_size) {
			r2.push_back(child[i]); continue;
		}
		if (r2.size() >= max_r2_size) {
			r1.push_back(child[i]); continue;
		}

		cost1 = child[i]->expand_cost(seed[0]);
		cost2 = child[i]->expand_cost(seed[1]);

		// �ֳ�ǰ�벿�ֺͺ�벿��
		if (cost1 < cost2)
			r1.push_back(child[i]);
		else
			r2.push_back(child[i]);
	}
	child.clear();


	// FIXME - create 2 rectangles with 2 rectangle vector
	rectangle *split_rect = new rectangle(RECTANGLE);
	for (int i = 0; i < r1.size(); i++)
		this->insert_2(*r1[i]);
	for (int i = 0; i < r2.size(); i++)
		split_rect->insert_2(*r2[i]);

	renovate();
	split_rect->renovate();
	split_rect->parent = parent;
	parent->insert_2(*split_rect);
}

void rectangle::split_2to3(rectangle & sibling, rectangle & new_rect)
{
	std::vector<rectangle*> seed = find_seed(sibling, new_rect);

	// 2�����εĽڵ���¼ӵĽڵ�ϲ�����3���µľ��Σ����ӽڵ�ֱ��ݴ���r1��r2��r3
	std::vector<rectangle*> r1, r2, r3;
	int total_node_num = 2 * RECTANGLE_CAPABILITY + 1;
	float cost1, cost2, cost3;
	int max_r1_size = total_node_num / 3, 
		max_r2_size = (total_node_num - max_r1_size) / 2,
		max_r3_size = total_node_num - max_r1_size - max_r2_size;
	int index; // ��Сcost�ı��

	// ��2�����εĽڵ���¼ӵĽڵ���䵽3����
	// �Ƚ��¼ӵĵ����
	if (&new_rect != seed[2]) { // �¼ӵĵ㲻��seed
		cost1 = new_rect.expand_cost(seed[0]);
		cost2 = new_rect.expand_cost(seed[1]);
		cost3 = new_rect.expand_cost(seed[2]);

		// �ֵ�ǰ���У���������֮һ
		index = min_index_of_three(cost1, cost2, cost3);
		if (index == 0)
			r1.push_back(&new_rect);
		else if (index == 1)
			r2.push_back(&new_rect);
		else
			r3.push_back(&new_rect);
	}
	else // �¼ӵĵ���seed
		r3.push_back(&new_rect);

	for (int i = 0; i < total_node_num - 1; i++) {
		// �Լ�������seed
		if (child[i] == seed[0]) {
			r1.push_back(child[i]); continue;
		}
		if (child[i] == seed[1]) {
			r2.push_back(child[i]); continue;
		}
		if (child[i] == seed[2]) {
			r3.push_back(child[i]); continue;
		}

		// �����������
		cost1 = child[i]->expand_cost(seed[0]);
		cost2 = child[i]->expand_cost(seed[1]);
		cost3 = child[i]->expand_cost(seed[2]);

		if (r1.size() >= max_r1_size) // ��һ���������
			cost1 = FLT_MAX;
		if (r2.size() >= max_r2_size) // �ڶ����������
			cost2 = FLT_MAX;
		if (r3.size() >= max_r3_size) // �������������
			cost3 = FLT_MAX;

		// �ֳ�������
		index = min_index_of_three(cost1, cost2, cost3);
		if (index == 0)
			r1.push_back(child[i]);
		else if (index == 1)
			r2.push_back(child[i]);
		else
			r3.push_back(child[i]);
	}
	child.clear();

	// FIXME - create 3 rectangles with 3 rectangle vector
	rectangle *split_rect = new rectangle(RECTANGLE);
	for (int i = 0; i < r1.size(); i++)
		this->insert_2(*r1[i]);
	for (int i = 0; i < r2.size(); i++)
		sibling.insert_2(*r2[i]);
	for (int i = 0; i < r3.size(); i++)
		split_rect->insert_2(*r3[i]);

	renovate();
	split_rect->renovate();
	split_rect->parent = parent;
	parent->insert(*split_rect);
}

void rectangle::naive_search(rectangle & target, std::vector<rectangle*>* result)
{
	for (int i = 0; i < child.size(); i++)
	{
		if (child[i]->overlap(target))
		{
			if (child[i]->type == POINT)
			{
				result->push_back(child[i]);
			}
			else if (child[i]->type == RECTANGLE)
			{
				child[i]->naive_search(target, result);
			}
		}
	}
}

float rectangle::get_min_distance(rectangle & node) {
	float dist = 0;
	for (int i = 0; i < COLOR_COUNT; i++) {
		if ((*node.min_point)[i] >= (*min_point)[i] && (*node.min_point)[i] <= (*max_point)[i])
			dist += 0;
		else
			dist += std::min(pow((*node.min_point)[i] - (*min_point)[i], 2), pow((*node.min_point)[i] - (*max_point)[i], 2));
	}

	return sqrt(dist);
}

// ��Զ�Խ��߾��롣������������Զ���룬��������Զ�����
float rectangle::get_max_distance(rectangle & node) {
	float dist = 0;
	for (int i = 0; i < COLOR_COUNT; i++)
		dist += std::min(abs((*node.min_point)[i] - (*min_point)[i]), abs((*node.min_point)[i] - (*max_point)[i]));

	return sqrt(dist);
}

void rectangle::knn_search(rectangle & target, std::vector<rectangle*>* result, int need_count) {
	float max_dist = 0;
	std::vector<rectangle*>* overlap_node = new std::vector<rectangle*>;
	_knn_search(target, result, need_count, max_dist, overlap_node);

	for (int i = 0; i < overlap_node->size(); i++)
		(*overlap_node)[i]->expand(target, result, need_count, max_dist);

	while (result->size() < need_count) {
		for (int i = 0; i < overlap_node->size(); i++) {
			(*overlap_node)[i] = (*overlap_node)[i]->parent;
			max_dist *= 2;
			(*overlap_node)[i]->expand(target, result, need_count, max_dist);
		}
	}
}

// knn����ʱtarget��һ����
void rectangle::_knn_search(rectangle & target, std::vector<rectangle*>* result,
	int need_count, float &max_dist, std::vector<rectangle*>* overlap_node)
{
	float min_worst = FLT_MAX, temp_worst = 0;
	float *best_dist = new float[child.size()];
	// ���ڳ���Ҷ�ӽ��ʱ
	bool leaf_flag = false;

	for (int i = 0; i < child.size(); i++) {
		if (child[i]->type == RECTANGLE) { // ���ӽڵ��Ǿ���
			temp_worst = child[i]->get_max_distance(target);
			best_dist[i] = child[i]->get_min_distance(target);

			if (temp_worst < min_worst)
				min_worst = temp_worst;
		}
		else { // ���ӽڵ���Ҷ�ڵ�
			result->push_back(child[i]);
			leaf_flag = true;

			temp_worst = child[i]->get_max_distance(target); // �����ڵ�֮��ľ���
			if (temp_worst > max_dist)
				max_dist = temp_worst;
		}
	}

	// ��֦ ���ӽڵ��Ǿ���
	if (!leaf_flag && overlap_node) {
		for (int i = 0; i < child.size(); i++) {
			if (best_dist[i] > min_worst) // ����ľ��붼����С�ľ���󣬲�����
				continue;
			else
				child[i]->_knn_search(target, result, need_count, max_dist, overlap_node);	// ��������
		}
	}

	// �����������ʱ��������Щ�������䣨��expand������
	if (leaf_flag && overlap(target) && overlap_node)
		overlap_node->push_back(this);

	delete[]best_dist;
}

void rectangle::expand(rectangle & target, std::vector<rectangle*>* result, int need_count, float &max_dist)
{
	rectangle* parent = this->parent;
	// Ϊ���ҳ�����target�غϣ����Ǳ��ڽ������Զ�Ľڵ���������е�
	// �ѽڵ�targetչ���������Σ���ȨֵΪ2max_dist���������������غϵľ���
	rectangle* new_rect = new rectangle(RECTANGLE);
	for (int i = 0; i < COLOR_COUNT; i++) {
		(*new_rect->min_point)[i] = (*target.min_point)[i] - max_dist; // FIXME ���겻������
		(*new_rect->max_point)[i] = (*target.min_point)[i] + max_dist;
	}
	for (int i = 0; i < parent->child.size(); i++) {
		// ���ڵ�target���غϵ���������new_rect�غ�
		// ������ڵ�target�غϵĻ��϶���������������������ظ�
		//if (!parent->child[i]->overlap(target) && parent->child[i]->overlap(*new_rect)) // FIXME
		if (parent->child[i]->overlap(*new_rect))
			parent->child[i]->_knn_search(target, result, need_count, max_dist, nullptr); // ������������ľ��β�������Ϊ��������
	}
}

bool rectangle::overlap(rectangle & target)
{
	for (int i = 0; i < COLOR_COUNT; i++)
	{
		if ((*min_point)[i] > (*target.max_point)[i] || (*max_point)[i] < (*target.min_point)[i])
		{
			return false;
		}
	}
	return true;
}
