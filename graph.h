struct Node;
struct Edge;

using namespace std;

struct Graph
{
    int vertexNum, edgeNum;
    int walkLen, walkNum;
    int p, q;
    bool isDirected, isWeighted;
    Node *vertex;
    double stdValue, ratio;

    //���ݲ����������Բ����붥�㼯�ռ�
    Graph(char** args);

    // void *operator new(size_t size);

    //�����ļ�����ȡ�߼�����ͼ���ڲ���ʾ
    void initialGraph(char* filename);

    //Ԥ�������㶥��ͱߵ�ת�Ƹ��ʺ����ɱ�����
    void preprocess();

    void simulateWalk();

    int *randomWalk(int src, int len);

    //�����ķ���ֵ������Դ����ĵ� i ���ڽӵ�
    int aliasSample(float *transProbTable, int *aliasTable, int len);

    void showGraph();

    //����ÿ�������inValue
    void setInvalue();

    //�������õı������Ƚ����ж����inValueֵ����û�׼inValue
    void getStdInvalue();

    void countMemLoc();
};
