#include <iostream>
#include <algorithm>
using namespace std;
 
char *facts[]={"",
	"��ۻ","����","������","����ǰ��","��צ",      //1-5
	"Ȯ��","����","�µ�","�ܷ�","����ë",
	"����","ʳ����","����","����","ë��",
	"�Ʒ�","��ɫ������","��Ӿ","����","������",
	"������","���ߵ�","�ƺ�ɫ"
};
 
char *resultSet[]={"",
	"������","���","����","����","����¹","��","��"  //1-7
};
 
struct factDB{         //�ۺ����ݿ��е���֪��ʵ
	 int curNum;
	 int fact[30];
	 int initNum;
}factDb;
 
struct Rule{          //���� �ṹ��
	int factNum;      
	int fact[5];
	bool endResult;     //�Ƿ�Ϊ�����
	int resultID;       //�Ƴ��Ľ��ID
	bool used;			//��ʹ�ù���־
	bool possible;      //�Ƿ���ܱ�־�����-9�����ܷɣ���Rule����9�����ۺ����ݿ�����-9 ��ù����ǲ�����
	int needFactPos;    //ͨ����ʵ�Ƚϣ���¼��һ����Ҫ����ʵλ�ã������´αȽϿ�ʼ����ƺ�����Ѱ�Ҹ�λ��ֵ�Ƿ���������
}*rule;
 
bool findNeedFact(int factID,int pos)     //Ѱ��needFactPosλ��ֵ�Ƿ�������ۺ���ʵ����
{
	int i=pos;
	for(;i<factDb.curNum;i++) 
	{
		if(factID==factDb.fact[i])return true;
		if(factID<factDb.fact[i])return false;
	}
	return false;
}
 
bool cmpArray(Rule &r)      //�����������ʵ�Ƚ�
{
	int i,j;
	i=r.needFactPos;
	j=i;
	while(i<r.factNum)
	{
		while(j<factDb.curNum)
		{
			if(r.fact[i]==factDb.fact[j])
			{
				i++;
				if(i==r.factNum)return true;
			}
			else if(r.fact[i]<factDb.fact[j])
			{
				if(r.fact[i]<0 && findNeedFact(-r.fact[i],i))r.possible=false;   //�������෴���ʱ����ǲ�����
				r.needFactPos=i;
				return false;
			}
			j++;
		}
		if(i<r.factNum){r.needFactPos=i;return false;}
	}
	return true;
}
 
int cmpFact(Rule &r)
{
	if(r.factNum>factDb.curNum)return 0;        //��֪��ʵ��û�иù�����ʵ��
	if(r.needFactPos)
	{
		if(!findNeedFact(r.fact[r.needFactPos],r.needFactPos))return 0; //��Ҫ��ֵ��������
		else r.needFactPos++;
	}
	if(cmpArray(r))
	{
		r.used=true;
		if(r.endResult)return 1;     //�õ������ս��
		else return 2;               //�õ��м���
	}else return 0;
}
 
int main()
{
	freopen("in.txt","r",stdin);
	int n;
	cin>>n;
	rule=(Rule *)malloc(sizeof(Rule)*n);
	int i,j;
	for(i=0;i<n;i++)
	{
		cin>>rule[i].factNum;
		for(j=0;j<rule[i].factNum;j++)
			cin>>rule[i].fact[j];
		sort(rule[i].fact,rule[i].fact+rule[i].factNum);
		cin>>rule[i].endResult>>rule[i].resultID;
		rule[i].used=false;
		rule[i].possible=true;
		rule[i].needFactPos=0;
	}
	
	cin>>factDb.initNum;
	factDb.curNum=factDb.initNum;
	cout<<"ԭʼ��ʵ�ۺ����ݿ⣺"<<endl;
	for(i=0;i<factDb.initNum;i++)
	{
		cin>>factDb.fact[i];
		cout<<facts[factDb.fact[i]]<<"\t";
	}
	cout<<endl<<endl;
	sort(factDb.fact,factDb.fact+factDb.curNum);
 
	int res;
	bool isEnd=false;
	while(!isEnd)
	{
		isEnd=true;
		for(i=0;i<n;i++) 
		{
			if(rule[i].used || !rule[i].possible)continue;
			res=cmpFact(rule[i]); 
			if(res==0)continue;//��ƥ��
			else if(res==1)   //ƥ��ɹ����õ����ս��
			{
				cout<<"�õ����ս����"<<resultSet[rule[i].resultID]<<endl;
				return 0;
			}
			else if(res==2)//ƥ��ɹ����õ��м���
			{
				cout<<"�õ��м�����"<<facts[rule[i].resultID]<<endl;
				cout<<"\t������뵽���е���ʵ���ݿ��С�����"<<endl<<endl;
				factDb.fact[factDb.curNum]=rule[i].resultID;
				factDb.curNum++;
				sort(factDb.fact,factDb.fact+factDb.curNum);      //��������������������룬Ӧʹ�ò�������Ϊ��
				isEnd=false;
			}
		}
	}
	cout<<"������ʵ�޷��ƶϳ������"<<endl;
 
	return 0;
}
