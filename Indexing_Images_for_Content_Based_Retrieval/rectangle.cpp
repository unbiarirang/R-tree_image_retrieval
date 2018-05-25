#include "rectangle.h"
#include "util.h"

extern rectangle *root;
extern std::vector<rectangle*> image_data;

rectangle::rectangle(rectangle_type t, std::vector<int> * min_point_data, std::vector<int> * max_point_data)
{
	color_count = COLOR_COUNT;
	if (t == POINT)
	{
		type = POINT;
		point_data = new point();
		parent = nullptr;
		child.reserve(RECTANGLE_CAPABILITY + 1);
		if (min_point_data != nullptr)
		{
			min_point = min_point_data;
			max_point = min_point_data;
			point_data->data = min_point_data;
			renovate();
		}
		else
		{
			min_point = new std::vector<int>;
			min_point->resize(color_count);
			max_point = min_point;
		}
		volume = 0;
	}
	else if (t == RECTANGLE)
	{
		type = RECTANGLE;
		point_data = nullptr;
		parent = nullptr;
		child.reserve(RECTANGLE_CAPABILITY + 1);
		if (min_point_data != nullptr && max_point_data != nullptr)
		{
			min_point = min_point_data;
			max_point = max_point_data;
			renovate();
		}
		else
		{
			min_point = new std::vector<int>;
			min_point->resize(color_count);
			max_point = new std::vector<int>;
			max_point->resize(color_count);
		}
		volume = 0;
	}
}

rectangle::~rectangle()
{

}

void rectangle::renovate()
{
	double count = 1;
	if (type == RECTANGLE && child.size() != 0)
	{
		//根据这个矩形的孩子，刷新这个矩形的范围
		for (int i = 0; i < color_count; i++)
		{
			(*min_point)[i] = 2147483647;
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
				if ((*max_point)[m] < (*child[i]->max_point)[m])
				{
					(*max_point)[m] = (*child[i]->max_point)[m];
				}
			}
		}
		//刷新这个矩形的体积
		count = 1;
		for (int i = 0; i < color_count; i++)
		{
			count *= (*max_point)[i] - (*min_point)[i];
		}
		volume = count;
		count = 1;
		for (int i = 0; i < color_count; i++)
		{
			count += (((*max_point)[i] + (*min_point)[i]) / 2)*(((*max_point)[i] + (*min_point)[i]) / 2);
		}
		midpoint_distance = sqrt(count);
	}
	else
	{
		count = 1;
		for (int i = 0; i < color_count; i++)
		{
			count += (((*max_point)[i] + (*min_point)[i]) / 2)*(((*max_point)[i] + (*min_point)[i]) / 2);
		}
		midpoint_distance = sqrt(count);
	}
}

double rectangle::expand_cost(rectangle *new_rect)
{
	if (new_rect == nullptr) {
		return 0;
	}
	double min;
	double max;
	double cost = 1;
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

double rectangle::expand_cost(rectangle *rect1, rectangle *rect2)
{
	double min;
	double max;
	double cost = 1;
	for (int i = 0; i < color_count; i++) {
		// 找三个矩形中最小的min_point值
		if ((*min_point)[i] < (*rect1->min_point)[i])
			min = (*min_point)[i];
		else
			min = (*rect1->min_point)[i];
		if ((*rect2->min_point)[i] < min)
			min = (*rect2->min_point)[i];

		// 找三个矩形中最大的max_point值
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
		//这就是底层矩形
		return this;
	}
	else
	{
		//这是一个非空非底层矩形，将在其子节点中寻找插入的节点
		double min_cost = FLT_MAX;
		double temp;
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
			//不需分裂
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
			//split(new_rect);
			split_quadratic(new_rect);
			//split_2to3(new_rect);
		}
	}
	else
	{
		std::cout << "尝试将节点插入为点的子节点！" << std::endl;
	}
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

// 选用一个大矩形覆盖两个矩形时浪费掉的空间最大的两个矩形
std::vector<rectangle*> rectangle::find_seed(rectangle* new_rect)
{
	double max_waste_volume = -1;
	double v = 0;

	std::vector<rectangle*> seed;
	seed.resize(2);

	for (int i = 0; i < child.size(); i++) {
		for (int j = i + 1; j < child.size(); j++) {
			v = child[i]->expand_cost(child[j]);
			if (v > max_waste_volume) {
				max_waste_volume = v;
				seed[0] = child[i];
				seed[1] = child[j];
			}
		}

		// 跟new_rect也进行比较
		v = child[i]->expand_cost(new_rect);
		if (v > max_waste_volume) {
			max_waste_volume = v;
			seed[0] = child[i];
			seed[1] = new_rect;
		}
	}
	if (seed[0] == nullptr)
	{
		std::cout << "???";
		for (int i = 0; i < child.size(); i++) {
			for (int j = i + 1; j < child.size(); j++) {
				v = child[i]->expand_cost(child[j]);
				if (v > max_waste_volume) {
					max_waste_volume = v;
					seed[0] = child[i];
					seed[1] = child[j];
				}
			}

			// 跟new_rect也进行比较
			v = child[i]->expand_cost(new_rect);
			if (v > max_waste_volume) {
				max_waste_volume = v;
				seed[0] = child[i];
				seed[1] = new_rect;
			}
		}

	}
	return seed;
}

// 选出三个种子矩形
std::vector<rectangle*> rectangle::find_seed(rectangle* sibling, rectangle* new_rect) {
	double max_waste_volume = -1;
	double v = 0;

	std::vector<rectangle*> seed;
	seed.resize(3);

	// 把要分裂的两个矩形里的孩子节点和新加的节点插入nodes向量
	std::vector<rectangle*> nodes;
	for (int i = 0; i < this->child.size(); i++)
		nodes.push_back(this->child[i]);
	for (int i = 0; i < sibling->child.size(); i++)
		nodes.push_back(sibling->child[i]);
	nodes.push_back(new_rect);

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = i + 1; j < nodes.size(); j++) {
			for (int k = j + 1; k < nodes.size(); k++) {
				v = nodes[i]->expand_cost(nodes[j], nodes[k]);
				if (v > max_waste_volume) {
					max_waste_volume = v;
					seed[0] = nodes[i];
					seed[1] = nodes[j];
					seed[2] = nodes[k];
				}
			}

			// 跟new_rect也进行比较
			v = nodes[i]->expand_cost(nodes[j], new_rect);
			if (v > max_waste_volume) {
				max_waste_volume = v;
				seed[0] = nodes[i];
				seed[1] = nodes[j];
				seed[2] = new_rect;
			}
		}
	}
	if (seed[0] == nullptr)
	{
		std::cout << "???";
	}
	return seed;
}

rectangle * rectangle::find_sibling(int & sibling_count,bool & use)
{
	rectangle * result = nullptr;
	if (parent != nullptr)
		sibling_count = parent->child.size();
	else
		sibling_count = -1;
	if (sibling_count == 2)
	{
		if (parent->child[0]->child.size() == RECTANGLE_CAPABILITY && parent->child[1]->child.size() == RECTANGLE_CAPABILITY)
			use = true;
		else
			use = false;
	}
	for (int i = 0; i < parent->child.size(); i++)
	{
		if (parent->child[i]->child.size() < RECTANGLE_CAPABILITY);
		{
			return parent->child[i];
		}
	}
}

void rectangle::split(rectangle & new_rect)
{
	//本矩形已满，需要分裂
	int index[RECTANGLE_CAPABILITY + 1];
	double num[RECTANGLE_CAPABILITY + 1];
	for (int i = 0; i < RECTANGLE_CAPABILITY; i++)
	{
		num[i] = child[i]->midpoint_distance;
		index[i] = i;
		//然后排序，前（RECTANGLE_CAPABILITY + 1）/2放一堆，剩下的new一堆，把那一堆调用parent->insert(new）；
		//记得考虑根节点的特殊情况
	}
	num[RECTANGLE_CAPABILITY] = new_rect.midpoint_distance;
	index[RECTANGLE_CAPABILITY] = RECTANGLE_CAPABILITY;
	//对num进行排序,由index记忆结果
	unsigned int temp1 = 0;
	int temp2 = 0;
	int j;
	for (int i = 1; i < RECTANGLE_CAPABILITY + 1; i++)
	{
		temp1 = num[i];//从待插入组取出第一个元素。 
		temp2 = index[i];
		j = i - 1; //i-1即为有序组最后一个元素（与待插入元素相邻）的下标   
		while (j >= 0 && temp1 < num[j])  //判断条件为两个，j>=0对其进行边界限制。第二个为插入判断条件
		{
			num[j + 1] = num[j];//若不是合适位置，有序组元素向后移动
			index[j + 1] = index[j];
			j--;
		}
		num[j + 1] = temp1;//找到合适位置，将元素插入。
		index[j + 1] = temp2;
	}
	for (int i = (RECTANGLE_CAPABILITY + 1) / 2; i < RECTANGLE_CAPABILITY + 1; i++)
	{
		temp2 = index[i];//从待插入组取出第一个元素。 
		j = i - 1; //i-1即为有序组最后一个元素（与待插入元素相邻）的下标   
		while (j >= (RECTANGLE_CAPABILITY + 1) / 2 && temp2 < index[j])  //判断条件为两个，j>=0对其进行边界限制。第二个为插入判断条件
		{
			index[j + 1] = index[j];//若不是合适位置，有序组元素向后移动
			j--;
		}
		index[j + 1] = temp2;//找到合适位置，将元素插入。
	}
	//排序完成
	//输出一下结果检验一下
	//底层叶子节点分裂很平凡，子节点面积都是0，排序都是0,1,2,3,4,5
	if (child[0]->type == RECTANGLE)
	{
		std::cout << "发生节点分裂，此时，应当是：前一半是size递增且面积较小的元素，后一半是index递增的元素，size数组排序后为：" << std::endl;
		for (int i = 0; i < RECTANGLE_CAPABILITY + 1; i++)
		{
			std::cout << num[i] << " ";
		}
		std::cout << std::endl << "index数组排序后为：";
		for (int i = 0; i < RECTANGLE_CAPABILITY + 1; i++)
		{
			std::cout << index[i] << " ";
		}
		std::cout << std::endl << std::endl;
	}

	//此时，应当是：前一半是size递增且较小的元素，后一半是index递增的元素
	rectangle *temp_rect = new rectangle(RECTANGLE);
	for (int i = RECTANGLE_CAPABILITY; i >= (RECTANGLE_CAPABILITY + 1) / 2; i--)
	{
		if (index[i] != RECTANGLE_CAPABILITY)
		{
			//是原节点中有的元素
			temp_rect->insert(*(child[index[i]]));
			//从原节点删去
			delete_child(index[i]);
		}
		else
		{
			//是刚加的新元素
			temp_rect->insert(new_rect);
		}
	}
	renovate();
	temp_rect->renovate();
	//新节点创建完毕，将其插入自己的父节点，使之和自己地位相同
	//考虑树增高
	if (parent != nullptr)
	{
		//这不是根节点
		temp_rect->parent = parent;
		parent->insert(*temp_rect);
	}
	else
	{
		//根节点分裂成了2个，不再是根节点
		rectangle *new_root = new rectangle(RECTANGLE);
		new_root->child.push_back(temp_rect);
		temp_rect->parent = new_root;
		new_root->child.push_back(this);
		parent = new_root;
		new_root->renovate();
		root = new_root;
		std::cout << "树发生了增高，根节点改变！" << std::endl;
	}
}

void rectangle::split_quadratic(rectangle & new_rect)
{
	// 令new_rect的index为child.size()。seed[1]_index可能等于child.size
	if (child[0]->type != new_rect.type)
	{
		std::cout << "???";
	}
	std::vector<rectangle*> seed = find_seed(&new_rect);

	// 矩形的节点和新加的节点合并生成2个新的矩形，其子节点分别暂存在r1，r2
	std::vector<rectangle*> r1, r2;
	int total_node_num = RECTANGLE_CAPABILITY + 1;
	double cost1, cost2;
	int max_r1_size = total_node_num / 2, max_r2_size = total_node_num - max_r1_size;

	// 把一个矩形的节点和新加的节点分配到两个组
	// 将种子节点分配
	r1.push_back(seed[0]);
	r2.push_back(seed[1]);
	// 将新加的点分配
	if (&new_rect != seed[1]) { //  新加的点不是seed
		cost1 = new_rect.expand_cost(seed[0]);
		cost2 = new_rect.expand_cost(seed[1]);

		// 分到前半部分或后半部分
		if (cost1 < cost2)
			r1.push_back(&new_rect);
		else
			r2.push_back(&new_rect);
	}

	// 将两个矩形里的点分配
	for (int i = 0; i < child.size(); i++) {
		if (child[i] == seed[0] || child[i] == seed[1]) continue;

		// 一个组合已满,自动分到另一个组
		if (r1.size() >= max_r1_size) {
			r2.push_back(child[i]); continue;
		}
		if (r2.size() >= max_r2_size) {
			r1.push_back(child[i]); continue;
		}

		cost1 = child[i]->expand_cost(seed[0]);
		cost2 = child[i]->expand_cost(seed[1]);

		// 分成前半部分和后半部分
		if (cost1 < cost2)
			r1.push_back(child[i]);
		else
			r2.push_back(child[i]);
	}

	merge(&r1, &r2);
}

void rectangle::split_2to3(rectangle & new_rect)
{
	int sibling_count = 0;
	bool use;
	//sibling_count是父节点的孩子数目（包括他自己），use是本节点是否满足2分3的条件
	rectangle * target = find_sibling(sibling_count, use);
	if (use)
	{
		// 自己和自己的兄弟都已满，开始分裂
		std::vector<rectangle*> seed = parent->child[0]->find_seed(parent->child[1], &new_rect);

		// 2个矩形的节点和新加的节点合并生成3个新的矩形，其子节点分别暂存在r1，r2，r3
		std::vector<rectangle*> r1, r2, r3;
		int total_node_num = 2 * RECTANGLE_CAPABILITY + 1;
		double cost1, cost2, cost3;
		int max_r1_size = total_node_num / 3,
			max_r2_size = (total_node_num - max_r1_size) / 2,
			max_r3_size = total_node_num - max_r1_size - max_r2_size;
		int index; // 最小cost的编号

		std::vector<rectangle*> nodes;
		for (int i = 0; i < child.size(); i++) {
			nodes.push_back(parent->child[0]->child[i]);
			nodes.push_back(parent->child[1]->child[i]);
		}

		// 把2个矩形的节点和新加的节点分配到3个组
		// 将种子节点分配
		r1.push_back(seed[0]);
		r2.push_back(seed[1]);
		r3.push_back(seed[2]);

		// 将新加的点分配
		if (&new_rect != seed[2]) { // 新加的点不是seed
			cost1 = new_rect.expand_cost(seed[0]);
			cost2 = new_rect.expand_cost(seed[1]);
			cost3 = new_rect.expand_cost(seed[2]);

			// 分到前，中，后三部分之一
			index = min_index_of_three(cost1, cost2, cost3);
			if (index == 0)
				r1.push_back(&new_rect);
			else if (index == 1)
				r2.push_back(&new_rect);
			else
				r3.push_back(&new_rect);
		}

		for (int i = 0; i < nodes.size(); i++) {
			// 自己本身是seed
			if (nodes[i] == seed[0] || nodes[i] == seed[1] || nodes[i] == seed[2])
				continue;

			// 计算扩充代价
			cost1 = nodes[i]->expand_cost(seed[0]);
			cost2 = nodes[i]->expand_cost(seed[1]);
			cost3 = nodes[i]->expand_cost(seed[2]);

			if (r1.size() >= max_r1_size) // 第一个组合已满
				cost1 = FLT_MAX;
			if (r2.size() >= max_r2_size) // 第二个组合已满
				cost2 = FLT_MAX;
			if (r3.size() >= max_r3_size) // 第三个组合已满
				cost3 = FLT_MAX;

			// 分成三部分
			index = min_index_of_three(cost1, cost2, cost3);
			if (index == 0)
				r1.push_back(nodes[i]);
			else if (index == 1)
				r2.push_back(nodes[i]);
			else
				r3.push_back(nodes[i]);
		}

		parent->child[0]->merge(&r1, &r2, &r3, parent->child[1]);
	}
	else if(target!=nullptr)
	{
		target->split_quadratic(new_rect);
	}
	else
	{
		split_quadratic(new_rect);
	}
}

void rectangle::merge(std::vector<rectangle*>* r1, std::vector<rectangle*>* r2)
{
	child.clear();
	child.reserve(RECTANGLE_CAPABILITY + 1);

	rectangle *split_rect = new rectangle(RECTANGLE);
	for (int i = 0; i < r1->size(); i++)
	{
		if (&*(*r1)[i] != nullptr)
		{
			this->insert(*(*r1)[i]);
		}
		else
		{
			std::cout << "???";
		}
	}

	for (int i = 0; i < r2->size(); i++)
		if (&*(*r2)[i] != nullptr)
		{
			split_rect->insert(*(*r2)[i]);
		}
		else
		{
			std::cout << "???";
		}

	renovate();
	split_rect->renovate();

	if (parent != nullptr)
	{
		//这不是根节点
		split_rect->parent = parent;
		parent->insert(*split_rect);
	}
	else
	{
		//根节点分裂成了2个，不再是根节点
		rectangle *new_root = new rectangle(RECTANGLE);
		new_root->child.push_back(split_rect);
		split_rect->parent = new_root;
		new_root->child.push_back(this);
		parent = new_root;
		new_root->renovate();
		root = new_root;
		std::cout << "*****树发生了增高，根节点改变！*****\n" << std::endl;
	}
}

void rectangle::merge(std::vector<rectangle*>* r1, std::vector<rectangle*>* r2, 
	std::vector<rectangle*>* r3, rectangle* sibling)
{
	child.clear();
	child.reserve(RECTANGLE_CAPABILITY + 1);
	sibling->child.clear();
	sibling->child.reserve(RECTANGLE_CAPABILITY + 1);

	rectangle *split_rect = new rectangle(RECTANGLE);

	for (int i = 0; i < r1->size(); i++)
		this->insert(*(*r1)[i]);
	for (int i = 0; i < r2->size(); i++)
		sibling->insert(*(*r2)[i]);
	for (int i = 0; i < r3->size(); i++)
		split_rect->insert(*(*r3)[i]);

	renovate();
	sibling->renovate();
	split_rect->renovate();

	if (parent != nullptr) {
		//这不是根节点
		split_rect->parent = parent;
		parent->insert(*split_rect);
	}
	else {
		//根节点分裂成了2个，不再是根节点
		rectangle *new_root = new rectangle(RECTANGLE);
		split_rect->parent = new_root;
		parent = new_root;
		new_root->insert(*split_rect);
		new_root->insert(*this);
		new_root->insert(*sibling);
		root = new_root;
		std::cout << "*****树发生了增高，根节点改变！*****\n" << std::endl;
	}
}

void rectangle::naive_search(rectangle & target, std::vector<rectangle*>* result, int interval)
{
	if (target.type == RECTANGLE)
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
	else if (target.type == POINT && interval != -1)
	{
		rectangle rect(RECTANGLE);
		for (int i = 0; i < rect.color_count; i++)
		{
			(*rect.max_point)[i] = (*target.max_point)[i] + interval;
			(*rect.min_point)[i] = (*target.min_point)[i] - interval;
			if ((*rect.min_point)[i] < 0)
				(*rect.min_point)[i] = 0;
		}
		naive_search(rect, result);
	}
}

double rectangle::get_min_distance(rectangle & node) {
	double dist = 0;
	for (int i = 0; i < COLOR_COUNT; i++) {
		if ((*node.min_point)[i] >= (*min_point)[i] && (*node.min_point)[i] <= (*max_point)[i])
			dist += 0;
		else
			dist += std::min(pow((*node.min_point)[i] - (*min_point)[i], 2), pow((*node.min_point)[i] - (*max_point)[i], 2));
	}

	return sqrt(dist);
}

// 最远对角线距离。不是真正的最远距离，比理想最远距离大
double rectangle::get_max_distance(rectangle & node) {
	double dist = 0;
	for (int i = 0; i < COLOR_COUNT; i++)
		dist += std::min(abs((*node.min_point)[i] - (*min_point)[i]), abs((*node.min_point)[i] - (*max_point)[i]));

	return sqrt(dist);
}

bool sort_result(rectangle* r1, rectangle* r2) {
	return (r1->cost < r2->cost);
}

void rectangle::knn_search(rectangle & target, std::vector<rectangle*>* result, int need_count) {
	double max_dist = 0;
	std::vector<rectangle*>* overlap_node = new std::vector<rectangle*>;
	_knn_search(target, result, need_count, max_dist, overlap_node);

	for (int i = 0; i < overlap_node->size(); i++)
		(*overlap_node)[i]->expand(target, result, need_count, max_dist);

	while (result->size() < need_count) {
		for (int i = 0; i < overlap_node->size(); i++) {
			max_dist *= 2;
			(*overlap_node)[i]->expand(target, result, need_count, max_dist);
			(*overlap_node)[i] = (*overlap_node)[i]->parent;
		}
	}

	for (int i = 0; i < result->size(); i++) {
		(*result)[i]->cost = (*result)[i]->expand_cost(&target);
	}
	std::sort(result->begin(), result->end(), sort_result);
	result->erase(result->begin() + need_count, result->end());
}

// knn搜索时target是一个点
void rectangle::_knn_search(rectangle & target, std::vector<rectangle*>* result,
	int need_count, double &max_dist, std::vector<rectangle*>* overlap_node)
{
	double min_worst = FLT_MAX, temp_worst = 0;
	double *best_dist = new double[child.size()];
	// 用于出现叶子结点时
	bool leaf_flag = false;

	for (int i = 0; i < child.size(); i++) {
		if (child[i]->type == RECTANGLE) { // 孩子节点是矩形
			temp_worst = child[i]->get_max_distance(target);
			best_dist[i] = child[i]->get_min_distance(target);

			if (temp_worst < min_worst)
				min_worst = temp_worst;
		}
		else { // 孩子节点是叶节点
			result->push_back(child[i]);
			leaf_flag = true;

			temp_worst = child[i]->get_max_distance(target); // 两个节点之间的距离
			if (temp_worst > max_dist)
				max_dist = temp_worst;
		}
	}

	// 剪枝 孩子节点是矩形
	if (!leaf_flag) {
		for (int i = 0; i < child.size(); i++) {
			if (best_dist[i] > min_worst) // 最近的距离都比最小的距离大，不搜索
				continue;
			else
				child[i]->_knn_search(target, result, need_count, max_dist, overlap_node);	// 继续搜索
		}
	}

	// 结果个数不足时，基于这些矩形扩充（在expand函数）
	if (leaf_flag && overlap(target) && overlap_node)
		overlap_node->push_back(this);

	delete[]best_dist;
}

void rectangle::get_leaf_nodes(std::vector<rectangle*>* res) {
	if (this->type == POINT) {
		res->push_back(this);
		return;
	}

	for (int i = 0; i < child.size(); i++)
		child[i]->get_leaf_nodes(res);
}

void rectangle::expand(rectangle & target, std::vector<rectangle*>* result, int need_count, double &max_dist)
{
	rectangle* parent = this->parent;
	// 为了找出不跟target重合，但是比在结果里最远的节点更近的所有点
	// 把节点target展开成正方形，其权值为2max_dist，搜索跟正方形重合的矩形
	rectangle* new_rect = new rectangle(RECTANGLE);
	for (int i = 0; i < COLOR_COUNT; i++) {
		(*new_rect->min_point)[i] = (*target.min_point)[i] - max_dist; // FIXME 坐标不允许负号
		(*new_rect->max_point)[i] = (*target.max_point)[i] + max_dist;
	}
	for (int i = 0; i < parent->child.size(); i++) {
		// 跟节点target不重合但跟正方形new_rect重合
		// 如果跟节点target重合的话肯定会搜索到，在这儿不再重复
		if (parent->child[i]->overlap(*new_rect)) {
			std::vector<rectangle*>* leaf_nodes = new std::vector<rectangle*>;
			parent->child[i]->get_leaf_nodes(leaf_nodes);
			for (int j = 0; j < leaf_nodes->size(); j++) {
				auto match = std::find(result->begin(), result->end(), (*leaf_nodes)[j]);
				if (match == result->end()) // 不存在于结果
					result->push_back((*leaf_nodes)[j]);
			}
		}
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

void create_tree_from_file(std::string file_name)
{
	std::ifstream fin(file_name);
	if (!fin.good()) {
		std::cerr << "error: open file failed!\n";
		abort();
	}

	std::string line;
	std::vector<int>* point_data;
	int ID = 1;
	rectangle * new_rect;
	while (std::getline(fin, line))
	{
		point_data = new std::vector<int>;
		point_data->reserve(COLOR_COUNT);
		std::stringstream analyse_line(line);
		char c;
		int num;
		analyse_line >> c >> c >> c >> c;
		for (int i = 0; i < COLOR_COUNT; i++)
		{
			analyse_line >> num;
			point_data->push_back(num);
		}
		//点数据插入完毕
		new_rect = new rectangle(POINT, point_data);
		new_rect->point_data->ID = ID;
		ID++;
		image_data.push_back(new_rect);
		(root->search_insert_position(*new_rect))->insert(*new_rect);
	}
}

void create_tree_from_file2(std::string file_name)
{
	std::ifstream fin(file_name);
	if (!fin.good()) {
		std::cerr << "error: open file failed!\n";
		abort();
	}

	std::string line;
	std::vector<int>* point_data;
	int ID = 1;
	rectangle * new_rect;
	char name[100] = { 0 };
	char temp = 0;
	int count = 0;
	while (std::getline(fin, line))
	{
		point_data = new std::vector<int>;
		point_data->reserve(COLOR_COUNT);
		std::stringstream analyse_line(line);
		memset(name, 0, 100);
		count = 0;
		int num;
		analyse_line >> temp;
		while (temp != ':') {
			name[count] = temp;
			count++;
			analyse_line >> temp;
		}
		std::string image_name(name);

		for (int i = 0; i < COLOR_COUNT; i++)
		{
			analyse_line >> num;
			point_data->push_back(num);
		}
		//点数据插入完毕
		new_rect = new rectangle(POINT, point_data);
		new_rect->point_data->ID = ID;
		new_rect->point_data->image_name = image_name;
		ID++;
		image_data.push_back(new_rect);
		(root->search_insert_position(*new_rect))->insert(*new_rect);
	}
}

void create_tree_from_file3(std::string file_name)
{
	std::ifstream fin(file_name);
	if (!fin.good()) {
		std::cerr << "error: open file failed!\n";
		abort();
	}

	std::string line;
	std::vector<int>* point_data;
	int ID = 1;
	int ct = 0;
	rectangle * new_rect;
	char name[100] = { 0 };
	char temp = 0;
	int count = 0;
	while (std::getline(fin, line))
	{
		//if (ct>3000)
		//	break;
		std::stringstream analyse_line(line);
		//memset(name, 0, 100);
		count = 0;
		int num;
		//analyse_line >> temp;
		/*while (temp != ':') {
		name[count] = temp;
		count++;
		analyse_line >> temp;
		}*/
		if (ct % 2 == 0)//
		{
			analyse_line >> name;
			point_data = new std::vector<int>;
			point_data->reserve(COLOR_COUNT);
		}
		if (ct % 2 == 1)
		{
			std::string image_name(name);
			//std::cout<<name<<std::endl;
			//
			for (int i = 0; i < COLOR_COUNT; i++)
			{
				analyse_line >> num;
				point_data->push_back(num);
				//std::cout<<num<<" ";
			}
			//std::cout<<std::endl;
			//点数据插入完毕
			new_rect = new rectangle(POINT, point_data);
			new_rect->point_data->ID = ID;
			new_rect->point_data->image_name = image_name;
			new_rect->point_data->data = point_data;
			ID++;
			//std::cout<<ct<<std::endl;
			image_data.push_back(new_rect);
			(root->search_insert_position(*new_rect))->insert(*new_rect);
			memset(name, 0, 100);
		}
		ct++;
	}
}