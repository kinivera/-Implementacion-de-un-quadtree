#include"Quadtree.hpp"
#include<fstream>

string Place(ifstream &ifs, string quadrant){
  string temp;
  string line = "";
  string line2;
  string here = "";
  if (ifs.good()) {
    while (!ifs.eof()) {
      getline(ifs, line);
      temp = "";
      for(int j = 0; j < quadrant.size(); j++){
        temp+=line[j];
        if(j+1 == quadrant.size()){
          line2 = "";
          for (int k = j+3; k < line.size(); k++)
            line2+= line[k];
        }
        if(temp == quadrant)
          here+= line2 + "\n";
      }
    }
  }
  ifs.close();
  return here;
}


int main(){
  Quadtree raiz;

  char nombre[] = "Map.bmp";
  int x;
  int y;
  string m = "";
  string n = "";
  bool next = true;
  int g;
  string temp;

  ifstream ifs ("a.txt");
  ifstream ifst ("Places.txt");
  ofstream ofs ("return.txt", ios::out);

  string line = "";
  if (ifs.good()) {
    while (!ifs.eof()) {
      ifs >> line ;
    }
  }
  ifs.close();

  for (int k = 0; k < line.size(); k++){
    temp = line[k];
    g = atoi(temp.c_str());
    if(temp == ",")
      next = false;
    else if(next)
      m+=temp;
    else
      n+=temp;
  }

  x = atoi(m.c_str());
  y = atoi(n.c_str());

  raiz.divideMap(nombre);

  cout << raiz.size() << endl;

  raiz.display();

  cout << raiz.empty() << endl;

  string quadrant = raiz.search(x,y);
  cout << quadrant.size() << endl;
  cout << quadrant << endl;

  cout << x << ", " << y;

  string here;
  here = Place(ifst, quadrant);

  if(ofs.good()){
    ofs << quadrant << endl;
    ofs << here;
  }
  ofs.close();

  cout << here;
  return 0;
}
