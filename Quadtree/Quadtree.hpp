#ifndef _QUADTREE_HPP
#define _QUADTREE_HPP

#include"Image.hpp"

struct QTNode{
  string name;
  Image image;
  QTNode* parent;
  QTNode *topLeft;
  QTNode *topRight;
  QTNode *bottomLeft;
  QTNode *bottomRight;
};

class Quadtree{
private:
  int count;
  void divide_Map(char *fileName, int level, QTNode* &node, QTNode *p, int n);
  void display(QTNode *node, int level);
  string search(QTNode* node, int x, int y);
public:
  QTNode *data;
  Quadtree();
  void divideMap(char *fileName);
  string search(int x, int y);
  int size();
  bool empty();
  void display();
//  ~Quadtree();
};

Quadtree::Quadtree(){
  data = nullptr;
  count = 0;
}

void Quadtree::divide_Map(char *fileName, int level, QTNode* &node, QTNode *p, int n){
  node = new QTNode;
  count ++;
  node->name = fileName;
  node->parent = p;
  node->topLeft = nullptr;
  node->topRight = nullptr;
  node->bottomLeft = nullptr;
  node->bottomRight = nullptr;
  node->image.imgdata = node->image.LoadBMP(fileName, &node->image.info);
  switch (n) {
    case 1: node->parent->topLeft = node;
    break;
    case 2: node->parent->topRight = node;
    break;
    case 3: node->parent->bottomLeft = node;
    break;
    case 4: node->parent->bottomRight = node;
    break;
  }

  if(level < 3){
    //node->topLeft = new QTNode;
    node->image.Divide(node->image.info, fileName, 1);

    string s1 = node->image.rename(fileName, 1);
    char *cstr1 = &s1[0];

    divide_Map(cstr1, level+1, node->topLeft, node, 1);
/**********************************************************************/
    //node->topRight = new QTNode;
    node->image.Divide(node->image.info, fileName, 2);

    string s2 = node->image.rename(fileName, 2);
    char *cstr2 = &s2[0];

    divide_Map(cstr2, level+1, node->topRight, node, 2);
/************************************************************************/
  //node->bottomLeft = new QTNode;
  node->image.Divide(node->image.info, fileName, 3);

  string s3 = node->image.rename(fileName, 3);
  char *cstr3 = &s3[0];
  divide_Map(cstr3, level+1, node->bottomLeft, node, 3);
/***************************************************************************/
  //node->bottomRight = new QTNode;
  node->image.Divide(node->image.info, fileName, 4);

   string s4 = node->image.rename(fileName, 4);
   char *cstr4 = &s4[0];
  divide_Map(cstr4, level+1, node->bottomRight, node, 4);
  }
}

void Quadtree:: divideMap(char *fileName){
  divide_Map(fileName, 0, data, nullptr, 0);
}

void Quadtree::display(QTNode *node, int level){
  if(level < 3){
    cout << node->name << ":\n";
    cout << node->topLeft->name << ", " << node->topRight->name << ", ";
    cout << node->bottomLeft->name << ", " << node->bottomRight->name << "\n";
    display(node->topLeft, level+1);
    display(node->topRight, level+1);
    display(node->bottomLeft, level+1);
    display(node->bottomRight, level+1);
  }
}

void Quadtree::display(){
  display(data, 0);
}

string Quadtree::search(QTNode* node, int x, int y){
  if(this->empty())
    throw runtime_error("Search: Attempting to search at an empty Quadtree");
  // else if(x > node->image.info.width || y > node->image.info.height || x < 0 || y < 0){
  //   cout << node->image.info.width << " " << node->image.info.height << "     ";
  //   cout << x << " " << y << endl;
  //   throw runtime_error("Search: index out of range");
  // }
  else{
    if(node->topLeft == nullptr)
      return node->name;
    else{
      unsigned int w = div(node->image.info.width,2).quot;
      unsigned int h = div(node->image.info.height,2).quot;
      if(x <= w && y <= h)
        return search(node->topLeft, x, y);
      else if(x >= w && y <= h)
        return search(node->topRight, x-w, y);
      else if(x <= w && y >= h)
        return search(node->bottomLeft, x, y-h);
      else
        return search(node->bottomRight, x-w, y-h);
    }
  }
}

string Quadtree::search(int x, int y){
  return search(data, x, y);
}

int Quadtree::size(){
  return count;
}

bool Quadtree::empty(){
  return count == 0 ? 1: 0;
}

#endif
