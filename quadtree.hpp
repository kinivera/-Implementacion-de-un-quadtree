# include <iostream>
# include <cmath>
# include <stdexcept>

using namespace std;

template <typename T>
struct node
{
    T value;
    node<T> *children[4];
    node<T> *parent;
};

template <typename T>
class Quadtree
{
    private: 
        node<T> *root;
        int height;
        int breadth;

    public:
        Quadtree();
        ~Quadtree();

        bool empty();
        void erase(node<T> *t);
};

//Creates empty quadtree
template <typename T>
Quadtree<T>::Quadtree()
{
    root = nullptr;
}

//Deletes quadtree
template <typename T>
Quadtree<T>::~Quadtree()
{
    erase(root);
}

//Checks if quadtree is empty
template <typename T>
bool Quadtree<T>::empty()
{
    return root==nullptr;
}

//Erases instance of quadtree
template <typename T>
void Quadtree<T>::erase(node<T> *t)
{
    if (t != nullptr)
    {
        for (int i = 0; i < 4; ++i)
        {
            erase(t->children[i]);
        }
        delete t;
    }
}