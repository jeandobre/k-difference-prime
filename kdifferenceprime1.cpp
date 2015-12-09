/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ci�ncia da Computa��o

   Este projeto implementa a vers�o original do k-difference inexact match por meio da
   matriz D [m x n], algoritmo serial, proposto por Landau Gad M. e Vishkin Uzi no artigo
   INTRODUCING EFFICIENT PARALLELISM INTO APROXIMATE STRING MATCHING AND A NEW SERIAL ALGORITHM
   p�gina 222.

   A vers�o do k-difference primer foi idealizada pelo Prof. Dr. Said Sadique Adi cuja id�ia �
   verificar toda linha da matriz D tem valor >= k.
   A vers�o otimizada traz a i�deia de implementar a matriz D com apenas 2 linhas [2 x n] que � o m�nimo
   necess�rio para que a recorr�ncia aconte�a. Essa vers�o diminui muito o uso de mem�ria mas n�o de processamento.
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <fstream>

using namespace std;

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-opt] \tvers�o otimizada\n [-st] \tmostrar o tempo de execucao"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 0 e m. "
#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcorrencia "

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch1;

class KdifferencePrime{

    public:
      string alpha, beta;//seguindo o modelo proposto por GUSFIELD, nomes de vari�veis que representam o padr�o e o texto respectivamente
      int k;             //quantidade de difer�n�as
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      int mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int otimizado;     //booleano 0, 1 usado para executar a vers�o otimizada do algoritmo
      bool tempo;        //booleano 0, 1 usado para imprime o tempo na tela (O tempo � calculado independentemente)
      KdifferenceInexactMatch1 *c;

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usu�rio n�o escolher nada
         this->mostrarMatriz=0;
         this->otimizado=0;
         this->tempo=false;
      }
      void processar();
} prime; // � necess�rio apenas uma instancia de prime, j� declarada aqui

//fun��o auxiliar compara tr�s inteiros e devolve o maior
int maior(int x, int b, int c){
  if((x <= b) && (x <= c)) return x;
  else if((b <= x) && (b <= c)) return b;
  else return c;
}

//fun��o auxiliar recebe os par�metros que o usu�rio digitou, valida e transforma os valores
int parseParametros(int argc, char** argv){
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

      if(strcmp(argv[z], "-st") == 0){
        if(argc == 7) return 0;
        prime.tempo=true;
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

   ifstream arquivo(prime.alpha.c_str());

   if (arquivo.is_open()){
     getline(arquivo, prime.alpha);
   }
   arquivo.close();

   if (arquivo.is_open()){
     getline(arquivo, prime.beta);
   }
   arquivo.close();

   return cc;
}


//O algoritmo foi adaptado para entregar o resultado do primer que � o inverso da programa��o original
//Classe que resolve o problema k-mismatches problems em tempo O(mn)
//Essa classe � abstrata permitindo a implementa��o do comportamento do algoritmo (Original e Otimizado)
class KdifferenceInexactMatch1{

  protected:
    int **D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]
    string t;     //texto
    string a;     //padr�o
    int k;        //quantidade de difer�n�as
    int m, n;     //tamanho de a e t respectivamente
    int rowPrint; //vari�vel auxiliar que recebe o valor limite da linha de impress�o na tela, linha m�xima
  public:
    int linha;    //linha que cont�m todos os valores >= k

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
    void imprimirMatrizTela() const; //permite o cacheamento pois n�o muda o comportamento
    virtual void setA(string){};
};

//implementa��o original Matriz D[0..m, 0..n]
class KdifferenceInexactMatch1Original: public KdifferenceInexactMatch1{
 public:
   KdifferenceInexactMatch1Original(string a, string t, int k): KdifferenceInexactMatch1(a,t,k) {
       //aloca��o din�mica de inteiros, necess�rio mais um que � a posi��o 0
       this->D = new (nothrow) int*[m + 1];
       for (int j = 0; j <= m; ++j)
          this->D[j] = new (nothrow) int[n + 1];

       this->rowPrint = m; //m � a linha m�xima de impress�o na tela
   }
   ~KdifferenceInexactMatch1Original(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       for (int i = 0; i <= m; ++i)
         delete [] D[i];
       delete [] D;
    }

    void executar();

    /**
      este m�todo tem a responsabilidade de desalocar a �ltima linha da matriz D
      quando uma nova string 'a' for alocada. Isso garante que a primeira chamada do algoritmo
      usa a maior quantidade de mem�ria poss�vel (m+1 x n+1) e a cada nova chamada
      uma linha 'desnecess�ria' � removida
    */
    void setA(string a){
       int mm = this->a.size();
       int mmm = a.size();

       while(mm > mmm)
          delete [] D[mm--];

      this->a = a;
      this->m = mmm;
    }
};

//implementa��o original Matriz D[0..1, 0..n]
class KdifferenceInexactMatch1Optimizado: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado(string a, string t, int k):KdifferenceInexactMatch1(a,t,k){

       //aloca��o de 2 linhas de inteiros por n + 1
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

   //aqui n�o h� a necessidade de desalocar a �ltima linha, pois a matriz j� � m�nima
   void setA(string a){
      this->a = a;
      this->m = a.size();
    }
};

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch1::imprimirMatrizTela() const{

    cout << setfill(' ');          //setar o tamanho de espa�amento de elementos na tela
	cout<<"\n     ";               //espa�amento necess�rio para o cabe�alho

	for(int i = 0; i <= n; i++)
	   cout<<setw(2)<<t[i] << " "; //imprimi o cabe�alho da matriz com 2 espa�os

	cout<<"\n";

    //percorre toda a matriz D
	for(int i = 0; i <= rowPrint; i++){

		if(i == 0 ) cout<<"  ";
		else cout<<a[i - 1]<<" "; // Coluna cabe�alho lateral

		for(int l = 0; l <= n; l++){
			cout<<setw(2)<<D[i][l] << " "; //imprimi o valor da matriz
		}
		cout<<"\n";
	}
}

//m�todo que executa o comportamento original do algoritmo
void KdifferenceInexactMatch1Original::executar(){
   int i, l;

   for(l = 0; l <= n; l++) /** for all l, 0 <= l <= n, D[0,l] <- 0 */
	  D[0][l] = 0;
   for(i = 1; i <= m; i++) /** for all i, 1 <= i <= m, D[i,0] <- i */
	  D[i][0] = i;

   bool passou; //flag para verificar se toda a linha tem valores >= k
   linha = -1;  //flag que marca a primeira linha em que a condi��o acima foi satisfeita
	for(i = 1; i <= m; i++){ /** for i <- 1 to m */
        passou = true; //come�a com aceitando que a linha toda � valida

		for(l = 1; l <= n; l++){ /** for l <- 1 to n */
            /** rela��o de recorr�ncia */
			D[i][l] = maior(D[i-1][l] + 1,
                            D[i][l-1] + 1,
                            a.compare(i-1, 1, t, l-1, 1) == 0 ? D[i-1][l-1] : D[i-1][l-1] + 1);

			if(D[i][l] < k) passou = false; //se algum valor da linha � < k essa linha � descartada
		}
        if(passou && linha == -1) linha = i; //se a linha n�o foi descartada e ela � a primeira, guarda ela
	}
}

//m�todo que executa o comportamento otimizado do algoritmo
void KdifferenceInexactMatch1Optimizado::executar(){
   int l;

   for(l = 0; l <= n; l++) /** necess�rio inicializar apenas a primeira linha */
	  D[0][l] = 0;

   bool passou; //flag para verificar se toda a linha tem valores >= k
   linha = -1;  //flag que marca a primeira linha em que a condi��o acima foi satisfeita
   for(int i = 1; i <= m && linha == -1; i++){
     D[1][0] = i;
	 for(l = 1; l <= n; l++)
        D[1][l] = maior(D[0][l] + 1,
                        D[1][l-1] + 1,
                        a.compare(i-1, 1, t, l-1, 1) == 0 ? D[0][l-1] : D[0][l-1] + 1);

     /**
       aqui vai verificar se a linha tem todos os valores >= k
       ocorre tamb�m o transporte dos valores da linha 1 para 0 a cada nova itera��o
       n�o h� necessidade de terminar a execu��o se a linha j� foi alcan�ada por isso o for tem a condi��o 'linha == -1'
     */
     passou = true;
	 for(l = 0; l <= n; l++){
        if(passou && D[1][l] < k) passou = false; //se algum valor da linha � < k essa linha � descartada
        D[0][l] = D[1][l];
        D[1][l] = 0;
     }
     if(passou) linha = i; //se a linha n�o foi descartada ela � a primeira
   }
}

//m�todo que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: n�o h� execu��o sem a invoca��o deste m�todo
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorr�ncias

   if(otimizado) //se o usu�rio escolheu a vers�o otimizada
      c = new KdifferenceInexactMatch1Optimizado(alpha, beta, k); //instancia a classe Otimizada
   else
      c = new KdifferenceInexactMatch1Original(alpha, beta, k);  //instancia a classe Original

   /*
     para todo �ndice j em alpha, aqui n�o h� necessidade de ir at� m
     pois � necess�rio k caracteres para existir compara��o
   */
   for(int j = 0; j < (m - k) + 1; j++){
       c->setA(alpha.substr(j));
       c->executar();

       if(c->linha > -1){ //se a linha foi obtida, significa que existe uma ocorr�ncia
           cout<<MSG_N_OCCR<<++ocr<<" em: "<<j<<", "<<j + c->linha<<" "<<alpha.substr(j, c->linha)<<endl;
           if(mostrarMatriz) c->imprimirMatrizTela();
       }
   }
   if(ocr == 0) cout<<MSG_0_OCCR<<k<<" diferenca(s)"<<endl;
}

int main(int argc, char** argv) {

   if (argc < 7 || argc > 9) {
	  cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   if(parseParametros(argc, argv) != 3){
      cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   prime.m = prime.alpha.size();
   prime.n = prime.beta.size();

   if(prime.k > prime.m){
     cout<<endl<<ERR_KMAIOR<<endl;
     return 0;
   }

   time_t inicio, fim;

   inicio = clock();
   prime.processar();
   fim = clock();

   if(prime.tempo)
     cout<<"Tempo de execucao: "<< ((fim - inicio) / (CLOCKS_PER_SEC / 1000)) << " milisegundos";

   return 1;
}
