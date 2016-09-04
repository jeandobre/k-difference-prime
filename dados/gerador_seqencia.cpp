#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define mega 1048576

//#define RAND_MAX 3

// 1 milhão

void gerar(string arquivo, int tamanho){
  ofstream myfile;
  myfile.open (arquivo);
  for(int i = 1; i<= (tamanho); i++){
    switch(rand() % 4 + 1){
      case 1 : myfile << "A"; break;
      case 2 : myfile << "C"; break;
      case 3 : myfile << "G"; break;
      case 4 : myfile << "T"; break;
    }
  }
  myfile.close();
}

void gerar(string arquivo, int tamanho, const char *a){
  ofstream myfile;
  myfile.open (arquivo);
  for(int i = 1; i<= (tamanho); i++){
     myfile << a;
  }
  myfile.close();
}

int main(){
    int sizet = 100000;
    gerar("alfa_100000.txt", sizet);
    gerar("beta_100000.txt", sizet);
    gerar("alfa_A100000.txt", sizet, "A");
    gerar("beta_C100000.txt", sizet, "C");
    return 0;

}
