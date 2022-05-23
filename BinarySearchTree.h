#pragma once
#include <cmath>
#include <functional> // std::less
#include <iostream>
#include <queue> // std::queue
#include <utility> // std::pair

using namespace std;

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
  public:
    using key_type        = K;
    using value_type      = V;
    using key_compare     = Comparator;
    using pair            = std::pair<key_type, value_type>;
    using pointer         = pair*;
    using const_pointer   = const pair*;
    using reference       = pair&;
    using const_reference = const pair&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

  private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const_reference theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ theElement }, left{ lt }, right{ rt } { }
        
        BinaryNode( pair && theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    using node           = BinaryNode;
    using node_ptr       = node*;
    using const_node_ptr = const node*;

    node_ptr _root;
    size_type _size;
    key_compare comp;

  public:
    BinarySearchTree() {
        _size = 0;
        _root = nullptr;
    }
    BinarySearchTree( const BinarySearchTree & rhs ) {
        _size = rhs.size();
        _root = clone(rhs._root);
    }
    BinarySearchTree( BinarySearchTree && rhs ) {
        _size = rhs.size();
        rhs._size = 0;
        _root = rhs._root;
        rhs._root = nullptr;
    }
    ~BinarySearchTree() {
        clear();
    }

    const_reference min() const { return min( _root )->element; }
    const_reference max() const { return max( _root )->element; }
    const_reference root() const {
        return _root->element;
    }
    bool contains( const key_type & x ) const { return contains( x, _root ); }
    value_type & find( const key_type & key ) { return find( key, _root )->element.second; }
    const value_type & find( const key_type & key ) const { return find( key, _root )->element.second; }
    bool empty() const {
        if (!_size) {
            return true;
        }
        return false;
    }
    size_type size() const {
        return _size;
    }

    void clear() {
        clear( _root );
        _size = 0;
    }
    void insert( const_reference x ) { insert( x, _root ); }
    void insert( pair && x ) { insert( std::move( x ), _root ); }
    void erase( const key_type & x ) { erase(x, _root); }

    BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
        if (rhs._root == _root) {
            return *this;
        }
        clear();
        _size = rhs.size();
        _root = clone(rhs._root);
        return *this;
    }
    BinarySearchTree & operator=( BinarySearchTree && rhs ) {
        if (rhs._root == _root) {
            return *this;
        }
        clear();
        _size = rhs.size();
        rhs._size = 0;
        _root = rhs._root;
        rhs._root = nullptr;
        return *this;
    }

  private:
    void insert( const_reference x, node_ptr & t ) {
        if (t == nullptr ) {
            t = new node(x,nullptr,nullptr);
            _size++;
            return;
        }
        else if (t->element.first == x.first) {
            t->element = x;
            return;
        }

        if (comp(x.first, t->element.first)) {
            insert(x, t->left);
        }
        else {
            insert(x, t->right);
        }
    }
    void insert( pair && x, node_ptr & t ) {
        if (t == nullptr) {
            t = new node(std::move(x),nullptr,nullptr);
            _size++;
            return;
        }
        else if (t->element.first == x.first) {
            t->element = std::move(x);
            return;
        }

        if (comp(x.first, t->element.first)) {
            insert(std::move(x), t->left);
        }
        else {
            insert(std::move(x), t->right);
        }
    }
    BinaryNode* removemin (BinaryNode * n) {
        if (n->left != nullptr) {
            n->left = removemin(n->left);
        }
        else {
            _size--;
            BinaryNode* t = n;
            n = n->right;
            delete t;
        }
        return n;
    }
    void erase( const key_type & x, node_ptr & t ) {
        if (t == nullptr) { // base case
            return;
        }
        if (x > t->element.first) { // recursive case 1, greater than x
            erase(x, t->right);
        }
        else if (x < t->element.first) { //recursive case 2, less than x
            erase(x,t->left);
        }
        else if (t->left != nullptr && t->right != nullptr) { //recursive case 3, both left and right
            t->element = min(t->right)->element;
            t->right = removemin(t->right);
        }
        else { // recursive case 4, one child
            _size--;
            BinaryNode * n = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete n;
        }
    }

    const_node_ptr min( const_node_ptr t ) const {
        if (t->left == nullptr) {
            return t;
        }
        return min(t->left);

    }
    const_node_ptr max( const_node_ptr t ) const {
        if (t->right == nullptr) {
            return t;
        }
        return max(t->right);

    }

    bool contains( const key_type & x, const_node_ptr t ) const {
        if (t == nullptr) {
            return false;
        }

        if (x == t->element.first) {
            return true;
        }
        if (comp(x, t->element.first)) {
            return contains(x, t->left);
        }
        else if (!comp(x, t->element.first)) {
            return contains(x, t->right);
        }
        return false;
    }
    node_ptr find( const key_type & key, node_ptr t ) {
        if (key == t->element.first) {
            return t;
        }
        if (key < t->element.first) {
            return find(key, t->left);
        }
        else if (key > t->element.first) {
            return find(key, t->right);
        }
        else {
            return nullptr;
        }
    }
    const_node_ptr find( const key_type & key, const_node_ptr t ) const {
        if (key == t->element.first) {
            return t;
        }
        if (key < t->element.first) {
            return find(key, t->left);
        }
        else if (key > t->element.first) {
            return find(key, t->right);
        }
        else {
            return nullptr;
        }
    }

    void clear( node_ptr & t ) {
        if (t == nullptr) {
            return;
        }
        clear(t->left);
        clear(t->right);
        delete t;
        t = nullptr;
    }
    node_ptr clone ( const_node_ptr t ) const {
        if (t == nullptr) {
            return nullptr;
        }
        node* mynode = new node(t->element, clone(t->left), clone(t->right));
        return mynode; // new node(t->element, clone(t->left), clone(t->right));
    }

  public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend std::ostream& printNode(std::ostream& o, const typename BinarySearchTree<KK, VV, CC>::node& bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
        std::ostream & out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev
    );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> & bst, 
        std::ostream & out
    );
};

template <typename KK, typename VV, typename CC>
std::ostream& printNode(std::ostream & o, const typename BinarySearchTree<KK, VV, CC>::node & bn) {
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}

template <typename KK, typename VV, typename CC>
void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out = std::cout ) {
    // using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    // using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;
    
    // TODO -- Guide in Instructions
    std::queue<node_ptr> my_queue;

    if (bst.empty()) {
        return;
    }

    my_queue.push(bst._root);
    int count = 0;
    bool cont = false;
    int level = 0;
    while (!my_queue.empty()) {
        // start with the root
        //running while the queue is not empty
        // every time you push, keep track of how many you've done
        //once this reaches a power of two, reset the counter
        // increment some integer about level
        node_ptr v = my_queue.front();
        my_queue.pop();
        if (v == nullptr) {
            count++;
            my_queue.push(nullptr);
            my_queue.push(nullptr);
            out << "null ";
        }
        else {
            count++;
            out << "(" << v->element.first << ", " << v->element.second << ")";
            if (v->left != nullptr) {
                my_queue.push(v->left);
                cont = true;
            }
            else {
                my_queue.push(nullptr);
            }
            if (v->right != nullptr) {
                my_queue.push(v->right);
                cont = true;
            }
            else {
                my_queue.push(nullptr);
            }
        }
        if (pow(2,level) == count) {
            count = 0;
            level++;
            out << "\n";
            if (cont == false) {
                break;
            }
            else {
                cont = false;
                continue;
            }
        }
    }
}

template <typename KK, typename VV, typename CC>
void printTree( const BinarySearchTree<KK, VV, CC> & bst, std::ostream & out = std::cout ) { printTree<KK, VV, CC>(bst._root, out ); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth = 0 ) {
    if (t != nullptr) {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
    std::ostream & out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr
) {
    if(node) {
        std::hash<KK> khash{};

        out << "\t" "node_" << (uint32_t) khash(node->element.first)
            << "[label=\"" << node->element.first 
            << " [" << node->element.second << "]\"];" << std::endl;
        
        if(prev)
            out << "\tnode_" << (uint32_t) khash(prev->element.first) <<  " -> ";
        else
            out << "\t";
        
        out << "node_" << (uint32_t) khash(node->element.first) << ";" << std::endl;
    
        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> & bst, 
    std::ostream & out = std::cout
) {
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
