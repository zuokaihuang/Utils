#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <functional>

#include <list>
template <typename T>
class TreeNode{
public:
    TreeNode(T t){
        value = t;
    }

    void insert(const TreeNode<T>* node){
        chilren.push_back ( node );
    }

    void walk(const TreeNode<T>* tree, std::function<void(T)> callback){
        callback(tree->value);
        auto node = tree->chilren.begin ();
        while( node != tree->chilren.end () ){
            const TreeNode<T>* next = *node;
            this->walk( next , callback );
            node++;
        }
    }
    static bool is_contains(const TreeNode<T>* tree, const TreeNode<T>* dest){
        if( !tree || !dest ) return false;
        if (tree == dest) return true;

        auto node = tree->chilren.begin ();
        bool flag = false;
        while( node != tree->chilren.end () ){
            const TreeNode<T>* next = *node;
            if ( ((void*)next == (void*)dest) ){
                flag = true;
                break;
            }
            flag = TreeNode<T>::is_contains( next, dest );
            node++;
        }
        return flag;
    }

    static void dump(const TreeNode<T>* tree){
        std::cout << tree << "=>" << tree->value << std::endl;
    }

private:
    T value;
    std::list< const TreeNode<T>* > chilren;
};


#endif // TREE_H
