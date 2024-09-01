#ifndef AVL_HPP
#define AVL_HPP

#include "lib/node.hpp"
#include <cstdint>
#include <cassert>
#include <vector>

template <typename T, typename U>
class avl{
    private:
        Node<std::pair<T, U>>* root;
        
	int64_t get_height(const Node<std::pair<T, U>>* curr){
	    if(curr == nullptr) return -1;
	    return curr->height;
	}
	void calc_height(Node<std::pair<T, U>>* curr){
	    curr->height = 1LL+std::max(get_height(curr->left), get_height(curr->right));
	}
	int64_t calc_balance_factor(const Node<std::pair<T, U>>* curr){
	    return get_height(curr->left)-get_height(curr->right);    	
	}
	
	Node<std::pair<T, U>>* find(const T key){
            Node<std::pair<T, U>>* prev = nullptr;
	    Node<std::pair<T, U>>* curr = this->root;
            while(curr != nullptr){
                prev = curr;
                if(key < curr->val.first) curr = curr->left;
                else curr = curr->right;
            }
        
            return prev;
        }

	void add_node(Node<std::pair<T, U>>* prev, Node<std::pair<T, U>>* curr){
	    if(prev == nullptr){
		this->root = curr;
		return;
	    }

	    if(curr->val.first < prev->val.first) prev->left = curr;
	    else prev->right = curr;
	    curr->parent = prev;
	}

	Node<std::pair<T, U>>* left_rotation(Node<std::pair<T, U>>* curr){
	    Node<std::pair<T, U>>* new_root = curr->right;
	    Node<std::pair<T, U>>* left = new_root->left;

	    new_root->left = curr;
	    curr->right = left;
	    
	    new_root->parent = curr->parent;
	    curr->parent = new_root;
	    if(left != nullptr) left->parent = curr;

	    calc_height(new_root);
	    calc_height(curr);

	    return new_root;
	}
	Node<std::pair<T, U>>* right_rotation(Node<std::pair<T, U>>* curr){
	    Node<std::pair<T, U>>* new_root = curr->left;
	    Node<std::pair<T, U>>* right = new_root->right;

	    new_root->right = curr;
	    curr->left = right;

	    new_root->parent = curr->parent;
	    curr->parent = new_root;
	    if(right != nullptr) right->parent = curr;

	    calc_height(new_root);
	    calc_height(curr);
	    
	    return new_root;
	}

	Node<std::pair<T, U>>* begin(){
	    if(this->root == nullptr) return nullptr;
	    
	    Node<std::pair<T, U>>* curr = this->root;
	    while(curr->left != nullptr) curr = curr->left;

	    return curr;
	}
	Node<std::pair<T, U>>* end(){
	    if(this->root == nullptr) return nullptr;
	    
	    Node<std::pair<T, U>>* curr = this->root;
	    while(curr->right != nullptr) curr = curr->right;

	    return curr;
	}

	Node<std::pair<T, U>>* lower_bound(T key){
	    Node<std::pair<T, U>>* sol = nullptr;
	    Node<std::pair<T, U>>* curr = this->root;
            
	    while(curr != nullptr){
                if(key > curr->val.first){
		    sol = curr;
		    curr = curr->right;
		}
                else curr = curr->left;
            }
        
            return sol;
	}
	Node<std::pair<T, U>>* upper_bound(T key){
	    Node<std::pair<T, U>>* sol = nullptr;
	    Node<std::pair<T, U>>* curr = this->root;
            
	    while(curr != nullptr){
                if(key < curr->val.first){
		    sol = curr;
		    curr = curr->left;
		}
                else curr = curr->right;
	    }

            return sol;
	}
    public:
	avl(){
            this->root = nullptr;
        }
        avl(avl& avl_param){
	    this->root = nullptr;

	    std::vector<std::pair<T, U>> values;
            std::vector<Node<std::pair<T, U>>*> s;
	    s.push_back(avl_param.root);
            while(!s.empty()){
		Node<std::pair<T, U>>* curr = s.back();
		s.pop_back();
		
		if(curr == nullptr) continue;

                values.push_back(curr->val);
		s.push_back(curr->left);
                s.push_back(curr->right);
            }

	    for(auto [key, val] : values) insert(key, val);
	}
	~avl(){
	    clear();
	}

	Node<std::pair<T, U>>* insert(const T key, const U val){
            Node<std::pair<T, U>>* prev = find(key);
            Node<std::pair<T, U>>* curr = new Node<std::pair<T, U>>({key, val});
	    add_node(prev, curr);

	    for(auto ptr = prev; ptr != nullptr; ptr = ptr->parent){
		int64_t balance_factor = calc_balance_factor(ptr);
		
		if(balance_factor > 1){       
                    if(key > ptr->left->val.first) ptr->left = left_rotation(ptr->left);

                    Node<std::pair<T, U>>* par_ptr = ptr->parent;
		    if(par_ptr != nullptr && par_ptr->left == ptr) par_ptr->left = right_rotation(ptr);
		    else if(par_ptr != nullptr && par_ptr->right == ptr) par_ptr->right = right_rotation(ptr);
		    else right_rotation(ptr);
                }
		else if(balance_factor < -1){ 
                    if(key < ptr->right->val.first) ptr->right = right_rotation(ptr->right);

                    Node<std::pair<T, U>>* par_ptr = ptr->parent;
		    if(par_ptr != nullptr && par_ptr->left == ptr) par_ptr->left = left_rotation(ptr);
		    else if(par_ptr != nullptr && par_ptr->right == ptr) par_ptr->right = left_rotation(ptr);
		    else left_rotation(ptr);
                }
		else calc_height(ptr); 

                if(ptr->parent == nullptr) this->root = ptr;
	    }

	    return curr;
        }
       
	bool exists(const T key){
	    if(find(key) != nullptr && find(key)->val.first == key) return true;
	    return false;
	}

	U& operator[](const T key){
	    if(!exists(key)) return insert(key, {})->val.second;
	    return find(key)->val.second;
	}

	void clear(){
	    if(this->root == nullptr) return;

	    std::vector<Node<std::pair<T, U>>*> s1;
	    s1.push_back(this->root);
	    std::vector<Node<std::pair<T, U>>*> s2;
	    while(!s1.empty()){
		Node<std::pair<T, U>>* curr = s1.back();
		s1.pop_back();

		s2.push_back(curr);
		if(curr->left != nullptr) s1.push_back(curr->left);
		if(curr->right != nullptr) s1.push_back(curr->right);
	    }

	    while(!s2.empty()){
		Node<std::pair<T, U>>* curr = s2.back();
		s2.pop_back();
		delete curr;
	    }
	    this->root = nullptr;
	}
		
	template <typename, typename>
	friend class avl_iterator;
};


template <typename T, typename U>
class avl_iterator{
    private:
	avl<T, U>& tree;
	Node<std::pair<T, U>>* ptr;
    public:
	avl_iterator() = delete;
	avl_iterator(avl<T, U>& tree_param) : tree(tree_param){
	    ptr = tree.begin();
	}
	avl_iterator(avl_iterator& avl_iterator_param){
	    ptr = avl_iterator_param.ptr;
	    tree = avl_iterator_param.tree;
	}

	void begin(){
	    ptr = tree.begin();
	}
	void end(){
	    ptr = tree.end();
	}
	
	std::pair<T, U> operator*(){
	    assert(tree.root != nullptr);
	    return ptr->val;
	}
	
	std::pair<T, U> operator++(){
	    assert(tree.root != nullptr);
	    ptr = tree.upper_bound(ptr->val.first);
	    if(ptr == nullptr) ptr = tree.begin();

	    return ptr->val;
	}
	std::pair<T, U> operator--(){
	    assert(tree.root != nullptr);
	    ptr = tree.lower_bound(ptr->val.first);
	    if(ptr == nullptr) ptr = tree.end();

	    return ptr->val;
	}

	std::pair<T, U> operator++(int){
	    assert(tree.root != nullptr);
	    std::pair<T, U> tmp = ptr->val;
	    ptr = tree.upper_bound(ptr->val.first);
	    if(ptr == nullptr) ptr = tree.begin();

	    return tmp;
	}
	std::pair<T, U> operator--(int){
	    assert(tree.root != nullptr);
	    std::pair<T, U> tmp = ptr->val;
	    ptr = tree.lower_bound(ptr->val.first);
	    if(ptr == nullptr) ptr = tree.end();

	    return tmp;
	}
};

#endif
