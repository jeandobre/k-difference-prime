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
#include "classes.h"

//implementação Matriz D[0, 0..n]
class KdifferenceInexactMatch1Modificado: public KdifferenceInexactMatch{
  protected:
    int *D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]

  public:
   KdifferenceInexactMatch1Modificado(char *a, char *t, int *k):KdifferenceInexactMatch(a,t,k){

       //alocação de 1 linha de inteiros por n + 1
       this->D = new (nothrow) int[n + 1];

       this->rowPrint = 0;
   }

   ~KdifferenceInexactMatch1Modificado(){
       delete [] D;
    }

    string name() const {
        return "K1md";
    }

   int executar(int m, int j);
};

class KdifferencePrime1: public KdifferencePrime{

    public:
      void instanciar(){
          c = new KdifferenceInexactMatch1Modificado(alpha, beta, &k);  //instancia a classe Original
      }

      void processar(){
        instanciar(); //essa chamada depende diretamente da implementação do kdifferenceInexactMatch que será utilizado
         //O(m), onde m é o tamanho de alpha
         j = 0;
         while(j < (m - k) + 1){
             //guardo em um array todos as ocorrências r de primer para cada j
             cout<<j<<" ";
             c->executar(m - j, j);
             j += jr[j].Jmax;
         }
      }
} prime; // é necessário apenas uma instancia de prime, já declarada aqui

//método que executa o comportamento otimizado do algoritmo
int KdifferenceInexactMatch1Modificado::executar(int m, int j){
   this->m = m;
   this->rowPrint = m;
   int l;

   for(l = 0; l <= n; l++) /** necessário inicializar apenas a primeira linha */
	  D[l] = 0;

   bool passou;     //flag para verificar se toda a linha tem valores >= k
   int Imax = -1;  //flag que marca última linha que tem algum D[i,j] = k-1
   int Jmax = -1;  //maior linha com 0 diferenças = Maior alcance do LCE(0,0..n)
   int pivo, aux, diagonal1, diagonal2;
   char tt;

   for(int i = 1; i <= m && Imax == -1; i++){
     pivo = i; passou = true;
     tt = a[prime.j + i-1];
     diagonal1 = -(k+1);
	  for(l = 1; l <= n; l++){
       aux = menorDeTres(D[l] + 1,
                         pivo + 1,
                         D[l-1] + (tt == t[l-1] ? 0 : 1));
	    D[l-1] = pivo;
	    pivo = aux;
	    if(passou && pivo < k) passou = false;

	    if(pivo == 0){
	       Jmax = i;
          diagonal1 = l-i;
       }
	  }
	  D[n] = aux;
     if(passou){
       Imax = i; //se a linha não foi descartada ela é a primeira
       //busca o maior j
       if(Jmax > 1 && Imax + diagonal1 <= n && D[Imax + diagonal1] == k){
         Jmax = 1;
       }
     }
   }

   prime.jr[j].j    = prime.j;
   prime.jr[j].Jmax = Jmax <= 0 ? 1: Jmax;
   prime.jr[j].r    = Imax;

   return Imax;
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

   cout<<"K-difference-primer-1 processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;

   time_t inicio, fim;

   time(&inicio);
   if(prime.tempo) formataTempo(inicio, "Inicio");
   prime.processar();

   time(&fim);
   if(prime.tempo) formataTempo(fim, "Fim   ");

   prime.gerarOcorrencias();
   prime.mostrarOcorrencias();

   if(prime.tempo){
     double seconds = difftime(fim, inicio);
     formataSegundos(seconds);
     mostrarMemoria();
   }


   return 1;
}
