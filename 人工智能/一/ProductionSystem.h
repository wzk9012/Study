#pragma once

#include <stdio.h>

#define MAX_SIZE  50

struct Node {  				//������ 
    int id; 				//���
    char name[MAX_SIZE];  	//���� 
};

struct Statu { 		//״̬�� 
    int statu; 		//0δʹ�� 1ʹ�ù�һ�� 2û��ʹ�õ���Ӱ�� 
    int id;
}; //���滻�޸�

struct Fact { 				//���ݿ� 
    int st_num; 			//��ʼ��ʵ��
    int now_num; 			//��ǰ��ʵ�������ϸ��¼����м����
    int neFlag; 			//����������1 
    Statu data[MAX_SIZE]; 	//�������� 
};

struct Rule {  			//����� 
    int data_num; 		//ǰ�����
    int id_set[10]; 	//ǰ�������� 
    int result; 		//���ս���
    int matched;  		//�Ƿ���ƥ��ɹ�  0 1
}; 		//����� 



