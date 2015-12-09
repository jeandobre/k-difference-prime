/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ci�ncia da Computa��o

   Este projeto implementa a vers�o modificada do k-difference inexact match por meio da
   matriz L [d x e], algoritmo serial, proposto por Landau Gad M. e Vishkin Uzi no artigo
   FAST PARALLEL AND SERIAL APPROXIMATE STRING MATCHING
   p�gina 5.

   A vers�o do k-difference primer � a abordagem do livro ALGORITHMS ON STRINGS, TREES, AND SEQUENCES de Gusfiel
   que avalia a coluna k-1 procurando se 'm' foi alcan�ado, se n�o foi alcan�ado existe um prime com pelo menos k diferen�as
   e ele est� no maior valor da coluna k-1.
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <fstream>

using namespace std;

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-st] \tmostrar o tempo de execucao"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 0 e m. "
#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcorrencia "

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch2;

class KdifferencePrime{

    public:
      string alpha, beta;//seguindo o modelo proposto por GUSFIELD, nomes de vari�veis que representam o padr�o e o texto respectivamente
      int k;             //quantidade de difer�n�as
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      int mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int otimizado;     //booleano 0, 1 usado para executar a vers�o otimizada do algoritmo
      bool tempo;        //booleano 0, 1 usado para imprime o tempo na tela (O tempo � calculado independentemente)
      KdifferenceInexactMatch2 *c;

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usu�rio n�o escolher nada
         this->mostrarMatriz=0;
         this->otimizado=0;
         this->tempo=false;
      }
      void processar();
} prime;

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

//fun��o auxiliar que compara dois inteiros e devolve o menor
int minimo(int a, int b){
   return a <= b ? a : b;
}

//funcao auxiliar que compara tres inteiros e devolve o maior
int maximo(int a, int b, int c){
   if(a >= b && a >= c) return a;
   else if(b >= a && b >= c) return b;
   else return c;
}


//O algoritmo foi adaptado para entregar o resultado do primer que � o inverso da programa��o original
//Classe que resolve o problema k-mismatches problems em tempo O(nk)
class KdifferenceInexactMatch2{

  private:
    int **L;   //Matriz L[-(k+1)..d, -1..e]
    string a;  //padr�o
    string t;  //texto
    int k;     //quantidade de difer�n�as
    int m, n;  //tamanho de a e t respectivamente
  public:
    int linha; //linha que cont�m o menor valor >= k

  private:
    int getMinD(){
      return -(k + 1); //menor valor d
    }
    int getMaxD(){
      return n + 1;  //maior valor d
    }
    int dToMatriz(int d){
      return d + (k + 1); //converte �ndice d para �ndices reais
    }

    int getMinE(){ //menor valor e
      return -1;
    }

    int getMaxE(){//maior valor e
      return k;
    }

    int eToMatriz(int e){
      return e + 1; //converte �ndice e para �ndices reais
    }

    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor; //L[d,e] = valor
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //k+1, n+1, 1 da posi��o 0
    }

    int getDimensaoE(){
      return k + 2; // k + 1, 1 da posi��o 0
    }

  public:
    KdifferenceInexactMatch2(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();

       //aloca��o din�mica de inteiros
       this->L = new int*[getDimensaoD()];
       for (int j = 0; j < getDimensaoD(); ++j)
          this->L[j] = new int[getDimensaoE()];

    }

    ~KdifferenceInexactMatch2(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       for (int i = 0; i < getDimensaoD(); ++i)
         delete [] L[i];
       delete [] L;
    }
    void executar();
    void escreverMatrizTela();

    void setA(string a){
      this->a = a;
      this->m = a.size(); //atualiza o valor de 'm' com o novo tamanho de 'a'
    }
};

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch2::escreverMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela

    cout<<endl<<setw(4)<<" "; //espa�amento necess�rio para o cabe�alho
	for(int i = getMinE(); i <= getMaxE(); i++) cout<<setw(2)<<i<< " "; //imprimi o cabe�alho da matriz com 2 espa�os

	int vr;
    cout<<endl;
    for(int i = getMinD(), x = 0; i <= getMaxD(); i++, x++){

        cout<<setw(3)<<i<<" ";
        for(int l = getMinE(), y = 0; l <= getMaxE(); l++, y++){
            vr = getL(i,l);

            if ((x <= k || y <= k) && (x+y) <= k)cout<<setw(3)<<" ";
			else cout<<setw(2)<<vr<<" ";
		}
		cout<<"\n";
	}
}

//m�todo que executa o comportamento original do algoritmo
void KdifferenceInexactMatch2::executar(){
    int d,e, row;

	for(d = 0; d <= n; d++) /** for all d, 0 <= d <= n, L[d,-1] <= -1 */
        setL(d, - 1, -1);

    for(d = -(k+1); d <= -1; d++){ /** for all d, -(k+1) <= d <= -1 */
      setL(d, abs(d) - 2, abs(d) - 2); /** L[d,|d|-1] <= |d|-1 */
      setL(d, abs(d) - 1, abs(d) - 1); /** L[d,|d|-2] <= |d|-2 */
    }

    for(e = -1; e<= k; e++) /** for all e, -1 <= e <= k, L[n+1, e] <= -1 */
        setL(getMaxD(), e, -1);

    bool passou = true;
    linha = -1;
    for(e = 0; e <= k && passou; e++){
        for(d = -(e); d <= n; d++){
           row = maximo(getL(d-1, e-1),
                        getL(d,   e-1) + 1,
                        getL(d+1, e-1) + 1);
           row = minimo(row, m);
           //LCE
           while(row < m && row+d < n && a.compare(row, 1, t, row+d, 1) == 0)
             row++;

           setL(d, e, row);
           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m && e < k) passou = false;
        }
    }

    /*
       se passou ent�o existe uma coluna 'e' = k-1 com a maior �ndice
       basta localizar na linha qual � o maior valor e incrementar 1
       isso ocorre pois a matriz guarda o �ndice final do LCE, logo o inicio do LCE(k) � LCE(k-1)+1.
    */
    if(passou){
        row = -1;
        for(d = -(k+1); d <= n; d++){
            row = getL(d, k-1);
            if (row > linha) linha = row;
        }
        linha++;
    }
    /*
       (a) Para todo i, l, D[i,l] - D[i-1,l-1] � ou Zero ou 1
       (b) d > n - m + k + 1 ou d < -k s�o descart�veis para solu��o deste problema
       (c) a inicializa��o apenas atualiza a matriz com os limites
    */
}

//m�todo que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: n�o h� execu��o sem a invoca��o deste m�todo
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorr�ncias

   c = new KdifferenceInexactMatch2(alpha, beta, k); //instancia apenas uma vez

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
