#ifndef NODE_HPP
#define NODE_HPP

#include <cstdint>

template <class T>
struct Node{
    T val;
    int64_t height;
    Node* left;
    Node* right;
    Node* parent;

    Node(){
        this->val = {};
	this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
	this->parent = nullptr;
    }
    Node(Node& node){
	this->val = node.val;
	this->height = node.height;
        this->left = node.left;
        this->right = node.right;
	this->parent = node.parent;
    };
    Node(T val){
        this->val = val;
	this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
	this->parent = nullptr;
    }
};

#endif
