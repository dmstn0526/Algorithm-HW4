#include<stdio.h>
#include<stdlib.h> //srand(), rand()함수를 사용하기 위한 헤더파일
#include<time.h> //time() 사용을 위한 헤더파일

#define MD 9 //Minimum degree 5,7,9 여기서 변경

//B-tree-Node class
class B_treeN
{
    int* data;  // node에 들어갈 자료의 배열
    int md;      // Minimum degree (defines the range for number of keys)
    B_treeN** ChildPtr; //노드 포인트 배열
    int n;     // 자료의 개수
    bool leaf; // leaf node인지 확인
public:
    B_treeN(int _t, bool _leaf);   // Constructor
    void insertNF(int k); //이 node로 루트된 하위 트리에 새 키를 삽입하는 함수
    
    /*node의 자식 노드를 분할하는 함수
    i는 자식 배열 C[]에서 y의 인덱스*/
    void splitC_node(int i, B_treeN* y);

    void traverse(); //이 node로 루트된 하위 tree의 모든 노드를 traverse하는 함수
    
    /*이 node로 루트된 하위 tree에서 data를 검색하는 함수
    k가 없으면 NULL을 반환*/
    B_treeN* search(int k); 

    //Btree기능에서 이 클래스의 개인 구성원에게 액세스할 수 있도록 만든 친구
    friend class B_tree;
};

//B-tree
class B_tree
{
    B_treeN* root; //root node 포인트
    int t;  // Minimum degree
public:
    // Constructor (Initializes tree as empty)
    B_tree(int _t)
    {
        root = NULL;  t = _t;
    }
    //traverse하는 tree 함수
    void traverse()
    {
        if (root != NULL) root->traverse();
    }
    //이 트리에서 키를 검색하는 기능
    B_treeN* search(int k)
    {
        return (root == NULL) ? NULL : root->search(k);
    }
    //이 B-Tree에 새 키를 삽입하는 기본 기능
    void insert(int k);
};

// Constructor for BTreeNode class
B_treeN::B_treeN(int t1, bool leaf1)
{
    //지정된 minimum degree 및 leaf 속성 복사
    md = t1;
    leaf = leaf1;
    //가능한 최대 data 및 하위 포인터 수에 대해 메모리 할당
    data = new int[2 * md - 1];
    ChildPtr = new B_treeN * [2 * md];
    n = 0; // data 수를 0으로 초기화
}

//현재 트리를 보여주는 순회함수
void B_treeN::traverse()
{
    int i;
    //n개의 키와 n+1개의 자식이 있고, n개의 키와 첫 번째 n개의 자식이 있다.
    //첫번째 자식 node부터 가장 마지막 node까지 밑으로 내려간다.
    for (i = 0; i < n; i++)
    {
        if (leaf == false) //leaf node가 아니라면 더 밑으로 내려감
            ChildPtr[i]->traverse();
        printf("%d ", data[i]); //data 출력
    }
    if (leaf == false) //leaf node가 아니라면 더 밑으로 내려간다.
        ChildPtr[i]->traverse();
}

//k가 존재하는 데이터 배열의 데이터를 정렬하는 함수
B_treeN* B_treeN::search(int k)
{
    //k보다 크거나 같은 첫 번째 키 찾기
    int i = 0;
    while (i < n && k > data[i])
        i++;
    //찾은 키가 k와 같으면 이 노드를 반환합니다.
    if (data[i] == k)
        return this;
    //여기서 키를 찾을 수 없고 리프 노드인 경우
    if (leaf == true)
        return NULL;
    return ChildPtr[i]->search(k); //해당 하위 항목으로 이동합니다.
}

//이 B-Tree에 새 키를 삽입하는 기본 기능
void B_tree::insert(int k)
{
    // tree 가 비어있을때
    if (root == NULL)
    {
        //루트에 메모리 할당
        root = new B_treeN(t, true); 
        root->data[0] = k;  // Insert data
        root->n = 1;  // root의 data 수 업데이트
    }
    else // tree 비어 있지 않은 경우
    {
        // root가 가득 차면 tree grows in height
        if (root->n == 2 * t - 1)
        {
            B_treeN* s = new B_treeN(t, false); //새 root에 메모리 할당
            s->ChildPtr[0] = root; //새 root의 자식으로서 이전 root를 만든다
            s->splitC_node(0, root); //이전 root를 분할하고 data 1개를 새 root로 이동합니다.
            //새 root가 자식을 2개 가짐. 둘 중 어느 자식이 새 data를 가질 지 결정
            int i = 0;
            if (s->data[0] < k)
                i++;
            s->ChildPtr[i]->insertNF(k);
            
            root = s; // root 변경
        }
        else  // root가 가득 차지 않으면 root에 대해 insertNonFull을 호출
            root->insertNF(k);
    }
}

/*이 Node에 새 data를 삽입하는 함수
이 함수를 호출할 때 Node가 가득 차지 않아야한다고 가정한다*/
void B_treeN::insertNF(int k)
{
    //index를 맨 오른쪽 요소의 index로 초기화
    int i = n - 1;
    //leaf node인 경우
    if (leaf == true)
    {
        //삽입할 새 data의 위치를 찾고, 더 큰 data를 모두 앞의 한 곳으로 이동
        while (i >= 0 && data[i] > k)
        {
            data[i + 1] = data[i];
            i--;
        }
        // 발견된 위치에 새 data 삽입
        data[i + 1] = k;
        n = n + 1;
    }
    else // 이 node가 leaf node가 아닌 경우
    {
        // 새 data를 가질 자식을 찾는다
        while (i >= 0 && data[i] > k)
            i--;
        //찾은 자식이 차있는지 확인
        if (ChildPtr[i + 1]->n == 2 * md - 1)
        {
            // child가 꽉 찬 경우, 분할한다
            splitC_node(i + 1, ChildPtr[i + 1]);

            /*분할 후, C[i]의 가운데 data가 위로 올라가서 C[i]가 둘로 나뉜다. 
            둘 중 누가 새 data를 가질지 확인*/
            if (data[i + 1] < k)
                i++;
        }
        ChildPtr[i + 1]->insertNF(k);
    }
}

/*이 node의 자식 node를 분할하는 함수
이 함수를 호출할 때는 y가 가득 차야한다*/
void B_treeN::splitC_node(int i, B_treeN* y)
{
    //y의 (t-1) data를 저장할 새 node를 생성
    B_treeN* z = new B_treeN(y->md, y->leaf);
    z->n = md - 1;
    //y의 마지막 (t-1) data를 z로 복사
    for (int j = 0; j < md - 1; j++)
        z->data[j] = y->data[j + md];
    //y의 마지막 하위 항목을 z로 복사
    if (y->leaf == false)
    {
        for (int j = 0; j < md; j++)
            z->ChildPtr[j] = y->ChildPtr[j + md];
    }
    // y에서 data 수 감소
    y->n = md - 1;
    //이 node에서 새 하위 node를 가질 예정-> 새 하위노드 공간 생성
    for (int j = n; j >= i + 1; j--)
        ChildPtr[j + 1] = ChildPtr[j];
    ChildPtr[i + 1] = z; //이 node에서 새 child 연결
    
    /*y의 data가 이 node로 이동 
    새 data의 위치를 찾고 큰 data를 모두 한 칸 앞으로 이동*/
    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];
    data[i] = y->data[md - 1]; //y의 중간 data를 이 node에 복사
    n = n + 1; //이 node의 data 개수 증가
}


int main()
{
    B_tree t(MD); // Minimum degree MD로 처음에 define
    int n = 0;
    for (int i = 0; i < 1000; i++) { //1000개를 삽입
        n = rand() % 1000; //0 - 9999 사이의 데이터
        t.insert(n);
    }
    printf("생성된 트리 \n");
    t.traverse();

    printf("\n");
    //실행 시간을 나타내는 코드
    int TESTCASE = 1000;
    int TIME = 0;
    //Timer on
    clock_t start = clock();
    //실행 코드
    for (int tc = 1; tc <= TESTCASE; tc++) {
        int a = 1;
        for (int v = 0; v < 100000; v++) {
            a += v;
            a %= 100;
        }
    }
    //Timer off
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %d ms\n", TIME); //ms단위로 출력

    return 0;
}