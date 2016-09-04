/* *******************************************************************
 *  Autor: Jean Alexandre Dobre                                      *
 *  Orientador: Said Sadique Adi                                     *
 *  Ano: 2015/2016                                                   *
 *  FACOM: Mestrado em Ci�ncia da Computa��o                         *
 *                                                                   *
 * Este projeto implementa a vers�o original do k-difference inexact *
 * match por meio da matriz D [m x n], algoritmo serial, proposto    *
 * por Landau Gad M. e Vishkin Uzi no artigo INTRODUCING EFFICIENT   *
 * PARALLELISM INTO APROXIMATE STRING MATCHING AND A NEW SERIAL      *
 * ALGORITHM, p�gina 222.                                            *
 *                                                                   *
 * A vers�o do k-difference primer foi idealizada pelo               *
 * Prof. Dr. Said Sadique Adi cuja id�ia � verificar toda linha da   *
 * matriz D tem valor >= k.                                          *
 * A vers�o otimizada traz a i�deia de implementar a matriz D com    *
 * apenas 2 linhas [2 x n] que � o m�nimo necess�rio para que a      *
 * recorr�ncia aconte�a. Essa vers�o diminui muito o uso de mem�ria  *
 * mas n�o de processamento.
 *
 * Atualizado em 09/03/2016 vers�o com opt-2 que deve computar a     *
 * matriz  D1..m + 1 pivo e impentanta��o de leitura arquivo FASTA   *
 * e agrupamento em forma de projeto, removendo as fun��es auxilares *
 * para uma biblioteca de uso geral e por �ltimo a implementa��o de  *
 * um vetor<string> para guardar os resultados indexando pelo sufixo *
 * *******************************************************************/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <pthread.h>
#include "classes.h"

//O algoritmo foi adaptado para entregar o resultado do primer que � o inverso da programa��o original
//Classe que resolve o problema k-mismatches problems em tempo O(mn)
//Essa classe � abstrata permitindo a implementa��o do comportamento do algoritmo (Original e Otimizado)
class KdifferenceInexactMatch1: public KdifferenceInexactMatch{

  protected:
    int **D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]
    int j;        //�ndice de j

  public:
    KdifferenceInexactMatch1(char *a, char *t, int *k): KdifferenceInexactMatch(a,t,k){};
    ~KdifferenceInexactMatch1(){};
    virtual int executar(int m, int j){};//executa o algoritmo kdifference inexact match e retorna a linha da ocorr�ncia de prime
    void imprimirMatrizTela() const;
};

//implementa��o original Matriz D[0..m, 0..n]
class KdifferenceInexactMatch1Original: public KdifferenceInexactMatch1{
 public:
   KdifferenceInexactMatch1Original(char *a, char *t, int *k): KdifferenceInexactMatch1(a,t,k) {
       //aloca��o din�mica de inteiros, necess�rio mais um que � a posi��o 0
       this->D = new (nothrow) int*[m + 1];
       for (int j = 0; j <= m; ++j)
          this->D[j] = new (nothrow) int[n + 1];

       this->rowPrint = m ; //m � a linha m�xima de impress�o na tela
   }

   ~KdifferenceInexactMatch1Original(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       for (int i = 0; i <= m; ++i)
         delete [] D[i];
       delete [] D;
    }

    int executar(int m, int j);
};

//implementa��o Matriz D[0..1, 0..n]
class KdifferenceInexactMatch1Optimizado1: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado1(char *a, char *t, int *k):KdifferenceInexactMatch1(a,t,k){

       //aloca��o de 2 linhas de inteiros por n + 1
       this->D = new (nothrow) int*[2];
       this->D[0] = new (nothrow) int[n + 1];
       this->D[1] = new (nothrow) int[n + 1];

       this->rowPrint = 1;
   }

   ~KdifferenceInexactMatch1Optimizado1(){
       delete [] D[0];
       delete [] D[1];
       delete [] D;
    }
    int executar(int m, int j);
//    int executar(int m) override { return executar(m, int j); }
};

//implementa��o Matriz D[0, 0..n]
class KdifferenceInexactMatch1Optimizado2: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado2(char *a, char *t, int *k):KdifferenceInexactMatch1(a,t,k){

       //aloca��o de 1 linha de inteiros por n + 1
       this->D = new (nothrow) int*[1];
       this->D[0] = new (nothrow) int[n + 1];

       this->rowPrint = 0;
   }

   ~KdifferenceInexactMatch1Optimizado2(){
       delete [] D[0];
       delete [] D;
    }

    int executar(int m, int j);
};

class KdifferencePrime1: public KdifferencePrime{

    public:
      void processar();
      void instanciar(){
        if(versao == 1) //se o usu�rio escolheu a vers�o otimizada 2
          c = new KdifferenceInexactMatch1Optimizado2(alpha, beta, &k); //instancia a classe Otimizada
        else if(versao == 2) //se o usu�rio escolheu a vers�o otimizada
          c = new KdifferenceInexactMatch1Optimizado1(alpha, beta, &k); //instancia a classe Otimizada
        else  //se o usu�rio escolheu a vers�o normal
          c = new KdifferenceInexactMatch1Original(alpha, beta, &k);  //instancia a classe Original
      };

} prime; // � necess�rio apenas uma instancia de prime, j� declarada aqui

void KdifferenceInexactMatch1::imprimirMatrizTela() const{

      cout << setfill(' ');          //setar o tamanho de espa�amento de elementos na tela
      cout<<endl<<"     ";               //espa�amento necess�rio para o cabe�alho

      for(int i = 0; i <= n; i++)
         cout<<setw(2)<<t[i] << " "; //imprimi o cabe�alho da matriz com 2 espa�os

      cout<<endl;

       //percorre toda a matriz D
      for(int i = 0; i <= rowPrint; i++){

         if(i == 0) cout<<"  ";
         else cout<<a[j + i - 1]<<" "; // Coluna cabe�alho lateral

         for(int l = 0; l <= n; l++){
            cout<<setw(2)<<D[i][l] << " "; //imprimi o valor da matriz
         }
         cout<<endl;
      }
}

//m�todo que executa o comportamento original do algoritmo
int KdifferenceInexactMatch1Original::executar(int m, int j){
      /**
         desalocar a �ltima linha da matriz D
         quando um novo �ndice j de 'a' for passado. Isso garante que a primeira chamada do algoritmo
         usa a maior quantidade de mem�ria j� alocada (m+1 x n+1) e a cada nova chamada
         uma linha 'desnecess�ria' � removida
       */

       while(this->m > m)
          delete [] D[this->m--];

      this->m = m;
      this->rowPrint = m;
      this->j = j;

      int i, l;

      for(l = 0; l <= n; l++) /** for all l, 0 <= l <= n, D[0,l] <- 0 */
        D[0][l] = 0;
      for(i = 1; i <= m; i++) /** for all i, 1 <= i <= m, D[i,0] <- i */
        D[i][0] = i;

      bool passou; //flag para verificar se toda a linha tem valores >= k
      int linha = -1;  //flag que marca a primeira linha em que a condi��o acima foi satisfeita
      for(i = 1; i <= m && linha == -1; i++){ /** for i <- 1 to m */
           passou = true; //come�a com aceitando que a linha toda � valida

         for(l = 1; l <= n; l++){ /** for l <- 1 to n */
            /** rela��o de recorr�ncia */
            D[i][l] = menorDeTres(D[i-1][l] + 1,
                                  D[i][l-1] + 1,
                                  D[i-1][l-1] + (a[j + i-1] == t[l-1] ? 0 : 1));


            if(passou && D[i][l] < k) passou = false; //se algum valor da linha � < k essa linha � descartada
         }
         if(passou && linha == -1) linha = i; //se a linha n�o foi descartada e ela � a primeira, guarda ela
      }
      return linha;
}

//m�todo que executa o comportamento otimizado do algoritmo com 2 linhas
int KdifferenceInexactMatch1Optimizado1::executar(int m, int j){
      this->m = m;
      this->j = j;
      int l;

      for(l = 0; l <= n; l++) /** necess�rio inicializar apenas a primeira linha */
        D[0][l] = 0;

      bool passou; //flag para verificar se toda a linha tem valores >= k
      int linha = -1;  //flag que marca a primeira linha em que a condi��o acima foi satisfeita
      for(int i = 1; i <= m && linha == -1; i++){
        D[1][0] = i;
        passou = true;
       for(l = 1; l <= n; l++){
           D[1][l] = menorDeTres(D[0][l]   + 1,
                                 D[1][l-1] + 1,
                                 D[0][l-1] + (a[j + i-1] == t[l-1] ? 0 : 1));

           if(passou && D[1][l] < k) passou = false; //se algum valor da linha � < k essa linha � descartada
           //aqui reorganiza a matriz para o pr�ximo la�o
           D[0][l-1] = D[1][l-1];
           D[1][l-1] = 0;
        }
        D[0][n] = D[1][n];
        D[1][n] = 0;
        if(passou) linha = i; //se a linha n�o foi descartada ela � a primeira
      }
      return linha;
}

//m�todo que executa o comportamento otimizado do algoritmo
int KdifferenceInexactMatch1Optimizado2::executar(int m, int j){
   this->m = m;
   this->rowPrint = m;
   this->j = j;

   int l;

   for(l = 0; l <= n; l++) /** necess�rio inicializar apenas a primeira linha */
     D[0][l] = 0;

   bool passou; //flag para verificar se toda a linha tem valores >= k
   int linha = -1;  //flag que marca a primeira linha em que a condi��o acima foi satisfeita
   int pivo,aux;


   for(int i = 1; i <= m && linha == -1; i++){
     pivo = i;
     passou = true;
    for(l = 1; l <= n; l++){
        //cmp = a.at(i-1) == t.at(l-1) ? 0 : 1;//essa compara��o direta � muito mais r�pida
        aux = menorDeTres(D[0][l] + 1,
                             pivo + 1,
                             D[0][l-1] + (a[j + i-1] == t[l-1] ? 0 : 1));
       D[0][l-1] = pivo;
       if(l == n) D[0][l] = aux;
       pivo = aux;
       if(passou && pivo < k) passou = false;
    }
     if(passou) linha = i; //se a linha n�o foi descartada ela � a primeira
   }
   return linha;
}

struct thread_data{
   int id;
   int x;
   int y;
   int versao;
};

void *teste(void *p){

 struct thread_data *my;
 my = (struct thread_data *) p;

 KdifferencePrime1 *nw = new KdifferencePrime1();
 nw->alpha = prime.alpha;
 nw->beta = prime.beta;
 nw->k = prime.k;
 nw->m = prime.m;
 nw->n = prime.n;

 nw->instanciar();
 int r = -1;
 for(nw->j = my->x; nw->j < my->y; nw->j++){
      r = nw->c->executar(nw->m - nw->j, nw->j);
      if(r > -1){ //se a linha foi obtida, significa que existe uma ocorr�ncia
        Primer *pr = new Primer(nw->j, nw->j, r, prime.ocorrencia.substr(nw->j, r));
        prime.primers.insert(prime.primers.end(), pr);
        if(prime.mostrarMatriz) nw->c->imprimirMatrizTela();
      }
   }
   //termina a execu��o da thread
   pthread_exit(NULL);
}

//m�todo que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: n�o h� execu��o sem a invoca��o deste m�todo
void KdifferencePrime1::processar(){

   pthread_t threads[4];//abre 4 treads para executar todos os j
   struct thread_data td[4];
   int mod = (m / 4);
   for(int rc = 0; rc < 4; rc++){
      td[rc].id = rc;
      td[rc].x = 0 + (rc * 1 * mod);
      td[rc].y = mod * (rc + 1) - (rc == 3 ? k : 0);
      td[rc].versao = versao;
      pthread_create(&threads[rc], NULL, teste, (void *)&td[rc]);
   }

  //junta todas as threads
  void *status;
  for(int rc = 0; rc < 4; rc++)
    pthread_join(threads[rc], &status);
}

int main(int argc, char** argv) {

   if (argc < 7 || argc > 10) {
	  cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   Parametro *p = parseParametros(argc, argv);
   if(p->total != 3){
      cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   prime.setaParametros(p);

   if(prime.k > prime.m){
     cout<<endl<<ERR_KMAIOR<<prime.m<<endl;
     return 0;
   }

   if(prime.versao > 3 || prime.versao < 1){
     cout<<MSG_VERSAO_INCORRETA;
     cout<<MSG_VERSAO_K1_VS1<<MSG_VERSAO_K1_VS2<<MSG_VERSAO_K1_VS3;
     return 0;
   }

   cout<<"K-difference-primer-1 (Com threads) processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;

   time_t inicio, fim;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(prime.tempo) formataTempo(tempo_execucao);

   return 1;
}
