/* *******************************************************************
 *  Autor: Jean Alexandre Dobre                                      *
 *  Orientador: Said Sadique Adi                                     *
 *  Ano: 2015/2016                                                   *
 *  FACOM: Mestrado em Ciência da Computação                         *
 *                                                                   *
 * Este projeto implementa a versão original do k-difference inexact *
 * match por meio da matriz D [m x n], algoritmo serial, proposto    *
 * por Landau Gad M. e Vishkin Uzi no artigo INTRODUCING EFFICIENT   *
 * PARALLELISM INTO APROXIMATE STRING MATCHING AND A NEW SERIAL      *
 * ALGORITHM, página 222.                                            *
 *                                                                   *
 * A versão do k-difference primer foi idealizada pelo               *
 * Prof. Dr. Said Sadique Adi cuja idéia é verificar toda linha da   *
 * matriz D tem valor >= k.                                          *
 * A versão otimizada traz a i´deia de implementar a matriz D com    *
 * apenas 2 linhas [2 x n] que é o mínimo necessário para que a      *
 * recorrência aconteça. Essa versão diminui muito o uso de memória  *
 * mas não de processamento.
 *
 * Atualizado em 09/03/2016 versão com opt-2 que deve computar a     *
 * matriz  D1..m + 1 pivo e impentantação de leitura arquivo FASTA   *
 * e agrupamento em forma de projeto, removendo as funções auxilares *
 * para uma biblioteca de uso geral e por último a implementação de  *
 * um vetor<string> para guardar os resultados indexando pelo sufixo *
 * *******************************************************************/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <pthread.h>
#include "classes.h"
#include "auxiliar.h"

//declaração da classe que é utilizada no KdifferencePrime
class KdifferenceInexactMatch1;

class KdifferencePrime1: public KdifferencePrime{

    public:
      void processar();

} prime; // é necessário apenas uma instancia de prime, já declarada aqui

//O algoritmo foi adaptado para entregar o resultado do primer que é o inverso da programação original
//Classe que resolve o problema k-mismatches problems em tempo O(mn)
//Essa classe é abstrata permitindo a implementação do comportamento do algoritmo (Original e Otimizado)
class KdifferenceInexactMatch1: public KdifferenceInexactMatch{

  protected:
    int **D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]

  public:
    KdifferenceInexactMatch1(char *a, char *t, int *k): KdifferenceInexactMatch(a,t,k){};
    ~KdifferenceInexactMatch1(){};
    virtual int executar(){};//executa o algoritmo kdifference inexact match e retorna a linha da ocorrência de prime
    void imprimirMatrizTela() const{

      cout << setfill(' ');          //setar o tamanho de espaçamento de elementos na tela
      cout<<endl<<"     ";               //espaçamento necessário para o cabeçalho

      for(int i = 0; i <= n; i++)
         cout<<setw(2)<<t[i] << " "; //imprimi o cabeçalho da matriz com 2 espaços

      cout<<endl;

       //percorre toda a matriz D
      for(int i = 0; i <= rowPrint; i++){

         if(i == 0) cout<<"  ";
         else cout<<a[prime.j + i - 1]<<" "; // Coluna cabeçalho lateral

         for(int l = 0; l <= n; l++){
            cout<<setw(2)<<D[i][l] << " "; //imprimi o valor da matriz
         }
         cout<<endl;
      }
   }

    virtual void setaTamanhoM(){};
};

//implementação original Matriz D[0..m, 0..n]
class KdifferenceInexactMatch1Original: public KdifferenceInexactMatch1{
 public:
   KdifferenceInexactMatch1Original(char *a, char *t, int *k): KdifferenceInexactMatch1(a,t,k) {
       //alocação dinâmica de inteiros, necessário mais um que é a posição 0
       this->D = new (nothrow) int*[m + 1];
       for (int j = 0; j <= m; ++j)
          this->D[j] = new (nothrow) int[n + 1];

       this->rowPrint = m ; //m é a linha máxima de impressão na tela
   }

   ~KdifferenceInexactMatch1Original(){
       //Ao destruir a classe desaloca toda memória que foi usada
       for (int i = 0; i <= m; ++i)
         delete [] D[i];
       delete [] D;
    }

    //método que executa o comportamento original do algoritmo
    int executar(){
      int i, l;

      for(l = 0; l <= n; l++) /** for all l, 0 <= l <= n, D[0,l] <- 0 */
        D[0][l] = 0;
      for(i = 1; i <= m; i++) /** for all i, 1 <= i <= m, D[i,0] <- i */
        D[i][0] = i;

      bool passou; //flag para verificar se toda a linha tem valores >= k
      int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
      for(i = 1; i <= m && linha == -1; i++){ /** for i <- 1 to m */
           passou = true; //começa com aceitando que a linha toda é valida

         for(l = 1; l <= n; l++){ /** for l <- 1 to n */
            /** relação de recorrência */
            D[i][l] = menorDeTres(D[i-1][l] + 1,
                                  D[i][l-1] + 1,
                                  D[i-1][l-1] + (a[prime.j + i-1] == t[l-1] ? 0 : 1));


            if(passou && D[i][l] < k) passou = false; //se algum valor da linha é < k essa linha é descartada
         }
         if(passou && linha == -1) linha = i; //se a linha não foi descartada e ela é a primeira, guarda ela
      }
      return linha;
   }

    /**
      este método tem a responsabilidade de desalocar a última linha da matriz D
      quando um novo índice j de 'a' for passado. Isso garante que a primeira chamada do algoritmo
      usa a maior quantidade de memória já alocada (m+1 x n+1) e a cada nova chamada
      uma linha 'desnecessária' é removida
    */
    void setaTamanhoM(){
       int mmm = prime.m - prime.j;

       while(this->m > mmm)
          delete [] D[this->m--];

      this->m = mmm;
      this->rowPrint = this->m;
    }
};

//implementação Matriz D[0..1, 0..n]
class KdifferenceInexactMatch1Optimizado1: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado1(char *a, char *t, int *k):KdifferenceInexactMatch1(a,t,k){

       //alocação de 2 linhas de inteiros por n + 1
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

   //método que executa o comportamento otimizado do algoritmo com 2 linhas
   int executar(){
      int l;

      for(l = 0; l <= n; l++) /** necessário inicializar apenas a primeira linha */
        D[0][l] = 0;

      bool passou; //flag para verificar se toda a linha tem valores >= k
      int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
      for(int i = 1; i <= m && linha == -1; i++){
        D[1][0] = i;
        passou = true;
       for(l = 1; l <= n; l++){
           D[1][l] = menorDeTres(D[0][l]   + 1,
                                 D[1][l-1] + 1,
                                 D[0][l-1] + (a[prime.j + i-1] == t[l-1] ? 0 : 1));

           if(passou && D[1][l] < k) passou = false; //se algum valor da linha é < k essa linha é descartada
           //aqui reorganiza a matriz para o próximo laço
           D[0][l-1] = D[1][l-1];
           D[1][l-1] = 0;
        }
        D[0][n] = D[1][n];
        D[1][n] = 0;
        if(passou) linha = i; //se a linha não foi descartada ela é a primeira
      }
      return linha;
   }

   //aqui não há a necessidade de desalocar a última linha, pois a matriz já é mínima
   void setaTamanhoM(){
      this->m = prime.m - prime.j;
    }
};

//implementação Matriz D[0, 0..n]
class KdifferenceInexactMatch1Optimizado2: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado2(char *a, char *t, int *k):KdifferenceInexactMatch1(a,t,k){

       //alocação de 1 linha de inteiros por n + 1
       this->D = new (nothrow) int*[1];
       this->D[0] = new (nothrow) int[n + 1];

       this->rowPrint = 0;
   }

   ~KdifferenceInexactMatch1Optimizado2(){
       delete [] D[0];
       delete [] D;
    }

   //método que executa o comportamento otimizado do algoritmo
   int executar(){
      int l;

      for(l = 0; l <= n; l++) /** necessário inicializar apenas a primeira linha */
        D[0][l] = 0;

      bool passou; //flag para verificar se toda a linha tem valores >= k
      int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
      int pivo,aux;


      for(int i = 1; i <= m && linha == -1; i++){
        pivo = i;
        passou = true;
       for(l = 1; l <= n; l++){
           //cmp = a.at(i-1) == t.at(l-1) ? 0 : 1;//essa comparação direta é muito mais rápida
           aux = menorDeTres(D[0][l] + 1,
                                pivo + 1,
                                D[0][l-1] + (a[prime.j + i-1] == t[l-1] ? 0 : 1));
          D[0][l-1] = pivo;
          if(l == n) D[0][l] = aux;
          pivo = aux;
          if(passou && pivo < k) passou = false;
       }
        if(passou) linha = i; //se a linha não foi descartada ela é a primeira
      }
      return linha;
   }

   //aqui não há a necessidade de desalocar a última linha, pois a matriz já é mínima
   void setaTamanhoM(){
      this->m = prime.m - prime.j;
      this->rowPrint = this->m;
    }
};

struct thread_data{
   int id;
   int x;
   int y;
   int versao;
};

void *teste(void *p){

 struct thread_data *my;
 my = (struct thread_data *) p;

 //KdifferencePrime1 *nw = &prime;
 KdifferencePrime1 *nw = new KdifferencePrime1();
 //nw = &prime;

  if(my->versao == 1) //se o usuário escolheu a versão otimizada 2
    nw->c = new KdifferenceInexactMatch1Optimizado2(prime.alpha, prime.beta, &prime.k); //instancia a classe Otimizada
  else if(my->versao == 2) //se o usuário escolheu a versão otimizada
    nw->c = new KdifferenceInexactMatch1Optimizado1(prime.alpha, prime.beta, &prime.k); //instancia a classe Otimizada
  else  //se o usuário escolheu a versão normal
    nw->c = new KdifferenceInexactMatch1Original(prime.alpha, prime.beta, &prime.k);  //instancia a classe Original

 /*
     para todo índice j em alpha, aqui não há necessidade de ir até m
     pois é necessário k caracteres para existir comparação
   */
 for(nw->j = my->x; nw->j < my->y; nw->j++){
      nw->c->setaTamanhoM();
      int r = nw->c->executar();
      if(r > -1){ //se a linha foi obtida, significa que existe uma ocorrência

       // Primer *pr = new Primer(++ocr,j, r, ocorrencia.substr(j, r));
        Primer *pr = new Primer(my->id, nw->j, r, prime.ocorrencia.substr(nw->j, r));
        prime.primers.insert(prime.primers.end(), pr);
        if(prime.mostrarMatriz) nw->c->imprimirMatrizTela();
      }
   }
   //termina a execução da thread
   pthread_exit(NULL);
}

//método que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: não há execução sem a invocação deste método
void KdifferencePrime1::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorrências

  /* if(versao == 1) //se o usuário escolheu a versão otimizada 2
      c = new KdifferenceInexactMatch1Optimizado2(alpha, beta, &k); //instancia a classe Otimizada
   else if(versao == 2) //se o usuário escolheu a versão otimizada
      c = new KdifferenceInexactMatch1Optimizado1(alpha, beta, &k); //instancia a classe Otimizada
   else  //se o usuário escolheu a versão normal
      c = new KdifferenceInexactMatch1Original(alpha, beta, &k);  //instancia a classe Original

   */
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
   prime.alpha = new char[p->alpha.length() + 1];
   strcpy(prime.alpha, p->alpha.c_str());
   prime.beta = new char[p->beta.length() + 1];
   strcpy(prime.beta, p->beta.c_str());
   prime.k = p->k;
   prime.mostrarMatriz = p->mostrarMatriz;
   prime.versao = p->versao;

   prime.m = p->alpha.size();
   prime.n = p->beta.size();
   prime.ocorrencia = p->alpha;

   bool tempo = p->mostrarTempo;

   delete p;

   if(prime.k > prime.m){
     cout<<endl<<ERR_KMAIOR<<prime.m<<endl;
     return 0;
   }

   cout<<"K-difference-primer-1 processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;

   time_t inicio, fim;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();

   if(tempo)
     cout<<"Tempo de execucao: "<< ((fim - inicio) / (CLOCKS_PER_SEC / 1000)) << " milisegundos"<<endl;

   return 1;
}
