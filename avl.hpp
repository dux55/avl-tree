#ifndef AVL_HPP
#define AVL_HPP

#include "lib/node.hpp"
#include <cstdint>
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
    public:
	avl(){
            this->root = nullptr;
        }
        avl(avl& avl){
	    this->root = nullptr;

	    std::vector<std::pair<T, U>> values;
            std::vector<Node<std::pair<T, U>>*> s;
	    s.push_back(avl.root);
            while(!s.empty()){
		Node<std::pair<T, U>>* curr = s.back();
		s.pop_back();
		
		if(curr == nullptr) continue;

                values.push_back(curr->val);
		s.push_back(curr->left);
                s.push_back(curr->right);
            }

	    for(auto [key, val] : values) this->insert(key, val);
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


        std::vector<T> dbg_traverse(){
            std::vector<T> traversal;
        
            Node<std::pair<T, U>>* curr = this->root;
            std::vector<Node<std::pair<T, U>>*> s;
            while(curr != nullptr || !s.empty()){
                while(curr != nullptr){
                    s.push_back(curr);
                    curr = curr->left;
                }
        
                curr = s.back();
                traversal.push_back(curr->val.first);
                curr = curr->right;
                s.pop_back();
            }
            
            return traversal;
        }
};

#endif

/*
TODO:
iterator class(begin, end)
*/
