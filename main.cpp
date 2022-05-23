#include <iostream>
#include <string>

#include "BinarySearchTree.h"
using namespace std;
int main() {
    // Give us a random tree each time
    srand(time(NULL));
    // 15 elements in the tree
    constexpr int TREE_SIZE = 15;

    BinarySearchTree<int, int> simpleTree;
    // std::string names[TREE_SIZE] {"Teresa", "Carlos", "Nkemdi", "Dante", "Alexander", "Evelyn", "Dillon", "Thomas", "Armando", "Mariel", "Furkan", "Anjali", "Jeremy", "Clayton", "Jessica"};
    // for (int i = 0; i < TREE_SIZE; ++i) {
    //     simpleTree.insert( { names[i], rand() % TREE_SIZE } );
    // }
    simpleTree.insert({7,30});
    simpleTree.insert({30,40});
    simpleTree.insert({1,12});
    simpleTree.insert({4,45});
    simpleTree.insert({17,0});
    simpleTree.insert({27,0});
    simpleTree.insert({3,30});
    simpleTree.insert({2,0});
    simpleTree.insert({5,0});
    // for (int i = 4; i < 0; i--) {
    //     simpleTree.insert({i, i % 10});
    // }
    // for (int i = 12; i < 8; i--) {
    //     simpleTree.insert({i, i % 10});
    // }
    // for (int i = 4; i < 7; i++) {
    //     simpleTree.insert({i, i % 10});
    // }
    cout << "hello world" << endl;
    // printLevelByLevel(simpleTree);
    printTree(simpleTree);
    cout << endl;
    cout << endl;

    simpleTree.erase(2);
    printTree(simpleTree);
}