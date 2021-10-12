

#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#define MAX_JOB_NUM 5
#define MAX_MACHINE_NUM 4
#define MAX_CHROMOSOME_NUM 20 //
#define INVALID_FITNESS -1

using namespace std;

class Random
{
public:
    Random() {
        srand(static_cast<unsigned int>(time(nullptr)));
    };
    ~Random() {};

    /*����[start,end]���������*/
    int getInt(int start, int end) {
        return rand() % (end - start + 1) + start;
    }
    /*����[start,end]�����������*/
    float getFloat(float start, float end) {
        return (end - start)*(float)rand() / RAND_MAX + start;
    }
};
Random random;


/*
�±�ѡ���������ظ������ѡ���±ꡣ
���еķ��������ظ����ã����������Ҫѡ���5��С�꣬����������������int Choose().
*/
class IndexChooser
{
public:
    IndexChooser(int length) {
        Reset(length);
    }
    void Reset(int length) {
        index.resize(length);
        for (int i = 0; i < length; i++)
            index[i] = i;
    }
    int Choose() {
        int temp = random.getInt(0, index.size() - 1);
        int a = index[temp];
        index.erase(index.begin() + temp);
        return a;
    }
    void Choose(int &a, int &b) {
        a = Choose();
        b = Choose();
    }
private:
    vector<int> index;
};


class Process
{
public:
    int machineId;
    int time;
    void Set(int id, int t) {
        machineId = id;
        time = t;
    }
    void Show(){
        printf("(%d,%d)", machineId, time);
    }
};

class Job
{
public:
    Process procInfo[MAX_MACHINE_NUM];
    int procNum;  //���������

    Job() {
        procNum = 0;
    }
    void Append(int machineId, int time) {
        procInfo[procNum].Set(machineId, time);
        procNum++;
    }
    void Reset() {
        procNum = 0;
    }
    void Show() {
        printf("Process Num: %d.\r\n", procNum);
        for (int i = 0; i < procNum; i++){
            procInfo[i].Show();
        }
        printf("\r\n");
    }
};
class JobManager
{
public:
    JobManager() {};
    ~JobManager() {};

    bool Init() {
        printf("input job and machine:\r\n");
        scanf("%d%d", &jobNum, &machineNum);

        for (int i = 0; i < jobNum; i++) {
            int p;
            printf("input job%d  process:\r\n", i);
            scanf("%d", &p);
            printf("input machine and time:\r\n");
            for (int j = 0; j < p; j++) {
                int m, t;
                scanf("%d%d", &m, &t);
                jobsInfo[i].Append(m, t);
            }
        }
        return true;
    }
    void Show() {
        printf("####################################\r\n");
        printf("Job Num: %d, Machine Num: %d\r\n", jobNum, machineNum);
        for (int i = 0; i < jobNum; i++) {
            jobsInfo[i].Show();
        }
        printf("####################################\r\n");
    }
    int GetMachineNum() const { return machineNum; }
    int GetJobNum() const { return jobNum; }

    Job jobsInfo[MAX_JOB_NUM];   //������Ϣ
private:
    int machineNum;          //��������
    int jobNum;              //��������
};

class Chromosome
{
public:
    Chromosome() {};
    ~Chromosome() {}
    Chromosome(const Chromosome &other) {
        length = other.length;
        memcpy(Genes, other.Genes, length * sizeof(int));
        fitness = other.fitness;
        cFitness = other.cFitness;
    }
    Chromosome &operator=(const Chromosome &other) {
        length = other.length;
        memcpy(Genes, other.Genes, length * sizeof(int));
        fitness = other.fitness;
        cFitness = other.cFitness;
        return *this;
    }

    void RandInit(int len) { //�����ʼ��
        IndexChooser index(len);
        length = len;
        for (int i = 0; i < len; i++)
            Genes[i] = index.Choose();

        fitness = INVALID_FITNESS;
        cFitness = INVALID_FITNESS;
    }

    int GetLength()const { return length; }
    const int *GetGene() { return Genes; }
    float GetFitness()const { return fitness; }
    void SetFitness(float fit) { fitness = fit; }
    float GetCFitness()const { return cFitness; }
    void SetCFitness(float cfit) { cFitness = cfit; }
    void Show() {
        for (int i = 0; i < length; i++)
        {
            printf("%d", Genes[i]);
        }
        printf(": %f\r\n", fitness);
    }

    void Mutation(int n = 1) {
        if (n > length) n = length;
        IndexChooser index(length);
        int first, second, temp;
        for (int k = 0; k < n; k++)
        {
            index.Choose(first, second);
            temp = Genes[first];
            Genes[first] = Genes[second];
            Genes[second] = temp;
        }
    }

    //������������������µ�Ⱦɫ�壬�ֱ���this�Լ�mother
    void CrossOver(Chromosome &mother) {
        IndexChooser index(length);

        int start, end;
        index.Choose(start, end);
        if (start > end) {
            int t = start;
            start = end;
            end = t;
        }
        int buf[MAX_JOB_NUM];
        memcpy(buf, Genes, length * sizeof(int));
        GetGenFromMother(Genes, mother.Genes, length, start, end);
        GetGenFromMother(mother.Genes, buf, length, start, end);
    }

private:
    //���[a,b]֮���Ƿ��������gene
    bool Contain(int dat[], int len, int gene, int a, int b) {
        for (int i = a; i <= b; i++)
        {
            if (dat[i] == gene)return true;
        }
        return false;
    }

    //��mother�н�����[a,b]֮��Ļ����ƹ���
    void GetGenFromMother(int child[], int mother[],int len, int start, int end)
    {
        int index = 0;
        for (int i = 0; i < start; i++)
        {
            while (index<len && Contain(child, len, mother[index], start, end))index++;
            child[i] = mother[index];
            index++;
        }
        for (int i = end+1; i < len; i++)
        {
            while (index<len && Contain(child, len, mother[index], start, end))index++;
            child[i] = mother[index];
            index++;
        }
    }
private:
    int Genes[MAX_JOB_NUM];
    int length;
    float fitness; //��Ӧ��
    float cFitness;//�ۼ���Ӧ�ȡ�ѡ������ʱ����Ҫ�õ�
};

class Population
{
public:
    Population(int iterNum = 50,float mutation = 0.1f, float cross = 0.6f) {
        nIterNum = iterNum; fMutation = mutation; fCross = cross;
    };
    ~Population() {};
    void Init() {
        jobs.Init();
        int genelen = jobs.GetJobNum();

        //��ʼ����Ⱥ��
        for (int i = 0; i < chromosomes.size(); i++)
            chromosomes[i].RandInit(genelen);
    }
    void UpdateFitness(){
        for (int i = 0; i < chromosomes.size(); i++)
            chromosomes[i].SetFitness(CalFitness(jobs, chromosomes[i]));
    }  
    void Show() {
        jobs.Show();
        for (int i = 0; i < chromosomes.size(); i++)
            chromosomes[i].Show();
    }
    void MakeNewGeneration();
private:
    float CalFitness(const JobManager &jobManager, Chromosome &chromosome);
    void Sort() {
        sort(chromosomes.begin(), chromosomes.end(),
            [](const Chromosome &a, const Chromosome &b) -> bool { return a.GetFitness()< b.GetFitness(); });
    }
    void SelectNewGeneration();
    void Crossover();
    void Mutation();
    void UpdateElitist() {
        nElitistNum = chromosomes.size() * 3 / 100;
        if (nElitistNum < 1)nElitistNum = 1;
    }

    /*
    �����ѡ��ʹ�ý�������(Tournament Selection)�������Ϊ��������Ⱥ�г�ȡn���壬
    �����ǽ��о���(������)����ȡ���е����ŵĸ��塣�μӽ������ĸ���Ϊ������Ⱥ��yĬ��ֵΪ3��

    ʹ�ý�������������Ϊ
    ʱ�临�Ӷȸ�С����O(n)
    �ײ��л�����
    ��������ֲ����ŵ�
    ����Ҫ�����е���Ӧ��ֵ�������������
    */
    Chromosome& TournamentSelection(int n = 3) {
        IndexChooser index(chromosomes.size());
        Chromosome *pBest = NULL;
        for (int i = 0; i < n; i++)
        {
            Chromosome *pCur = &chromosomes[index.Choose()];
            if (!pBest)
                pBest = pCur;
            else if (pBest->GetFitness()> pCur->GetFitness())
                pBest = pCur;
        }
        return *pBest;

    }
private:
    vector<Chromosome> chromosomes=vector<Chromosome>(MAX_CHROMOSOME_NUM);
    JobManager jobs;

    float fMutation;//������
    float fCross;//�������
    int   nIterNum;  //�����Ĵ���
    int   nElitistNum;//��Ӣ������
};

#define MAX(A,B) ((A)>(B)?(A):(B))
float Population::CalFitness(const JobManager &jobManager, Chromosome &chromosome)
{
    int totalTime = 0, machineTime[MAX_MACHINE_NUM] = {};
    int c[MAX_JOB_NUM][MAX_MACHINE_NUM] = {}; //�ο��̲ģ�c[i][j]��ʾ����i�ڻ���j�ϼӹ���Ҫ��ʱ��
    const int *pGene = chromosome.GetGene();
    
    //���ݽ̲��еĹ�ʽ6.5.1����
    int j1 = pGene[0];
    c[j1][0] = jobManager.jobsInfo[j1].procInfo[0].time;

    //���ݽ̲��еĹ�ʽ6.5.2����
    for (int k = 1; k < jobManager.jobsInfo[j1].procNum; k++)
        c[j1][k] = c[j1][k-1]+ jobManager.jobsInfo[j1].procInfo[k].time;

    //���ݽ̲��еĹ�ʽ6.5.3����
    for (int i = 1; i < jobManager.GetJobNum(); i++)
    {
        int ji = pGene[i];
        int ji_1 = pGene[i-1];
        c[ji][0] = c[ji_1][0] + jobManager.jobsInfo[ji].procInfo[0].time;
    }

    //���ݽ̲��еĹ�ʽ6.5.4����
    for (int i = 1; i < jobManager.GetJobNum(); i++)
    {
        int ji = pGene[i];
        int ji_1 = pGene[i - 1];
        for (int k = 1; k < jobManager.jobsInfo[i].procNum; k++)
        {
            c[ji][k]=MAX(c[ji_1][k], c[ji][k-1])+ jobManager.jobsInfo[ji].procInfo[k].time;
        }
    }
    int jn = pGene[jobManager.GetJobNum() - 1];
    int m = jobManager.GetMachineNum() - 1;
    return c[jn][m];
}

void Population::MakeNewGeneration()
{
    UpdateFitness();
    Sort();

    for (int i = 0; i < MAX_CHROMOSOME_NUM; i++)
        chromosomes[i].Show();
    printf("Start to make new generation===============================\r\n");

    for (int i = 0; i < nIterNum; i++)
    {
        UpdateElitist();
        SelectNewGeneration();
        Crossover();
        Mutation();
        Sort();

        printf("min fitness: ");
        chromosomes[0].Show();
    }
    printf("Finished!!===================================\r\n");
    //for (int i = 0; i < MAX_CHROMOSOME_NUM; i++)
    //    genes[i].Show();
}


/*
����������������Ӧ��ѡ���µ�һ��.�����˾�Ӣ�������㷨������Ӧ����õĸ���ᱣ��������
*/
void Population::SelectNewGeneration()
{
    float sumOfFit = 0;
    int sz = chromosomes.size(), j = 0,i = 0;
    for (i = 0; i < sz; i++)
        sumOfFit += 1.0f/ chromosomes[i].GetFitness();

    float temp = 0;
    for (i = 0; i < sz; i++)
    {
        temp += 1.0f/ chromosomes[i].GetFitness();
        chromosomes[i].SetCFitness(temp / sumOfFit);
    }
    vector<Chromosome> newGen(sz);
    for (int i = 0; i < nElitistNum; i++)
        newGen[i] = chromosomes[i];

    for (i = nElitistNum; i < sz; i++)
    {
        float p = random.getFloat(0, 1.0f);
        if (p < chromosomes[0].GetCFitness())
            newGen[i] = chromosomes[0];
        else
        {
            for (int j = 0; j < sz-1; j++)
            {
                if (chromosomes[j].GetCFitness() <= p && p < chromosomes[j + 1].GetCFitness())
                {
                    newGen[i] = chromosomes[j+1];
                    break;
                }
            }
            if(j>=sz)
                newGen[i] = chromosomes[sz-1];
        }
    }
    chromosomes = newGen;
}

//ѡ������������е��㽻��
void Population::Crossover()
{
    int first = 0;
    Chromosome *father = NULL, *mother = NULL;

    for (int i = nElitistNum; i < chromosomes.size(); i++)
    {
        float p = random.getFloat(0, 1.0f);
        if (p < fCross)
        {
            first++;
            if ((first & 1) == 1)
            {
                father = &chromosomes[i];
            }
            else
            {
                mother = &chromosomes[i];
                father->CrossOver(*mother);
                mother->SetFitness(CalFitness(jobs, *mother));
                father->SetFitness(CalFitness(jobs, *father));
            }
        }
    }
}

void Population::Mutation()
{
    for (int i = nElitistNum; i < chromosomes.size(); i++)
    {
        for (int j = 0; j < chromosomes[i].GetLength()/2; j++)
        {
            float p = random.getFloat(0, 1.0f);
            if (p < fMutation)
            {
                chromosomes[i].Mutation(1);
                chromosomes[i].SetFitness(CalFitness(jobs, chromosomes[i]));
            }
        }
    }
}

int main() 
{
    Population ga(100);
    ga.Init();
    ga.MakeNewGeneration();

    //ga.UpdateFitness();
    //ga.Show();
}


