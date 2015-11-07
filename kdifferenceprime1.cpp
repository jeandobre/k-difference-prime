/*
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ciência da Computação
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz, aqui e utilizado o algoritmo completo, nao otimizado"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 0 e m. "
#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcorrencia "

struct Prime{
  string alpha,beta;
  int k;
  int m;
  int n;
  int mostrarMatriz=0;
};

class KdifferenceInexactMatch1{

  private:
    int **D;
    string a;
    string t;
    int k;
    int m, n;

  private:
    int minimo(int, int);

  public:
    KdifferenceInexactMatch1(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();

       this->D = new int*[m + 1];
       for (int j = 0; j < m + 1; ++j)
          this->D[j] = new int[n];
    }
    ~KdifferenceInexactMatch1(){
       for (int i = 0; i < m + 1; ++i)
         delete [] D[i];
       delete [] D;
    }
    void executar();
    void escreverMatrizTela();
    int getLinhaMenorK();
};

int KdifferenceInexactMatch1::minimo(int i, int l){
	//l linha
	//c coluna

	int x = D[i-1][l] + 1;
	int b = D[i][l-1] + 1;

	int c = (a.compare(i-1, 1, t, l-1, 1) == 0) ? D[i-1][l-1] : D[i-1][l-1] + 1;
	//A < B < C then A

	if((x <= b) && (x <= c)) return x;
	else if((b <= x) && (b <= c)) return b;
	else return c;

}

void KdifferenceInexactMatch1::escreverMatrizTela(){
    cout << setfill(' ');
	cout<<"\n     ";

	for(int i = 0; i <= n; i++)
	   cout<<setw(2)<<t[i] << " ";

	cout<<"\n";

	for(int i = 0; i <= m; i++){

		if(i == 0 ) cout<<"  ";
		else cout<<a[i - 1]<<" ";

		for(int l = 0; l <= n; l++){
			cout<<setw(2)<<D[i][l] << " ";
		}
		cout<<"\n";
	}
}

void KdifferenceInexactMatch1::executar(){
   for(int l = 0; l <= n; l++)
	  D[0][l] = 0;

	for(int i = 1; i <= m; i++)
	   D[i][0] = i;

	for(int i = 1; i <= m; i++){
		for(int l = 1; l <= n; l++){
			D[i][l] = minimo(i, l);
		}
	}
}

int KdifferenceInexactMatch1::getLinhaMenorK(){
    int linha = -1;
    for(int z = m; z >= 0; z--){
        bool passou = true;
        for(int x = 0; x < n; x++){
           if(D[z][x] < k) passou = false;
        }
        if(passou) linha = z;
    }
    return linha;
}

int validarParametros(int argc, char** argv, Prime *prime){
   string argA[6] = {"-a", "-alpha", "-pattern", "-padrao", "-p", "-P"};
   string argB[6] = {"-b", "-beta", "-text", "-texto", "-t", "-T"};
   string argK[2] = {"-k", "-K"};

   int temA, temB, temK;
   temA = temB = temK = 0;

   for(int z = 1; z < argc; z++){
      if(strcmp(argv[z], "-sm") == 0){
        if(argc == 7) return 0;
        prime->mostrarMatriz=1;
        continue;
      }

      for(int w = 0; w < 6; w++){
        if(!temA && argA[w].compare(argv[z]) == 0){
            prime->alpha = argv[z + 1];
            temA = 1;
            continue;
        }
        else if(!temB && argB[w].compare(argv[z]) == 0){
            prime->beta = argv[z + 1];
            temB = 1;
            continue;
        }
        else if(w < 2 && !temK && argK[w].compare(argv[z]) == 0){
            try{
              prime->k = atoi(argv[z + 1]);
              if(prime->k > 0) temK = 1;
              continue;
            }catch(int e){
              return 0;
            }
        }
      }
   }
   int cc = temA + temB + temK;
   if(cc == 3){
     prime->m = prime->alpha.size();
     prime->n = prime->beta.size();
   }

   return cc;
}


int main(int argc, char** argv) {

   if (argc != 7 && argc != 8) {
	  cerr<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   Prime prime;
   if(validarParametros(argc, argv, &prime) != 3){
      cerr<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   if(prime.k > prime.m){
     cerr<<endl<<ERR_KMAIOR<<endl;
     return 0;
   }

   int linha;
   int ocr = 0;

   KdifferenceInexactMatch1* c;

   for(int j = 0; j < (prime.m - prime.k) + 1; j++){ //aki o j está varrendo todo o padrão alpha
       c = new KdifferenceInexactMatch1(prime.alpha.substr(j), prime.beta, prime.k); //melhorar aki, diminuir o uso de memória
       c->executar();
       linha = c->getLinhaMenorK(); //retorna -1 se não encontrou

       if(linha > -1){
           cerr<<MSG_N_OCCR<<++ocr<<" em: "<<j<<", "<<j + linha<<" "<<prime.alpha.substr(j, linha)<<endl;
           if(prime.mostrarMatriz) c->escreverMatrizTela();
       }
       delete(c);
   }
   if(ocr == 0)
      cerr<<MSG_0_OCCR<<prime.k<<" diferenca(s)"<<endl;

	return 1;
}
