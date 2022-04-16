/**
 * File: KDTree.h
 * Author: (your name here)
 * ------------------------
 * TODO: fill in this file comment!
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.
using namespace std;

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();
    
    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();
    
    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& other);
    KDTree& operator=(const KDTree& rhs);

    // KDTree(KDTree&& rhs);
    // KDTree& operator=(KDTree&& rhs);
    // Usage: KDTree<3, int> one = std::move(two);
    // Usage: one = std::move(two);
    // -----------------------------------------------------
    // Moves the contents of another KDTree into this one,
    // leaving the other KDTree in a valid but undefined state.
    KDTree(KDTree&& other);
    KDTree& operator=(KDTree&& rhs);
    
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;
    
    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;
    
    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;
    
    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);
    
    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
    
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
    
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;

private:
    struct kdtree{
        Point<N> key;
        ElemType value;
        kdtree* left;
        kdtree* right;
    };
    void freetree(kdtree* );
    kdtree* findnode(const Point<N>& pt) const {
        kdtree* node=root;
        size_t level=0;
        while(node!=nullptr&&node->key!=pt){

            if(pt[level%N]<node->key[level%N]){
                node=node->left;
            }else{
                node=node->right;
            }
            level++;
        }
        if(node==nullptr){
            return nullptr;
        }
        return node;
    };
    struct kdtree* root;
    size_t _size;
};

/** KDTree class implementation details */

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree():_size(0) ,root(nullptr){};

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    freetree(this->root);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::freetree(kdtree* node){
    if(node!=nullptr){
        freetree(node->left);
        freetree(node->right);
        delete node;
    }
};

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return root->key.size();
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const{
    return _size;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const{
    return _size==0;
}

template<size_t N,typename ElemType>
void KDTree<N,ElemType>::insert (const Point<N>& pt,const ElemType& value){
    if(contains(pt)){
        findnode(pt)->value=value;
    }else{
        kdtree* newnode = new kdtree{pt,value,nullptr,nullptr};
        if(root==nullptr){
            root=newnode;
            _size++;
        }else{
                kdtree* curr=root;
                size_t level=0;
                while(curr!=nullptr){
                    if((newnode->key)[level%N]<(curr->key)[level%N]){
                        if(curr->left==nullptr){
                            curr->left=newnode;
                            break;
                        }
                        curr=curr->left;
                        level++;
                    }else{
                        if(curr->right==nullptr){
                            curr->right=newnode;
                            break;
                        }
                        curr=curr->right;
                        level++;
                    }
                }
                _size++;
            }
    }
}

template<size_t N,typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const{
    return findnode(pt)!=nullptr;
}

template<size_t N,typename ElemType>
ElemType& KDTree<N, ElemType>::operator[] (const Point<N>& pt){
    if(findnode(pt)!=nullptr){
        return findnode(pt)->value;
    }else{
        ElemType value{};
        insert(pt,value);
        return findnode(pt)->value;
    }
};

template<size_t N,typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt){
    if(findnode(pt)==nullptr){
         throw out_of_range("out of range");
    }
    return findnode(pt)->value;
}

template<size_t N,typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const{
    if(findnode(pt)==nullptr){
         throw out_of_range("out of range");
    }
    return findnode(pt)->value;
}

#endif // KDTREE_INCLUDED
