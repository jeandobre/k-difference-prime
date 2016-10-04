/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2016
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

#include "cst_v_1_1/SSTree.h" //biblioteca arvore sufixo comprimida
#include "cst_v_1_1/Tools.h"

using namespace std;

//declaração da classe que é utilizada no KdifferencePrime
class KdifferenceInexactMatch3CST: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch3CST(char *a, char *t, int *k): KdifferenceInexactMatch234(a,t,k){};
    string name() const {return "K3cst";}
    inline long int LCE(int x, int y);
};


class KdifferencePrime3: public KdifferencePrime{

    public:
      SSTree *sst; //Arvore de Sufixo comprimida (CST)

    public:

      ~KdifferencePrime3(){
         delete c;
         delete sst;
      }
      void instanciar(){
        //implementação da arvore de sufixo comprimida
        c = new KdifferenceInexactMatch3CST(alpha, beta, &k);
      };
} prime;

long int KdifferenceInexactMatch3CST::LCE(int x, int y){
    return prime.sst->lce_lookup(x, y);
};

int main(int argc, char** argv) {

   if (argc < 7 || argc > 14) {
	  cout<<FRED(ERR_ARGS);
	  cout<<USO;
	  return 0;
   }

   Parametro *p = parseParametros(argc, argv);

   if(p->total != 3){
      cout<<FRED(ERR_ARGS);
      cout<<USO;
	   return 0;
   }

   if(p->tipoSaida < 1 || p->tipoSaida > 4){
     cout<<"\n"<<FRED(ERR_TSAIDA)<<"\n";
     return 0;
   }

   prime.setaParametros(p);

   if(prime.k > prime.m){
     cout<<"\n"<<FRED(ERR_KMAIOR)<<prime.m<<"\n";
     return 0;
   }

   if(prime.versao != 1){
     cout<<FRED(MSG_VERSAO_INCORRETA);
     cout<<MSG_VERSAO_K3_VS1;
     return 0;
   }

   time_t inicio, fim;

   cout<<"K-difference-primer-3 executando...\n";
   cout<<"Versao do algoritmo: ";
   !(p->escolheuVersao) ? cout<<FCYN("delfaut") : cout<<prime.versao;
   if(prime.mostrarMatriz) cout<<"\n"<<FMAG(MSG_MATRIZ);
   if(p->mostrarLog) cout<<KYEL<<"\nLog de infomacoes ativado.\n"<<RST;
   cout<<endl;

   //pré-processamento da árvore de sufixo
   if(p->mostrarLog) cout<<"Pre-processando arvore de sufixo...";
    string texto;
    uchar *text = (uchar*) texto.append(prime.alpha).append("#").append(prime.beta).c_str();
    ulong n = strlen((char*)text);
    n++;

    inicio = clock();
    //constroi a árvore de sufixo comprimida para a sequencia generalizada: alpha # beta $
    prime.sst = new SSTree(text, n);

    fim = clock();

   if(p->mostrarLog) cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")\n";

   time(&inicio);
   if(prime.tempo) formataTempo(inicio, true);
   prime.processar();
   time(&fim);
   if(prime.tempo) formataTempo(fim, false);

   if(!prime.mostrarMatriz) prime.mostrarOcorrencias(p);

   if(prime.tempo){
     double seconds = difftime(fim, inicio);
     formataSegundos(seconds);
     mostrarMemoria();
   }

   return 1;
}
