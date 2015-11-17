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

class KdifferenceInexactMatch2;

class KdifferencePrime{

    public:
      string alpha, beta;
      int k;
      int m;
      int n;
      int mostrarMatriz;
      int otimizado;
      KdifferenceInexactMatch2 *c;

    public:
      KdifferencePrime(){
         this->mostrarMatriz=0;
         this->otimizado=0;
      }

      void processar();
} prime;

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

int minimo(int a, int b){
   return a <= b ? a : b;
}

int maximo(int a, int b, int c){
   if(a >= b && a >= c) return a;
   else if(b >= a && b >= c) return b;
   else return c;
}

class KdifferenceInexactMatch2{

  private:
    int **L;
    string a;
    string t;
    int k;
    int m, n;
  public:
    int linha; //linha que contém o menor valor >= k

  private:
    int getMinD(){
      return -(k + 1);
    }
    int getMaxD(){
      return n + 1;
    }
    int dToMatriz(int d){ //converte indice d para indice da matriz L
      return d + (k + 1);
    }

    int getMinE(){
      return -1;
    }

    int getMaxE(){
      return k;
    }

    int eToMatriz(int e){
      return e - 1;
    }

    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor;
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)];
    }

    int getDimensaoD(){
      return k + n + 3; //k+1, n+1, 1
    }

    int getDimensaoE(){
      return k + 2; // k + 1, 2
    }

  public:
    KdifferenceInexactMatch2(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();

       this->L = new int*[getDimensaoD()];
       for (int j = 0; j < getDimensaoD(); ++j)
          this->L[j] = new int[getDimensaoE()];

    }

    ~KdifferenceInexactMatch2(){
       for (int i = 0; i < getDimensaoD(); ++i)
         delete [] L[i];
       delete [] L;
    }
    void executar();
    void escreverMatrizTela();

    void setA(string a){
      this->a = a;
      this->m = a.size();
    }
};

void KdifferenceInexactMatch2::escreverMatrizTela(){
    cout << setfill(' ');

    cout<<endl<<"    ";
	for(int i = getMinE(); i <= getMaxE(); i++) cout<<setw(2)<<i<< " ";

	int vr;
    cout<<endl;
    for(int i = getMinD(), x = 0; i <= getMaxD(); i++, x++){

        cout<<setw(3)<<i<<" ";
        for(int l = getMinE(), y = 0; l <= getMaxE(); l++, y++){
            vr = getL(i,l);

            if ((x <= k || y <= k) && (x+y) <= k)cout<<"   ";
			else cout<<setw(2)<<vr<<" ";
		}
		cout<<"\n";
	}
}

void KdifferenceInexactMatch2::executar(){
    int d,e, row;

	for(d = 0; d <= n; d++)
        setL(d, - 1, -1);

    for(d = -(k+1); d <= -1; d++){
      setL(d, abs(d) - 2, abs(d) - 2);
      setL(d, abs(d) - 1, abs(d) - 1);
    }

    for(e = -1; e<= k; e++)
        setL(getMaxD(), e, -1);

    bool passou = true;
    linha = -1;
    for(e = 0; e <= k && passou; e++){
        for(d = -(e); d <= n; d++){
           row = maximo(getL(d-1, e-1),
                        getL(d,   e-1) + 1,
                        getL(d+1, e-1) + 1);
           row = minimo(row, m);
           while(row < m && row+d < n && a.compare(row, 1, t, row+d, 1) == 0)
             row++;

           setL(d, e, row);
           if(row == m && e < k) passou = false;//para otizar aki deve parar o laço break
        }
    }

    if(passou){
        row = -1;
        for(d = -(k+1); d <= n; d++){
            row = getL(d, k-1);
            if (row > linha) linha = row;
        }
        linha++;
    }
}

void KdifferencePrime::processar(){
   int ocr = 0;

   c = new KdifferenceInexactMatch2(alpha, beta, k);

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
