#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

struct Flower {
    string name, content;
    int frequency;
};
vector<Flower> flowers;  // 按频率排序的数组

const int hash_size = 101;
struct Hash_Node {
    string name, content;
    int frequency;
    Hash_Node *next;
} *hash_table[hash_size];  // 字符串哈希表

struct Tree_Node {
    int id;
    string name, content;
    int frequency;
    Tree_Node *left, *right;
} *tree;
const int N = 20;
int cost[N][N];  // cost[i][j]表示id在[i, j]的花形成的子树的搜索代价
int sum[N][N];  // sum[i][j]表示id在[i, j]的花形成的子树的频率总和
int root[N][N];  // root[i][j]表示id在[i, j]的花形成的子树的根结点id

// 数组
bool get_flowers(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {  // 按行读取
            string name, content;
            name = line.substr(0, line.find(' '));
            content = line.substr(line.find(' ') + 1);
            flowers.push_back({name, content, 0});
        }
        file.close();
        return true;
    }
    return false;
}

bool get_frequencys(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        string n;
        int i = 0;
        while (getline(file, n))  // 按行读取
            flowers[i++].frequency = stoi(n);
        file.close();
        return true;
    }
    return false;
}

bool cmp(Flower a, Flower b) {
    return a.frequency > b.frequency;
}

string search_array(string name) {  // o(n)
    for (auto flower: flowers)
        if (flower.name == name) return flower.name + ":" + flower.content;
    return "sorry";
}

// 哈希表
int string_hashing(string s) { //牡丹 = 1234 % hash_size
    unsigned long long n;  // abc = a * p^2 + b * p^1 + c *p^0 = 12345556
    int p = 13331;
    for (int i = 0; i < s.size(); i++)
            n = n * p + s[i];
    return n % hash_size;
}

void init_hash() {
    for (auto flower: flowers) {
        Hash_Node *node = new Hash_Node();
        node -> name = flower.name;
        node -> content = flower.content;
        node -> frequency = flower.frequency;
        int key = string_hashing(node -> name);
        if (!hash_table[key]) hash_table[key] = node;
        else {
            node -> next = hash_table[key];
            hash_table[key] = node;
        }
    }
}

string search_hash(string name) {  // o(1)
    int key = string_hashing(name);
    Hash_Node *p = hash_table[key];
    while (p) {
        if (p -> name == name)
            return p -> name + ":" + p -> content;
        p = p -> next;
    }
    return "sorry";
}

// optimal BST
void count_root(int n) {
    for (int len = 1; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            if (i == j) {
                cost[i][j] = flowers[i].frequency;
                sum[i][j] = flowers[i].frequency;
                root[i][j] = i;
            } else {
                sum[i][j] = sum[i][j - 1] + flowers[j].frequency;
                for (int k = i; k <= j; k++) {
                    int t = sum[i][j];
                    if (i < k) t += cost[i][k - 1];
                    if (k < j) t += cost[k + 1][j];
                    if (t < cost[i][j]) {
                        cost[i][j] = t;
                        root[i][j] = k;
                    }
                }
            }
        }
    }
}

Tree_Node* build_tree(int l, int r) {  // 构建[i, j]的子树
    if (l > r) return nullptr;
    Tree_Node *node = new Tree_Node();
    int id = root[l][r];
    node -> id = id;
    node -> frequency = flowers[id].frequency;
    node -> name = flowers[id].name;
    node -> content = flowers[id].content;
    node -> left = build_tree(l, id - 1);
    node -> right = build_tree(id + 1, r);
    return node;
}

void init_tree() {
    // 计算所有的root
    memset(cost, 0x3f, sizeof cost);
    memset(root, -1, sizeof root);
    count_root(flowers.size());
    // 根据root数组构建tree
    tree = build_tree(0, flowers.size() - 1);
}

string search_tree(int id) {
    Tree_Node *p = tree;
    while (p && p -> id != id) {
        if (p -> id < id) p = p -> right;
        else p = p -> left;
    }
    if (p && p -> id == id)
        return p -> name + ":" + p -> content;
    return "sorry";
}

int main() {
    // 数组
    get_flowers("flower-info.txt");
    get_frequencys("frequency.txt");
    sort(flowers.begin(), flowers.end(), cmp);

    // 哈希表
    init_hash();
    while (true) {
        string name;
        cin >> name;
        cout << search_hash(name) << endl;
    }

    // 树
    init_tree();

    return 0;
}
