/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015-2016
   FACOM: Mestrado em Ciência da Computação

   Este projeto implementa a versão modificada do k-difference inexact match por meio da
   matriz L [d x e], algoritmo híbrido, proposto por Landau Gad M. e Vishkin Uzi no artigo
   FAST PARALLEL AND SERIAL APPROXIMATE STRING MATCHING
   página 5.

   A versão do k-difference primer é a abordagem do livro ALGORITHMS ON STRINGS, TREES, AND SEQUENCES de Gusfiel
   que avalia a coluna k-1 procurando se 'm' foi alcançado, se não foi alcançado existe um prime com pelo menos k diferenças
   e ele está no maior valor da coluna k-1.
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <list>
#include "classes.h"

//biblioteca array sufixo
#include "rmq-sa.h"


using namespace std;

class KdifferenceInexactMatch4SA: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch4SA(char *a, char *t, int *k): KdifferenceInexactMatch234(a,t,k){};
    string name() const {return "K4dmin";};
    inline long int LCE(int x, int y);
};

class KdifferenceInexactMatch4RMQ: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch4RMQ(char *a, char *t, int *k): KdifferenceInexactMatch234(a,t,k){};
    string name() const {return "K4rmq";};
    inline long int LCE(int x, int y);
};

class KdifferencePrime4: public KdifferencePrime{

    public:
      SuffixArray *sa;

      ~KdifferencePrime4(){
        delete sa;
      }

      void instanciar(){
        if(versao == 1) //arrays de sufixo e lcp com directMin
          c = new KdifferenceInexactMatch4SA(alpha, beta, &k);
        else //sa e lcp com RMQ em tempo O(1)
          c = new KdifferenceInexactMatch4RMQ(alpha, beta, &k);
      };

} prime;

long int KdifferenceInexactMatch4SA::LCE(int x, int y){
    return prime.sa->LCEdirectMin(x, y);
};

long int KdifferenceInexactMatch4RMQ::LCE(int x, int y){
    return prime.sa->LCEviaRMQ(x, y);
};

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
     cout<<MSG_VERSAO_K4_VS1<<MSG_VERSAO_K4_VS2<<MSG_VERSAO_K4_VS3;
     return 0;
   }

   time_t inicio, fim;

   //pré-processamento do array de sufixo
   cout<<"Pre-processando array de sufixo...";

   inicio = clock();
   string texto = prime.alpha;
   texto.append("#").append(prime.beta);

   char *cstr = new char[texto.length() + 1];
   strcpy(cstr, texto.c_str());

   prime.sa = new SuffixArray(cstr);
   prime.sa->builds();

   fim = clock();

   cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")"<<endl;
   cout<<"K-difference-primer-4 executando..."<<endl;
   cout<<"Versao:"<<prime.versao<<endl;

   time(&inicio);
   if(prime.tempo) formataTempo(inicio, true);
   prime.processar();
   time(&fim);
   if(prime.tempo) formataTempo(fim, false);

   if(!prime.mostrarMatriz) prime.mostrarOcorrencias();

   if(prime.tempo){
     double seconds = difftime(fim, inicio);
     formataSegundos(seconds);
   }

   return 0;
}
