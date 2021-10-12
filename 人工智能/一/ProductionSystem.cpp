#include "ProductionSystem.h"
#include <stdlib.h>
#include <memory.h>

using namespace std;

Rule ruleDB[MAX_SIZE];
Node targetDB[20];			//Ŀ�궯��
Node featuresDB[MAX_SIZE];	//�������ݿ�
Fact indata;  			//�������� 
int rule_num = 0; 		//��������
int feature_num = 0; 		//������

void readFeature() { //��ȡ������ 
    FILE *fp;
    if ((fp = fopen("features.txt", "r")) == NULL) {
        printf("Cant open features.txt!\n");
        exit(0);
    }

    int i = 0;
    while ((fscanf(fp, "%d %s", &featuresDB[i].id, &featuresDB[i].name)) != EOF)
        i++; //����

    feature_num = i;
    fclose(fp);
}

void readRule() { //��ȡ����� 
    FILE *fp;
    if ((fp = fopen("rules.txt", "r")) == NULL) {
        printf("Cant open rules.txt!\n");
        exit(0);
    }

    int i = 0;
    char ch;
    ruleDB[i].data_num = 0;

    int t = 1;
    while ((ch = fgetc(fp)) != EOF) { //�ļ���β 
        int cnt = 0; //����ǰ����� 
        while (ch != '\n'&&ch != EOF) { //ÿһ������ 
            int tmp_id = 0;

            while (ch >= '0'&&ch <= '9') {
                tmp_id = tmp_id * 10 + ch - '0';
                ch = fgetc(fp);
            }
            ruleDB[i].id_set[cnt++] = tmp_id;

            tmp_id = 0; //�������
            if (ch == '=') {
                ch = fgetc(fp);
                while (ch >= '0'&&ch <= '9') {
                    tmp_id = tmp_id * 10 + ch - '0';
                    ch = fgetc(fp);
                }
                ruleDB[i].result = tmp_id;
            }
            else if (ch == '&') ch = fgetc(fp);
        }

        ruleDB[i].data_num = cnt; //�ù���ǰ���� 
        i++;
    }
    rule_num = i; //�������� 

    fclose(fp);
    //	for(int i=0;i<rule_num;i++){
    //		for(int j=0;j<rule[i].data_num;j++){
    //			printf("%s",data[rule[i].id_set[j]].name);
    //			printf("%c",j==rule[i].data_num-1?'=':'&');
    //		}
    //		printf("%s\n",data[rule[i].result].name);
    //	}
}

void readTarget() { //��ȡĿ��� 
    FILE *fp;
    if ((fp = fopen("target.txt", "r")) == NULL) {
        printf("Cant open target.txt!\n");
        exit(0);
    }

    int i = 0;
    while ((fscanf(fp, "%d %s", &targetDB[i].id, &targetDB[i].name)) != EOF) i++; //����
    fclose(fp);
}

//����ֵ��ʾ��������������
int valueInput() { //�����Ƿ���Ч 

    int tmp; //�������
    int cnt = 0; //������ 

    while (cnt <=0)
    {
        printf("��������֪�������������,��-1������\n");
        do {
            scanf("%d", &tmp);
            if (-1 == tmp)
                break;
            else if (tmp >= feature_num || tmp<-1) { //Խ�� 
                printf("�������ֻ����0-%d֮�䣬����������:\n", feature_num - 1);
                fflush(stdin);
                cnt = 0;
                break;
            }
            indata.data[cnt++].id = tmp;
        } while (cnt<MAX_SIZE);
    }
    indata.st_num = cnt; //�����������
    indata.now_num = indata.st_num;

    return cnt;
}

int isOther(int in_id, int result) { //�����ͻ 
    int queue[MAX_SIZE], vis[MAX_SIZE]; //������б�� 
    int head = 0, tail = 1;
    int flag = 0;

    queue[0] = result; vis[result] = 1;
    while (head != tail&&flag != 1) {
        for (int i = 0; i<rule_num; i++)
            if (queue[head] == ruleDB[i].result) { //���ƥ�������� 
                for (int j = 0; j<ruleDB[i].data_num; j++)
                    if (indata.data[in_id].id == ruleDB[i].id_set[j])
                        flag = 1;
                    else {
                        if (vis[ruleDB[i].id_set[j]] != 1) {
                            queue[tail++] = ruleDB[i].id_set[j];
                            vis[ruleDB[i].id_set[j]] = 1;
                        }
                    }
            }
        head++;
    }
    return flag;
}

bool isContract(int result) { //�Ƿ��ͻ 
    for (int i = 0; i<indata.now_num; i++)
        if (indata.data[i].statu == 0 && indata.data[i].id != result) { //δ���ʵ��������� 
            if (isOther(i, result))
                indata.data[i].statu = 2;
            else
                return true;
        }
    return false;
}

void isMatch(int result, int *finish) { //ƥ�����ս���
    if (result >= feature_num) { //����������� 
        *finish = 1; //������� 
        if (isContract(result))
            printf("������ͻ��û�������Ķ��\n");
        else
            printf("���� %s.\n", targetDB[result - feature_num-1].name);
    }

}

void addToFact(int rule_id, int *finish) { //ƥ����� 
    for (int i = 0; i < ruleDB[rule_id].data_num; i++) //��Ƿ���
    {
        for (int k = 0; k < indata.now_num; k++)
        {
            if (indata.data[k].id == ruleDB[rule_id].id_set[i])
                indata.data[k].statu = 1;
        }
    }

    ruleDB[rule_id].matched = 1;

    int vis = 0;  //��ǽ����Ƿ���� 
    for (int i = 0; i < indata.now_num; i++)
    {
        if (indata.data[i].id == ruleDB[rule_id].result)   //ƥ�����
        {
            vis = 1; 
            break;
        }
    }

    if (vis == 0) {
        indata.data[indata.now_num].id = ruleDB[rule_id].result;
        //printf("add the %d\n",indata.data[indata.now_num].id);
        indata.now_num++;
    }

    isMatch(ruleDB[rule_id].result, finish);
}

void count_unum(int *unusef, int *u_num) { //δ���������� 
    int vis[MAX_SIZE]; //δ������ʵ������ 
    memset(vis, 0, sizeof(vis));

    for (int i = 0; i<indata.now_num; i++) {
        if (indata.data[i].statu == 0 && vis[indata.data[i].id] != 1) {
            unusef[*u_num] = indata.data[i].id;
            ++(*u_num);
            vis[indata.data[i].id] = 1;
        }
    }

}

void mayMatch(int tflag, int rule_id, int *getflag) { //ƥ������Խ��� 
    Node *res;
    int res_id;
    res = featuresDB;
    //printf("result=%d\n",rule[rule_id].result);
    res_id = ruleDB[rule_id].result;
    getflag[ruleDB[rule_id].result] = 1;

    if (res_id>23) {
        res = targetDB;
        res_id -= 24;
    }

    if (tflag == 0)
        printf("��������ȫ��������%s", res[res_id].name);
    else
        printf("��%s", res[res_id].name);
}

void guess() { //���ܽ��� 
    int tflag, ecnt; //��ͬ����
    int unusef[MAX_SIZE], u_num = 0;
    int getflag[MAX_SIZE];//�����Ƴ����"���ܽ���"������Ϊ1
    memset(getflag, 0, sizeof(getflag));

    count_unum(unusef, &u_num);

    tflag = 0;
    for (int i = 0; i<rule_num; i++) {
        ecnt = 0;
        for (int j = 0; j<ruleDB[i].data_num; j++)
            for (int k = 0; k<u_num; k++)
                if (unusef[k] == ruleDB[i].id_set[j])
                    ecnt++;
        if (ecnt * 2 >= ruleDB[i].data_num&&getflag[ruleDB[i].result] != 1) {
            mayMatch(tflag, i, getflag);
            tflag = 1;
        }
    }
    if (tflag == 0)
        printf("�������㣬�����Ƴ�����ʲô���\n");
    else
        printf("�Ĳ�������.\n");
}

void forwardWork() { //�������� 
    int finish = 0; //�����Ƿ���� 0 1
    int factNew = 1; //һ�������Ƿ�ó�����ʵ
    int fitFlag; //1 ������ 
    int fitPart;  //1 ���ַ��� 

    while (!finish&&factNew == 1) {
        factNew = 0;

        for (int i = 0; i<rule_num&&ruleDB[i].matched != 1 && finish == 0; i++) {  //ȡ��һ������ 
            if (indata.now_num >= ruleDB[i].data_num) {  //������ʵ����С�ڵ�ǰ���������� 
                fitFlag = 1;
                for (int j = 0; j<ruleDB[i].data_num&&fitFlag == 1; j++) { //�Ƚ��Ƿ�ƥ��ù���  
                    fitPart = 0;
                    for (int k = 0; k<indata.now_num; k++)
                        if (indata.data[k].id == ruleDB[i].id_set[j])
                            fitPart = 1; //����ƥ�� 
                    fitFlag = fitPart;
                }

                if (fitFlag == 1) { //��ù���ƥ�� 
                                    //printf("ƥ���%d������\n",i);
                    factNew = 1;
                    addToFact(i, &finish); //���µ���ʵ�� 
                    fitFlag = 0;
                }
            }
        }
    }
    if (factNew == 0) //��û���Ƴ��κζ�����Ƿ��п��ܵó�һЩ���� 
        guess();
}



//////////////////////////////////////////////////////////////////////

void InitDatabase() { //��ʼ�����ݿ� 
              //��ȡ�ļ� 
    readFeature();
    readRule();
    readTarget();
}

void init() { //��ʼ��״ֵ̬ 	
    for (int i = 0; i<MAX_SIZE; i++) {
        ruleDB[i].matched = 0; //δƥ��ɹ�
        indata.neFlag = 0;
        indata.data[i].statu = 0; //����δ���� 
    }
}

void showFeatureDB() { //���� 
    printf("\n\t******************����ʶ��ϵͳ******************\t\n\n\t");
    for (int i = 0; i<feature_num; i++) {
        printf("[%2d] %-12s", featuresDB[i].id, featuresDB[i].name);
        if (i % 3 == 2) printf("\n\t");
    }
    printf("\n\t***********************************************\t\n");
}



int main() { //����������  
    InitDatabase();
    char ch;
    do {
        //init();
        system("cls");
        showFeatureDB();

        valueInput(); //����������Ϣ

        forwardWork();	//��������

        printf("\n�����������,��'n'��'N'�˳�:");
        ch = getchar(); 
        ch = getchar();
    } while (ch != 'n'&&ch != 'N');  //�������
    return 0;
}

