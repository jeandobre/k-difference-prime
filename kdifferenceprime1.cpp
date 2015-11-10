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

#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-opt] \tversão otimizada"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 0 e m. "
#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcorrencia "

class KdifferenceInexactMatch1;

class KdifferencePrime{

    public:
      string alpha, beta;
      int k;
      int m;
      int n;
      int mostrarMatriz;
      int otimizado;
      KdifferenceInexactMatch1 *c;

    public:
      KdifferencePrime(){
         this->mostrarMatriz=0;
         this->otimizado=0;
      }

      void processar();
} prime;

//funções auxiliares
int maior(int x, int b, int c){
  if((x <= b) && (x <= c)) return x;
  else if((b <= x) && (b <= c)) return b;
  else return c;
}

int validarParametros(int argc, char** argv){
   string argA[6] = {"-a", "-alpha", "-pattern", "-padrao", "-p", "-P"};
   string argB[6] = {"-b", "-beta", "-text", "-texto", "-t", "-T"};
   string argK[2] = {"-k", "-K"};

   int temA, temB, temK;
   temA = temB = temK = 0;

   for(int z = 1; z < argc; z++){
      if(strcmp(argv[z], "-sm") == 0){
        if(argc == 7) return 0;
        prime.mostrarMatriz=1;
        continue;
      }

      if(strcmp(argv[z], "-opt") == 0){
        if(argc == 7) return 0;
        prime.otimizado=1;
        continue;
      }

      for(int w = 0; w < 6; w++){
        if(!temA && argA[w].compare(argv[z]) == 0){
            prime.alpha = argv[z + 1];
            temA = 1;
            continue;
        }
        else if(!temB && argB[w].compare(argv[z]) == 0){
            prime.beta = argv[z + 1];
            temB = 1;
            continue;
        }
        else if(w < 2 && !temK && argK[w].compare(argv[z]) == 0){
            try{
              prime.k = atoi(argv[z + 1]);
              if(prime.k > 0) temK = 1;
              continue;
            }catch(int e){
              return 0;
            }
        }
      }
   }
   int cc = temA + temB + temK;

   return cc;
}

class KdifferenceInexactMatch1{

  protected:
    int **D;
    string t;
    string a;
    int k;
    int m, n;
    int rowPrint;
  public:
    int linha; //linha que contém o menor valor >= k

  public:
    KdifferenceInexactMatch1(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();
    }
    ~KdifferenceInexactMatch1(){}

    virtual void executar(){};
    void escreverMatrizTela() const;
    virtual void setA(string){};
};

class KdifferenceInexactMatch1Original: public KdifferenceInexactMatch1{
 public:
   KdifferenceInexactMatch1Original(string a, string t, int k): KdifferenceInexactMatch1(a,t,k) {
       this->D = new (nothrow) int*[m + 1];
       for (int j = 0; j <= m; ++j)
          this->D[j] = new (nothrow) int[n + 1];

       this->rowPrint = m; //linha máxima de impressão
   }
   ~KdifferenceInexactMatch1Original(){
       for (int i = 0; i <= m; ++i)
         delete [] D[i];
       delete [] D;
    }

    void executar();

    void setA(string a){
       int mm = this->a.size();
       int mmm = a.size();

       while(mm > mmm)
          delete [] D[mm--];

      this->a = a;
      this->m = mmm;
    }
};

class KdifferenceInexactMatch1Optimizado: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado(string a, string t, int k):KdifferenceInexactMatch1(a,t,k){

       this->D = new (nothrow) int*[2];
       this->D[0] = new (nothrow) int[n + 1];
       this->D[1] = new (nothrow) int[n + 1];

       this->rowPrint = 1;
   }

   ~KdifferenceInexactMatch1Optimizado(){
       delete [] D[0];
       delete [] D[1];
       delete [] D;
    }

   void executar();

   void setA(string a){
      this->a = a;
      this->m = a.size();
    }
};

void KdifferenceInexactMatch1::escreverMatrizTela() const{
    cout << setfill(' ');
	cout<<"\n     ";

	for(int i = 0; i <= n; i++)
	   cout<<setw(2)<<t[i] << " ";

	cout<<"\n";

	for(int i = 0; i <= rowPrint; i++){

		if(i == 0 ) cout<<"  ";
		else cout<<a[i - 1]<<" ";

		for(int l = 0; l <= n; l++){
			cout<<setw(2)<<D[i][l] << " ";
		}
		cout<<"\n";
	}
}

void KdifferenceInexactMatch1Original::executar(){
   int i, l;

   for(l = 0; l <= n; l++)
	  D[0][l] = 0;
   for(i = 1; i <= m; i++)
	  D[i][0] = i;

   bool passou;
   linha = -1;
	for(i = 1; i <= m && linha == -1; i++){
        passou = true;
		for(l = 1; l <= n; l++){
			D[i][l] = maior(D[i-1][l] + 1,
                            D[i][l-1] + 1,
                            a.compare(i-1, 1, t, l-1, 1) == 0 ? D[i-1][l-1] : D[i-1][l-1] + 1);

			if(D[i][l] < k) passou = false;
		}
        if(passou) linha = i;
	}
}

void KdifferenceInexactMatch1Optimizado::executar(){
   int l;

   for(l = 0; l <= n; l++)
	  D[0][l] = 0;

   bool passou;
   linha = -1;
   for(int i = 1; i <= m && linha == -1; i++){
     D[1][0] = i;
	 for(l = 1; l <= n; l++)
        D[1][l] = maior(D[0][l] + 1,
                        D[1][l-1] + 1,
                        a.compare(i-1, 1, t, l-1, 1) == 0 ? D[0][l-1] : D[0][l-1] + 1);

     //aqui vai verificar se a linha que contem todos valores >= k já foi alcançada
     //transporta os valores da linha 1 para 0
     passou = true;
	 for(l = 0; l <= n; l++){
        if(passou && D[1][l] < k) passou = false;
        D[0][l] = D[1][l];
        D[1][l] = 0;
     }
     if(passou) linha = i;
   }
}

void KdifferencePrime::processar(){
   int ocr = 0;

   if(otimizado)
      c = new KdifferenceInexactMatch1Optimizado(alpha, beta, k);
   else
      c = new KdifferenceInexactMatch1Original(alpha, beta, k);

   for(int j = 0; j < (m - k) + 1; j++){
       c->setA(alpha.substr(j));
       c->executar();

       if(c->linha > -1){
           cout<<MSG_N_OCCR<<++ocr<<" em: "<<j<<", "<<j + c->linha<<" "<<alpha.substr(j, c->linha)<<endl;
           if(mostrarMatriz) c->escreverMatrizTela();
       }
   }
   if(ocr == 0) cout<<MSG_0_OCCR<<k<<" diferenca(s)"<<endl;
}

int main(int argc, char** argv) {

   if (argc < 7 || argc > 9) {
	  cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   if(validarParametros(argc, argv) != 3){
      cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   prime.m = prime.alpha.size();
   prime.n = prime.beta.size();

   if(prime.k > prime.m){
     cout<<endl<<ERR_KMAIOR<<endl;
     return 0;
   }

   prime.processar();

   return 1;
}
