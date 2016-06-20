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

int main(){
    gerar("alpha_1000.txt", 1000);
    gerar("beta_2000.txt",  2000);
    return 0;

}
