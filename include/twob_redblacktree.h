#pragma once 
#include <iostream>
#include <string>
#include <iomanip>

namespace twob
{
    enum class stateRB
    {
        empty,
        found,
        on_left,
        on_right
    };

    enum class ColorRB
    {
	    red,
        black
    };

    class NodeRB
    {
    public:
    friend class RedBlackTree;
        const int key;
        int data;

        NodeRB(int _key) : key(_key) {};
        
    private:
        NodeRB* left = nullptr;
        NodeRB* right = nullptr;
        NodeRB* parent = nullptr;
        ColorRB color;
    };

    class RedBlackTree
    {
	public:
        RedBlackTree();

		bool insert(int key);
		bool remove(int key);
        NodeRB* find(int key);

		void print();
        
    private:
        NodeRB* root;
        NodeRB* nil;

        void insert_fixup(NodeRB* node);
        void left_rotate(NodeRB* node);
        void right_rotate(NodeRB* node);

        NodeRB* tree_minimum(NodeRB* x);
        void transplant(NodeRB* u, NodeRB* v);
        void remove_fixup(NodeRB* x);

        void find(int key, NodeRB ***node, stateRB &state);

        void print(NodeRB* node, std::string indent, bool last);

    };

}