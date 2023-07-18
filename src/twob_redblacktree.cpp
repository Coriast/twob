#include "twob_redblacktree.h"

using namespace std;
using namespace twob;

RedBlackTree::RedBlackTree()
{
    nil = new NodeRB(0);
    nil->color = ColorRB::black;
    root = nil;
}

NodeRB* RedBlackTree::find(int key)
{
    stateRB state;

    NodeRB **search = &root;
    find(key, &search, state);

    if(state == stateRB::found)
        return *search;
    else 
        return nullptr;
}

bool RedBlackTree::insert(int key)
{
    stateRB state;
    NodeRB **search = &root;
    find(key, &search, state);

    if(state == stateRB::found)
    {
        return false;
    }
    cout << key << " Inserted!" << endl;

    NodeRB* node = new NodeRB(key);
    {
        node->left = nil;
        node->right = nil;
        node->color = ColorRB::red;
    }
    if(state == stateRB::empty)
    {
        root = node; // possible break
    }
    if (state == stateRB::on_left)
    {
        node->parent = *search;
        (*search)->left = node;
    }
    if (state == stateRB::on_right)
    {
        node->parent = *search;
        (*search)->right = node;
    }

    if (node->parent == nullptr || node->parent == nil)
    {
        node->color = ColorRB::black;
        return true;
    }

    if (node->parent->parent == nullptr)
        return true;

    insert_fixup(node);
    return true;
}

void RedBlackTree::insert_fixup(NodeRB* node)
{
    NodeRB* aux;
    while (node->parent->color == ColorRB::red)
    {
        if (node->parent == node->parent->parent->left)
        {
            aux = node->parent->parent->right;
            if (aux->color == ColorRB::red)
            {
                aux->color = ColorRB::black;
                node->parent->color = ColorRB::black;
                node->parent->parent->color = ColorRB::red;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    left_rotate(node);
                }
                node->parent->color = ColorRB::black;
                node->parent->parent->color = ColorRB::red;
                right_rotate(node->parent->parent);
            }  
        }
        else
        {
            aux = node->parent->parent->left;
            if (aux->color == ColorRB::red)
            {
                aux->color = ColorRB::black;
                node->parent->color = ColorRB::black;
                node->parent->parent->color = ColorRB::red;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    right_rotate(node);
                }
                node->parent->color = ColorRB::black;
                node->parent->parent->color = ColorRB::red;
                left_rotate(node->parent->parent);
            }
        }
        if (node == root)
            break;
    }
    root->color = ColorRB::black;
}

bool RedBlackTree::remove(int key)
{
    stateRB state;
    NodeRB **search = &root;
    find(key, &search, state);
    if(state != stateRB::found)
    {
        return false;
    }
    cout << key << " Removed!" << endl;

    NodeRB* sibling;
    NodeRB* aux = *search;
    NodeRB* subtree = *search;
    ColorRB subtree_color = subtree->color;
    
    if ((*search)->left == nil)
    {
        sibling = (*search)->right;
        transplant(aux, aux->right);
    }
    else if ((*search)->right == nil)
    {
        sibling = (*search)->left;
        transplant(aux, aux->left);
    }
    else
    {
        subtree = tree_minimum((*search)->right);
        subtree_color = subtree->color;
        sibling = subtree->right;
        if (subtree != (*search)->right)
        {
            transplant(subtree, subtree->right);
            subtree->right = aux->right;
            subtree->right->parent = subtree;
        }
        else
        {
            sibling->parent = subtree;
        }
        transplant(aux, subtree);
        subtree->left = aux->left;
        subtree->left->parent = subtree;
        subtree->color = aux->color;
    }

    if (subtree_color == ColorRB::black)
        remove_fixup(sibling);

    delete aux;
    return true;
}

void RedBlackTree::remove_fixup(NodeRB* node)
{
    NodeRB* aux;
    while (node != root && node->color == ColorRB::black)
    {
        if (node == node->parent->left)
        {
            aux = node->parent->right;
            if (aux->color == ColorRB::red)
            {
                aux->color = ColorRB::black;
                node->parent->color = ColorRB::red;
                left_rotate(node->parent);
                aux = node->parent->right;
            }
            if (aux->left->color == ColorRB::black && aux->right->color == ColorRB::black)
            {
                aux->color = ColorRB::red;
                node = node->parent;
            }
            else
            {
                if (aux->right->color == ColorRB::black)
                {
                    aux->left->color = ColorRB::black;
                    aux->color = ColorRB::red;
                    right_rotate(aux);
                    aux = node->parent->right;
                }
                aux->color = node->parent->color;
                node->parent->color = ColorRB::black;
                aux->right->color = ColorRB::black;
                left_rotate(node->parent);
                node = root;
            }
        }
        else
        {
            aux = node->parent->left;
            if (aux->color == ColorRB::red)
            {
                aux->color = ColorRB::black;
                node->parent->color = ColorRB::red;
                right_rotate(node->parent);
                aux = node->parent->left;
            }
            if (aux->right->color == ColorRB::black && aux->left->color == ColorRB::black)
            {
                aux->color = ColorRB::red;
                node = node->parent;
            }
            else
            {
                if (aux->left->color == ColorRB::black)
                {
                    aux->right->color = ColorRB::black;
                    aux->color = ColorRB::red;
                    left_rotate(aux);
                    aux = node->parent->left;
                }
                aux->color = node->parent->color;
                node->parent->color = ColorRB::black;
                aux->left->color = ColorRB::black;
                right_rotate(node->parent);
                node = root;
            }
        }
    }
    node->color = ColorRB::black;
}

void RedBlackTree::transplant(NodeRB* to_remove, NodeRB* to_replace)
{
    if (to_remove->parent == nil || to_remove->parent == nullptr)
    {
        root = to_replace;
    }
    else if (to_remove == to_remove->parent->left)
    {
        to_remove->parent->left = to_replace;
    }
    else
    {
        to_remove->parent->right = to_replace;
    }
    to_replace->parent = to_remove->parent;
}

NodeRB* RedBlackTree::tree_minimum(NodeRB* node)
{
    while (node->left != nil)
        node = node->left;
    return node;
}

void RedBlackTree::left_rotate(NodeRB* node)
{
    NodeRB* aux = node->right;
    node->right = aux->left;

    if (aux->left != nil)
    {
        aux->left->parent = node;
    }
    aux->parent = node->parent;

    if (node->parent == nil || node->parent == nullptr)
    {
        root = aux;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = aux;
    }
    else
    {
        node->parent->right = aux;
    }
    aux->left = node;
    node->parent = aux;
}

void RedBlackTree::right_rotate(NodeRB* node)
{
    NodeRB* aux = node->left;
    node->left = aux->right;

    if (aux->right != nil)
    {
        aux->right->parent = node;
    }
    aux->parent = node->parent;

    if (node->parent == nil || node->parent == nullptr)
    {
        root = aux;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = aux;
    }
    else
    {
        node->parent->left = aux;
    }
    aux->right = node;
    node->parent = aux;
}

void RedBlackTree::find(int key, NodeRB ***node, stateRB &state)
{
    if(**node == nil)
    {
        state = stateRB::empty;
    } 
    else
    {
        if(key == (**node)->key)
        {
            state = stateRB::found;
        } 
        else 
        {
            if (key < (**node)->key)
            {
                if((**node)->left == nil)
                {
                    state = stateRB::on_left;
                } 
                else 
                {
                    (*node) = &(**node)->left;
                    find(key, node, state);
                }
            } 
            else 
            {
                if((**node)->right == nil)
                {
                    state = stateRB::on_right;
                } 
                else 
                {
                    (*node) = &(**node)->right;
                    find(key, node, state);
                }
            }
        }
    }
}

void RedBlackTree::print()
{
    cout << "\n";
    print(root, "", true);
}

void RedBlackTree::print(NodeRB* node, string indent, bool last)
{
    if (node == nil)
        return;

    cout << indent;

    if (last)
    {
        cout << "R---";
        indent += "    ";
    }
    else
    {
        cout << "L---";
        indent += "|   ";
    }
    string node_color;
    if (ColorRB::red == node->color)
        node_color = "Red";
    else
        node_color = "Black";

    cout << node->key << "(" << node_color << ")" << endl;
    print(node->left, indent, false);
    print(node->right, indent, true);
}

