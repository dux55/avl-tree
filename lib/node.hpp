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
    Node(Node& node_param){
	this->val = node_param.val;
	this->height = node_param.height;
        this->left = node_param.left;
        this->right = node_param.right;
	this->parent = node_param.parent;
    };
    Node(T val_param){
        this->val = val_param;
	this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
	this->parent = nullptr;
    }
};

#endif
