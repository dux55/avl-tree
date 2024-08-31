#include "../avl.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>

#include<bits/stdc++.h>
using namespace std;

template<typename T>
class AVL_tree{
    private:
        class AVL_Node{  
            public:
                T val;
                AVL_Node* left;
                AVL_Node* right;
                AVL_Node* parent; 
                int height;
                
                // Node Constructor -
                AVL_Node():left{nullptr}, right{nullptr}, parent{nullptr}, val{}, height{0}{}
                AVL_Node(T val): left{nullptr}, right{nullptr}, parent{nullptr}, height{0}, val{val}{}
        };
        AVL_Node* root;

        short (*cmp_func)(T, T);
        
        // Utility -
        void add_child(AVL_Node* prev_nd, AVL_Node* chd_nd){
            if(prev_nd == nullptr){
                this->root = chd_nd;
                return;
            }
                
            if(chd_nd->val < prev_nd->val) prev_nd->left = chd_nd;
	    else prev_nd->right = chd_nd;

            // update parent pointer of child node
            chd_nd->parent = prev_nd;
        }

        AVL_Node* find_node(T val){
            AVL_Node* prev_ptr = nullptr;
            for(AVL_Node* tmp_ptr = this->root ; tmp_ptr != nullptr ; ){
                prev_ptr = tmp_ptr;
		if(val < tmp_ptr->val) tmp_ptr = tmp_ptr->left;
		else tmp_ptr = tmp_ptr->right;
            }
            // for not find the node, return their parent node.
            return prev_ptr;
        }

        int get_max(int a, int b){ return (a >= b) ? a : b; }

        int get_height(AVL_Node* ptr_nd){
            if(ptr_nd == nullptr)
                return -1;

            return ptr_nd->height;
        }

        int cal_balance(AVL_Node* nd_ptr){ return get_height(nd_ptr->left) - get_height(nd_ptr->right); }

        AVL_Node* Right_rotate(AVL_Node* curr_nd){
            AVL_Node* lft_chd = curr_nd->left;
            AVL_Node* rgt_suc = lft_chd->right;

            // Perform rotation
            lft_chd->right = curr_nd;
            curr_nd->left = rgt_suc;

            // update parent pointer of current pointed node and child node 
            lft_chd->parent = curr_nd->parent;
            curr_nd->parent = lft_chd;
            if(rgt_suc != nullptr)
                rgt_suc->parent = curr_nd;
	    
	    assert(lft_chd->parent != lft_chd);

            // Update heights
            lft_chd->height = get_max(get_height(lft_chd->left), get_height(lft_chd->right)) + 1;
            curr_nd->height = get_max(get_height(curr_nd->left), get_height(curr_nd->right)) + 1;

            return lft_chd;
        }

        AVL_Node* Left_rotate(AVL_Node* curr_nd){
            AVL_Node* rgt_chd =  curr_nd->right;
            AVL_Node* lft_suc = rgt_chd->left;
        
            // Perform rotation
            rgt_chd->left = curr_nd;
            curr_nd->right = lft_suc;

            // update parent pointer of current pointed node and child node 
            rgt_chd->parent = curr_nd->parent;
            curr_nd->parent = rgt_chd;
            if(lft_suc != nullptr)
                lft_suc->parent = curr_nd;

	    assert(rgt_chd->parent != rgt_chd);

            // Update heights
            rgt_chd->height = get_max(get_height(rgt_chd->left), get_height(rgt_chd->right)) + 1;
            curr_nd->height = get_max(get_height(curr_nd->left), get_height(curr_nd->right)) + 1;

            return rgt_chd;
        }

        void splice(AVL_Node* ptr_nd){
            /* remove node confirm that the ptr_nd have successor in single side.
                                                                   Case 1.   ;    Case 2.   */
            AVL_Node* succsor_nd = (ptr_nd->left != nullptr) ? ptr_nd->left : ptr_nd->right;
            
            if(ptr_nd == this->root){  // for remove the root.
                this->root = succsor_nd;
            }else{              
                AVL_Node* par_nd = ptr_nd->parent;
                if(par_nd->left == ptr_nd)
                    par_nd->left = succsor_nd;
                else
                    par_nd->right = succsor_nd;

                if(succsor_nd != nullptr) succsor_nd->parent = par_nd;
            }
        }

    public:
        enum Order{  // for the order traversal.
            pre_order,
            post_order,
            in_order
        };

	AVL_tree(){}
        // Constructor -
        AVL_tree(short (*def_cmp_func)(T, T)):root{nullptr}, cmp_func{def_cmp_func}{}
        
        // Operation - 
        void insert(T val){
            // BST insertion operation
            AVL_Node* prev_nd = find_node(val);  
            AVL_Node* chd_nd = new AVL_Node(val);
            add_child(prev_nd, chd_nd);
            
            // Balance the tree
	    assert(chd_nd != chd_nd->parent);
            for(AVL_Node* nd_ptr = prev_nd ; nd_ptr != nullptr ; nd_ptr = nd_ptr->parent){
                const int& bf = cal_balance(nd_ptr);
                
                // Left bias unbalance
                if( bf > 1 ){       
                    if(val > nd_ptr->left->val)
                        nd_ptr->left = Left_rotate(nd_ptr->left);

                    // update parent's pointer
                    AVL_Node* par_ptr = nd_ptr->parent;
                    if(par_ptr != nullptr && par_ptr->right == nd_ptr)
                        par_ptr->right = Right_rotate(nd_ptr);
                    else if(par_ptr != nullptr && par_ptr->left == nd_ptr)
                        par_ptr->left = Right_rotate(nd_ptr);
                    else
                        Right_rotate(nd_ptr);
                    
                // Right bias unbalance
                }else if(bf < -1){      
                    if(val < nd_ptr->right->val)
                        nd_ptr->right = Right_rotate(nd_ptr->right);

                    // update parent's pointer
                    AVL_Node* par_ptr = nd_ptr->parent;
                    if(par_ptr != nullptr && par_ptr->right == nd_ptr)
                        par_ptr->right = Left_rotate(nd_ptr);
                    else if(par_ptr != nullptr && par_ptr->left == nd_ptr)
                        par_ptr->left = Left_rotate(nd_ptr);
                    else  // nd_ptr equal root 
                        Left_rotate(nd_ptr);

                // else, the sub-tree is already balanced
                }else{  
                    nd_ptr->height = get_max(get_height(nd_ptr->left), get_height(nd_ptr->right)) + 1;
                } 

                // finally update the new root pointer 
                if(nd_ptr->parent == nullptr)
                    this->root = nd_ptr;
            }
        }

        // remove operation is still working on it though.

        // Smart_queue just like a queue offer general interface, you can use stl-container.
	vector<T> traverse(){
	    std::vector<T> traversal;
        
            auto curr = root;
            std::stack<AVL_Node*> s;
            while(curr != nullptr || !s.empty()){
                while(curr != nullptr){
                    s.push(curr);
                    curr = curr->left;
                }
        
                curr = s.top();
                traversal.push_back(curr->val);
                curr = curr->right;
                s.pop();
            }
            
            return traversal;
        }

};

int main(void){
    /*bst<int, int> test;
    std::vector<int> traversal;

    int iterations = 100000;    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> values(1, iterations);
    std::uniform_int_distribution<> choice(0, 1);
    
    for(int i = 1; i <= iterations; i++){
        if(choice(gen)){
	    test.insert(values(gen), 1);
        }

	traversal = test.traverse();
        if(!std::is_sorted(traversal.begin(), traversal.end())){
	    std::cout << "TEST FAILED\n";
            return 1;
        }
	traversal.clear();

        if(i%(iterations/100) == 0){
            std::cout << "iteration " << i << '/' << iterations << '\n';
        }
    }*/

    avl<int, int> test;
    test[1] = 1;

    avl<int, int> test2 = test;

    cout << test2.exists(1)
    << test2.exists(2);

    return 0;
}
