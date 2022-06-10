#include<stdio.h>
#include<stdlib.h> //srand(), rand()�Լ��� ����ϱ� ���� �������
#include<time.h> //time() ����� ���� �������

#define MD 9 //Minimum degree 5,7,9 ���⼭ ����

//B-tree-Node class
class B_treeN
{
    int* data;  // node�� �� �ڷ��� �迭
    int md;      // Minimum degree (defines the range for number of keys)
    B_treeN** ChildPtr; //��� ����Ʈ �迭
    int n;     // �ڷ��� ����
    bool leaf; // leaf node���� Ȯ��
public:
    B_treeN(int _t, bool _leaf);   // Constructor
    void insertNF(int k); //�� node�� ��Ʈ�� ���� Ʈ���� �� Ű�� �����ϴ� �Լ�
    
    /*node�� �ڽ� ��带 �����ϴ� �Լ�
    i�� �ڽ� �迭 C[]���� y�� �ε���*/
    void splitC_node(int i, B_treeN* y);

    void traverse(); //�� node�� ��Ʈ�� ���� tree�� ��� ��带 traverse�ϴ� �Լ�
    
    /*�� node�� ��Ʈ�� ���� tree���� data�� �˻��ϴ� �Լ�
    k�� ������ NULL�� ��ȯ*/
    B_treeN* search(int k); 

    //Btree��ɿ��� �� Ŭ������ ���� ���������� �׼����� �� �ֵ��� ���� ģ��
    friend class B_tree;
};

//B-tree
class B_tree
{
    B_treeN* root; //root node ����Ʈ
    int t;  // Minimum degree
public:
    // Constructor (Initializes tree as empty)
    B_tree(int _t)
    {
        root = NULL;  t = _t;
    }
    //traverse�ϴ� tree �Լ�
    void traverse()
    {
        if (root != NULL) root->traverse();
    }
    //�� Ʈ������ Ű�� �˻��ϴ� ���
    B_treeN* search(int k)
    {
        return (root == NULL) ? NULL : root->search(k);
    }
    //�� B-Tree�� �� Ű�� �����ϴ� �⺻ ���
    void insert(int k);
};

// Constructor for BTreeNode class
B_treeN::B_treeN(int t1, bool leaf1)
{
    //������ minimum degree �� leaf �Ӽ� ����
    md = t1;
    leaf = leaf1;
    //������ �ִ� data �� ���� ������ ���� ���� �޸� �Ҵ�
    data = new int[2 * md - 1];
    ChildPtr = new B_treeN * [2 * md];
    n = 0; // data ���� 0���� �ʱ�ȭ
}

//���� Ʈ���� �����ִ� ��ȸ�Լ�
void B_treeN::traverse()
{
    int i;
    //n���� Ű�� n+1���� �ڽ��� �ְ�, n���� Ű�� ù ��° n���� �ڽ��� �ִ�.
    //ù��° �ڽ� node���� ���� ������ node���� ������ ��������.
    for (i = 0; i < n; i++)
    {
        if (leaf == false) //leaf node�� �ƴ϶�� �� ������ ������
            ChildPtr[i]->traverse();
        printf("%d ", data[i]); //data ���
    }
    if (leaf == false) //leaf node�� �ƴ϶�� �� ������ ��������.
        ChildPtr[i]->traverse();
}

//k�� �����ϴ� ������ �迭�� �����͸� �����ϴ� �Լ�
B_treeN* B_treeN::search(int k)
{
    //k���� ũ�ų� ���� ù ��° Ű ã��
    int i = 0;
    while (i < n && k > data[i])
        i++;
    //ã�� Ű�� k�� ������ �� ��带 ��ȯ�մϴ�.
    if (data[i] == k)
        return this;
    //���⼭ Ű�� ã�� �� ���� ���� ����� ���
    if (leaf == true)
        return NULL;
    return ChildPtr[i]->search(k); //�ش� ���� �׸����� �̵��մϴ�.
}

//�� B-Tree�� �� Ű�� �����ϴ� �⺻ ���
void B_tree::insert(int k)
{
    // tree �� ���������
    if (root == NULL)
    {
        //��Ʈ�� �޸� �Ҵ�
        root = new B_treeN(t, true); 
        root->data[0] = k;  // Insert data
        root->n = 1;  // root�� data �� ������Ʈ
    }
    else // tree ��� ���� ���� ���
    {
        // root�� ���� ���� tree grows in height
        if (root->n == 2 * t - 1)
        {
            B_treeN* s = new B_treeN(t, false); //�� root�� �޸� �Ҵ�
            s->ChildPtr[0] = root; //�� root�� �ڽ����μ� ���� root�� �����
            s->splitC_node(0, root); //���� root�� �����ϰ� data 1���� �� root�� �̵��մϴ�.
            //�� root�� �ڽ��� 2�� ����. �� �� ��� �ڽ��� �� data�� ���� �� ����
            int i = 0;
            if (s->data[0] < k)
                i++;
            s->ChildPtr[i]->insertNF(k);
            
            root = s; // root ����
        }
        else  // root�� ���� ���� ������ root�� ���� insertNonFull�� ȣ��
            root->insertNF(k);
    }
}

/*�� Node�� �� data�� �����ϴ� �Լ�
�� �Լ��� ȣ���� �� Node�� ���� ���� �ʾƾ��Ѵٰ� �����Ѵ�*/
void B_treeN::insertNF(int k)
{
    //index�� �� ������ ����� index�� �ʱ�ȭ
    int i = n - 1;
    //leaf node�� ���
    if (leaf == true)
    {
        //������ �� data�� ��ġ�� ã��, �� ū data�� ��� ���� �� ������ �̵�
        while (i >= 0 && data[i] > k)
        {
            data[i + 1] = data[i];
            i--;
        }
        // �߰ߵ� ��ġ�� �� data ����
        data[i + 1] = k;
        n = n + 1;
    }
    else // �� node�� leaf node�� �ƴ� ���
    {
        // �� data�� ���� �ڽ��� ã�´�
        while (i >= 0 && data[i] > k)
            i--;
        //ã�� �ڽ��� ���ִ��� Ȯ��
        if (ChildPtr[i + 1]->n == 2 * md - 1)
        {
            // child�� �� �� ���, �����Ѵ�
            splitC_node(i + 1, ChildPtr[i + 1]);

            /*���� ��, C[i]�� ��� data�� ���� �ö󰡼� C[i]�� �ѷ� ������. 
            �� �� ���� �� data�� ������ Ȯ��*/
            if (data[i + 1] < k)
                i++;
        }
        ChildPtr[i + 1]->insertNF(k);
    }
}

/*�� node�� �ڽ� node�� �����ϴ� �Լ�
�� �Լ��� ȣ���� ���� y�� ���� �����Ѵ�*/
void B_treeN::splitC_node(int i, B_treeN* y)
{
    //y�� (t-1) data�� ������ �� node�� ����
    B_treeN* z = new B_treeN(y->md, y->leaf);
    z->n = md - 1;
    //y�� ������ (t-1) data�� z�� ����
    for (int j = 0; j < md - 1; j++)
        z->data[j] = y->data[j + md];
    //y�� ������ ���� �׸��� z�� ����
    if (y->leaf == false)
    {
        for (int j = 0; j < md; j++)
            z->ChildPtr[j] = y->ChildPtr[j + md];
    }
    // y���� data �� ����
    y->n = md - 1;
    //�� node���� �� ���� node�� ���� ����-> �� ������� ���� ����
    for (int j = n; j >= i + 1; j--)
        ChildPtr[j + 1] = ChildPtr[j];
    ChildPtr[i + 1] = z; //�� node���� �� child ����
    
    /*y�� data�� �� node�� �̵� 
    �� data�� ��ġ�� ã�� ū data�� ��� �� ĭ ������ �̵�*/
    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];
    data[i] = y->data[md - 1]; //y�� �߰� data�� �� node�� ����
    n = n + 1; //�� node�� data ���� ����
}


int main()
{
    B_tree t(MD); // Minimum degree MD�� ó���� define
    int n = 0;
    for (int i = 0; i < 1000; i++) { //1000���� ����
        n = rand() % 1000; //0 - 9999 ������ ������
        t.insert(n);
    }
    printf("������ Ʈ�� \n");
    t.traverse();

    printf("\n");
    //���� �ð��� ��Ÿ���� �ڵ�
    int TESTCASE = 1000;
    int TIME = 0;
    //Timer on
    clock_t start = clock();
    //���� �ڵ�
    for (int tc = 1; tc <= TESTCASE; tc++) {
        int a = 1;
        for (int v = 0; v < 100000; v++) {
            a += v;
            a %= 100;
        }
    }
    //Timer off
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %d ms\n", TIME); //ms������ ���

    return 0;
}