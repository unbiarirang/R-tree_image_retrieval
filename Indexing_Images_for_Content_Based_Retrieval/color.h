#pragma once
class color
{
public:
	color();
	~color();



	//��ɫ����
	int r;
	int g;
	int b;
};

//�ж�������ɫ�Ƿ��ܹ����֣�����ͼƬʱ�õ���
bool color_distinguish(color color_1, color color_2);