#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// Warning ! 
// In MAX/MIN use KNUTH's order
// So if you want to create a tree with parameter t = 6, then MAX = t - 1 = 5 and MIN = t/2 - 1 = 2
#define MAX 26
#define MIN 12
#define MAX_WORD_LEN 257
#define MAX_ULLINT_LEN 20
#define MAX_PATH_LEN 100
const int STR_LEN = 257;

// Structure of pair (key / value)
struct Pair{
    uint64_t value;
    char key[STR_LEN];
};

struct Node {

    // Attributes
    bool leaf;
    int count_elems;
    Pair *elems;
    Node **children;

    // Constructor
    Node(){
        count_elems = 0;
        leaf = false;
        elems = new Pair[MAX];
        children = new Node *[MAX + 1];
        for (int i = 0; i < MAX+1; i++){
            children[i] = nullptr;
        }
    }
    ~Node()
    {
        delete[] elems;
        delete[] children;
    }
};

// bin_search
int l_bin_search(int left, int right, char * key, Node * data){
    while (left <= right) {
        int m = (left + right) / 2;
        if (strcmp(data->elems[m].key, key) == 0){
            return -1;
        }
        else if (strcmp(data->elems[m].key, key) < 0) {
            left = m + 1;
        }
        else {
            right = m - 1;
        }
    }
    return left;
}

// bin_search
int s_bin_search(int left, int right, char * key, Node * data){
    while (left <= right) {
        int m = (left + right) / 2;
        if (strcmp(data->elems[m].key, key) == 0){
            return m;
        }
        else if (strcmp(data->elems[m].key, key) < 0) {
            left = m + 1;
        }
        else {
            right = m - 1;
        }
    }
    return left;
}

// Find the key
int findKey(Node * node, char * key) {
    int idx = 0;
    while (idx < node->count_elems && strcmp(node->elems[idx].key, key) < 0)
        ++idx;
    return idx;
}

// Remove from the leaf
void removeFromLeaf(Node * node, int idx) {
    for (int i = idx + 1; i < node->count_elems; ++i)
        memcpy(&node->elems[i - 1], &node->elems[i], sizeof(Pair));

    node->count_elems--;

    return;
}

void deletion(Node * node, char * key);


Pair * getPredecessor(Node * node, int idx) {
    Node *cur = node->children[idx];
    while (!cur->leaf)
        cur = cur->children[cur->count_elems];

    return &cur->elems[cur->count_elems - 1];
}

Pair * getSuccessor(Node * node, int idx) {
    Node *cur = node->children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];

    return &cur->elems[0];
}

// Merge
void mergez(Node * node, int idx) {
    Node * child = node->children[idx];
    Node * sibling = node->children[idx + 1];

    // Merging
    memcpy(&child->elems[MIN], &node->elems[idx], sizeof(Pair));

    for (int i = 0; i < sibling->count_elems; ++i)
        memcpy(&child->elems[i + MIN + 1], &sibling->elems[i], sizeof(Pair));

    if (!child->leaf) {
        for (int i = 0; i <= sibling->count_elems; ++i)
            child->children[i + MIN + 1] = sibling->children[i];
    }

    // Key deleting in indeed node
    for (int i = idx + 1; i < node->count_elems; ++i)
        memcpy(&node->elems[i - 1], &node->elems[i], sizeof(Pair));


    for (int i = idx + 2; i <= node->count_elems; ++i)
        node->children[i - 1] = node->children[i];

    child->count_elems += sibling->count_elems + 1;
    node->count_elems--;

    delete sibling;
    return;
}

// Delete from non leaf node
void removeFromNonLeaf(Node * node, int idx) {
    char * k = strdup(node->elems[idx].key);

    if (node->children[idx]->count_elems >= MIN + 1) {
        Pair * pred = getPredecessor(node, idx);
        memcpy(&node->elems[idx], pred, sizeof(Pair));
        deletion(node->children[idx], strdup(pred->key));
    }

    else if (node->children[idx + 1]->count_elems >= MIN + 1) {
        Pair * succ = getSuccessor(node, idx);
        memcpy(&node->elems[idx], succ, sizeof(Pair));
        deletion(node->children[idx + 1], strdup(succ->key));
    }

    else {
        mergez(node, idx);
        deletion(node->children[idx], k);
    }
    delete[] k;
    return;
}

// Borrow from previous
void borrowFromPrev(Node * node, int idx) {
    Node *child = node->children[idx];
    Node *sibling = node->children[idx - 1];

    for (int i = child->count_elems - 1; i >= 0; --i) {
        memcpy(&child->elems[i + 1].key, &child->elems[i].key, sizeof(child->elems[i].key));
        child->elems[i + 1].value = child->elems[i].value;
    }
    if (!child->leaf) {
        for (int i = child->count_elems; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    memcpy(&child->elems[0].key, &node->elems[idx - 1].key, sizeof(node->elems[idx - 1].key));
    child->elems[0].value = node->elems[idx - 1].value;

    if (!child->leaf)
        child->children[0] = sibling->children[sibling->count_elems];

    memcpy(&node->elems[idx - 1].key, &sibling->elems[sibling->count_elems - 1].key, sizeof(sibling->elems[sibling->count_elems - 1].key));
    node->elems[idx - 1].value = sibling->elems[sibling->count_elems - 1].value;

    child->count_elems += 1;
    sibling->count_elems -= 1;

    return;
}

// Borrow from the next
void borrowFromNext(Node * node, int idx) {
    Node *child = node->children[idx];
    Node *sibling = node->children[idx + 1];

    memcpy(&child->elems[(child->count_elems)], &node->elems[idx], sizeof(Pair));

    if (!(child->leaf))
        child->children[(child->count_elems) + 1] = sibling->children[0];

    memcpy(&node->elems[idx], &sibling->elems[0], sizeof(Pair));

    for (int i = 1; i < sibling->count_elems; ++i)
        memcpy(&sibling->elems[i - 1], &sibling->elems[i], sizeof(Pair));

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->count_elems; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->count_elems += 1;
    sibling->count_elems -= 1;

    return;
}

void fill_(Node * node, int idx) {
    if (idx != 0 && node->children[idx - 1]->count_elems >= MIN + 1)
        borrowFromPrev(node, idx);

    else if (idx != node->count_elems && node->children[idx + 1]->count_elems >= MIN + 1)
        borrowFromNext(node, idx);

    else {
        if (idx != node->count_elems)
            mergez(node, idx);
        else
            mergez(node, idx - 1);
    }
    return;
}

bool f = true;

// Deletion operation
void deletion(Node * node, char * key) {
    int idx = findKey(node, key);

    if (idx < node->count_elems && strcmp(node->elems[idx].key, key) == 0) {
        if (node->leaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(node, idx);
    }
    else {
        if (node->leaf) {
            cout << "NoSuchWord" << endl;
            f = false;
            return;
        }

        bool flag = (idx == node->count_elems);

        if (node->children[idx]->count_elems == MIN)
            fill_(node, idx);

        if (flag && idx > node->count_elems)
            deletion(node->children[idx - 1], key);
        else
            deletion(node->children[idx], key);
    }
    return;
}


void saved(Node * node, ofstream & out){
    for (int i = 0; i < node->count_elems; i++){
        out.write(reinterpret_cast<const char*>(&node->elems[i].key), sizeof(node->elems[i].key));
        out.write(reinterpret_cast<const char*>(&node->elems[i].value), sizeof(node->elems[i].value));
    }
    if (!(node->leaf)){
        for (int i = 0; i <= node->count_elems; i++){
            saved(node->children[i], out);
        }
    }
}

void toLower(char * str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] - 'A' + 'a';
        }
    }
}

// Function to split our full_node and pass the middle element to parent
void splitChild(Node * full_node, Node * parent){

    Node * right_node = new Node();
    if (full_node->leaf){
        right_node->leaf = true;
    }

    // Index to move_up
    int move_up = (MAX - 1) / 2;

    // Copying the data to right_node
    memcpy(right_node->elems, full_node->elems + move_up + 1, sizeof(Pair) * move_up);
    right_node->count_elems = move_up;

    // Reducing number of keys in donor node
    full_node->count_elems -= move_up + 1;

    // Copying pointers if our full node is not a leaf
    if (!full_node->leaf){
        for (int i = 0; i <= move_up; i++){
            right_node->children[i] = full_node->children[MAX - move_up + i];
        }
    }


    // Adding right node to the parent node

    // Making space for right pointer of right_node + inserting the pointer to the right place
    int i_of_pointers = parent->count_elems;
    while (i_of_pointers >= 0 && parent->children[i_of_pointers] != full_node){
        parent->children[i_of_pointers + 1] = parent->children[i_of_pointers];
        i_of_pointers--;
    }
    // Making space and inserting a move_up element
    parent->children[i_of_pointers + 1] = right_node;

    int i = parent->count_elems;
    while (i != 0 && strcmp(parent->elems[i-1].key, full_node->elems[move_up].key) > 0){
        memcpy(&parent->elems[i], &parent->elems[i-1], sizeof(Pair));
        i--;
    }
    strncpy(parent->elems[i].key, full_node->elems[move_up].key, sizeof(full_node->elems[move_up].key));
    parent->elems[i].value = full_node->elems[move_up].value;
    parent->count_elems++;
}

// Inserting an element in a tree
void findNode(Node * active_node, Node * parent, char * key, uint64_t value, Node ** rooT, bool check){

    // If we are in a leaf
    if (active_node->leaf){

        int i = l_bin_search(0, active_node->count_elems - 1, key, active_node);
        if (i == -1){
            if (check){
                cout << "Exist" << endl;
            }
        }
        else{

            i = active_node->count_elems;
            while (i > 0 && strcmp(active_node->elems[i - 1].key, key) > 0) {
                memcpy(&active_node->elems[i], &active_node->elems[i - 1], sizeof(Pair));
                i--;
            }

            if (i > 0 && strcmp(active_node->elems[i - 1].key, key) != 0) {
                strncpy(active_node->elems[i].key, key, STR_LEN);
                active_node->elems[i].value = value;
                active_node->count_elems++;
                if (check) {
                    cout << "OK" << endl;
                }
            } else if (i == 0) {
                strncpy(active_node->elems[i].key, key, STR_LEN);
                active_node->elems[i].value = value;
                active_node->count_elems++;
                if (check) {
                    cout << "OK" << endl;
                }
            }
        }
    }
        // If we are in an internal node
    else{


        int i = l_bin_search(0, active_node->count_elems - 1, key, active_node);

        if (i == -1){
            cout << "Exist" << endl;
            return;
        }

        findNode(active_node->children[i], active_node, key, value, rooT, check);
    }


    // After insertion, we tend to check if the node is full
    if (active_node->count_elems == MAX){

        if (active_node == *rooT){
            Node * new_root = new Node();
            new_root->count_elems = 0;
            new_root->children[0] = active_node;
            *rooT = new_root;
            splitChild(active_node, new_root);
        }
        else{
            splitChild(active_node, parent);
        }
    }

}

// Insertion into the tree
void insert(Node ** node, char * key, unsigned long long int value, bool check){
    // If the tree is empty - create the tree and fill it with some data
    if (*node == nullptr){
        *node = new Node();
        strncpy((*node)->elems[0].key, key, STR_LEN);
        (*node)->elems[0].value = value;
        (*node)->count_elems = 1;
        (*node)->leaf = true;

        if (check){
            cout << "OK" << endl;
        }
    }

    else{
        findNode(*node, nullptr, key, value, node, check);
    }

}

// Load from binary
void load(Node ** tree, ifstream & in){
    char key[MAX_WORD_LEN + 1];
    unsigned long long int value;

    while (in.read(reinterpret_cast<char*>(&key), 257) && in.read(reinterpret_cast<char*>(&value), 8))
    {
        insert(tree, key, value, false);
    }

}

// Search function;
void search(Node * tree_root, char * key){
    if (tree_root != nullptr) {
        int i = s_bin_search(0, tree_root->count_elems - 1, key, tree_root);
        if (strcmp(tree_root->elems[i].key, key) == 0){
            cout << "OK: " << tree_root->elems[i].value << endl;
            return;
        }
        else if (tree_root->leaf) {
            cout << "NoSuchWord" << endl;
            return;
        } else {
            search(tree_root->children[i], key);
        }
    }
    else{
        cout << "NoSuchWord" << endl;
    }
}

// Delete Operation
void m_deletion(Node ** tree, char * key) {
    if (*tree == nullptr) {
        cout << "NoSuchWord" << endl;
        return;
    }

    deletion(*tree, key);

    if ((*tree)->count_elems == 0) {
        Node *tmp = (*tree);
        if ((*tree)->leaf)
            (*tree) = nullptr;
        else
            *tree = (*tree)->children[0];


        delete tmp;
    }

    if (f) {
        cout << "OK" << endl;
    }
    f = true;
    return;
}

void save(Node * root, ofstream & out){
    if (root != nullptr)
        saved(root, out);
}

void deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }
    else {
        if (!node->leaf) {
            for (int i = 0; i <= node->count_elems; i++) {
                if (node->children[i] != nullptr) {
                    deleteTree(node->children[i]);
                }
            }
        }
        else{
            delete node;
        }
    }

}

void print_btree(Node *node) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < node->count_elems; ++i) {
        if (!node->leaf) {
            print_btree(node->children[i]);
        }
        cout << node->elems[i].key << " ";
    }
    if (!node->leaf) {
        print_btree(node->children[node->count_elems]);
    }
}

int main(){

    char buffer[1 + 1 + MAX_WORD_LEN + 1 + MAX_ULLINT_LEN + 1];
    char key[MAX_WORD_LEN + 1];
    uint64_t value;
    char path[MAX_PATH_LEN];
    char cmd[5];

    Node * root = nullptr;

    ifstream fin(INPUT_FILE);
    while (fin >> buffer){

        if (strcmp(buffer, "+") == 0){
            fin >> key >> value;
            toLower(key);
            insert(&root, key, value, true);
        }

        else if (strcmp(buffer, "-") == 0){
            fin >> key;
            toLower(key);
            m_deletion(&root, key);
        }

        else if (strcmp(buffer, "!") == 0){
            fin >> cmd >> path;

            if (strcmp(cmd, "Save") == 0){
                //save
                ofstream out(path, ios::out | ios::binary);
                save(root, out);
                out.close();
                cout << "OK" << endl;
            }

            else if (strcmp(cmd, "Load") == 0){
                // read
                deleteTree(root);
                root = nullptr;
                ifstream in(path, ios::in | ios::binary);
                if(in.good() && in.peek() != EOF)
                    load(&root, in);
                cout << "OK" << endl;
            }
        }

        else{
            toLower(buffer);
            search(root, buffer);
        }
    }
    deleteTree(root);
    delete root;
    fin.close();
    return 0;
}
