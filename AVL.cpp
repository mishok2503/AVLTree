#include <bits/stdc++.h>

using namespace std;

class AVLTree
{
private:

    struct Node
    {
        int v;
        Node* l = nullptr;
        Node* r = nullptr;
        int h = 1;
        int s = 1;

        Node(int v)
        {
            this->v = v;
        }

        int balance()
        {
            return (r ? r->h : 0) - (l ? l->h : 0);
        }

        void fix()
        {
            h = max(r ? r->h : 0, l ? l->h : 0) + 1;
            s = (r ? r->s : 0) + (l ? l->s : 0) + 1;
        }
    };

    Node* head = nullptr;
    int size = 0;

    function<Node*(Node*)> _min = [&](Node* n) { return n->l ? _min(n->l) : n; };
    function<Node*(Node*)> _max = [&](Node* n) { return n->r ? _max(n->r) : n; };

    Node* small_rotate(Node* node, bool right)
    {
        Node* t;
        if (right)
        {
            t = node->l;
            node->l = t->r;
            t->r = node;
        }
        else
        {
            t = node->r;
            node->r = t->l;
            t->l = node;
        }

        node->fix();
        t->fix();

        return t;
    }

    Node* balance(Node* node)
    {
        node->fix();
        if (node->balance() == -2)
        {
            if (node->l->balance() == 1)
                node->l = small_rotate(node->l, false);
            return small_rotate(node, true);
        }

        if (node->balance() == 2)
        {
            if (node->r->balance() == -1)
                node->r = small_rotate(node->r, true);
            return small_rotate(node, false);
        }

        return node;
    }

public:

    void add(int v)
    {
        ++size;
        function<Node*(Node*)> deep_add = [&](Node* node)
        {
            if (!node)
                return new Node(v);
            if (v >= node->v)
                node->r = deep_add(node->r);
            else
                node->l = deep_add(node->l);
            return balance(node);
        };
        head = deep_add(head);
    }

    void del(int v)
    {
        --size;
        function<Node*(Node*)> deep_del = [&](Node* node)
        {
            if (v > node->v)
                node->r = deep_del(node->r);
            else if (v < node->v)
                node->l = deep_del(node->l);
            else
            {
                Node* l = node->l;
                Node* r = node->r;
                delete node;
                if (!r)
                    return l;

                function<Node*(Node*)> del_min = [&](Node* n)
                {
                    if (!n->l)
                        return n->r;

                    n->l = del_min(n->l);
                    return balance(n);
                };
                Node* t = _min(r);
                t->r = del_min(r);
                t->l = l;
                return balance(t);
            }

            return balance(node);
        };
        head = deep_del(head);
    }

    int get_size() { return size; }

    int get_max() { return _max(head)->v;}
    int get_min() { return _min(head)->v;}

    bool check(Node* node)
    {
        if (node)
        {
            if (!check(node->l) || !check(node->r))
                return false;
            node->fix();
            return abs(node->balance()) < 2;
        }

        return true;
    }

    int nth_element(int k)
    {
        function<int(Node*, int)> dfs = [&](Node* node, int n)
        {
            int l = node->l ? node->l->s : 0;
            if (n == l)
                return node->v;
            if (n > l)
                return dfs(node->r, n - l - 1);
            return dfs(node->l, n);
        };

        return dfs(head, k);
    }

    pair<bool, int> lower_bound(int v)
    {
        if (v > get_max() || v < get_min())
            return {false, 0};

        auto less = [&](Node* a, Node* b)
        {
            if (!a || a->v < v)
                return b;
            if (!b || b->v < v)
                return a;
            return a->v < b->v ? a : b;
        };

        function<Node*(Node*)> lb = [&](Node* node)
        {
            return node ? less(node, lb(v > node->v ? node->r : node->l)) : node;
        };

        return {true, lb(head)->v};
    }

    ~AVLTree()
    {
        function<void(Node*)> destroy = [&](Node* node)
        {
            if (node)
            {
                destroy(node->l);
                destroy(node->r);
                delete node;
            }
        };
        destroy(head);
    }

};

int main()
{
    AVLTree tree;
    
    for (int i=0; i < 10; ++i)
        tree.add(5 * i);
    
    cout << tree.lower_bound(11).second << '\n';
    
    return 0;
}
