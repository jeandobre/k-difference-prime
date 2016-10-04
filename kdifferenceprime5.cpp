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

//biblioteca arvore sufixo
#include "strmat/strmat.h"
#include "strmat/stree_ukkonen.h"
#include "strmat/strmat_stubs2.h"
#include "strmat/strmat_print.h"
#include "strmat/peter_longest_common_extension.h"
#include "strmat/strmat_util.h"
#include "strmat/stree_lca.h"

using namespace std;

//declaração da classe que é utilizada no KdifferencePrime
class KdifferenceInexactMatch5ST: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch5ST(char *a, char *t, int *k): KdifferenceInexactMatch234(a,t,k){};
    string name() const {return "K5st";}
    inline long int LCE(int x, int y);
};


class KdifferencePrime5: public KdifferencePrime{

    public:
     LCE *lce_ctx;//  = prepare_longest_common_extension(strings[0], strings[1], false);

    public:

      ~KdifferencePrime5(){
         delete c;
         longest_common_extension_free(lce_ctx);
      }
      void instanciar(){
        //implementação da arvore de sufixo comprimida
        c = new KdifferenceInexactMatch5ST(alpha, beta, &k);
      };
} prime;

long int KdifferenceInexactMatch5ST::LCE(int x, int y){
   return lookup(prime.lce_ctx, x, y);
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

   cout<<"K-difference-primer-5 executando...\n";
   cout<<"Versao do algoritmo: ";
   !(p->escolheuVersao) ? cout<<FCYN("delfaut") : cout<<prime.versao;
   if(prime.mostrarMatriz) cout<<"\n"<<FMAG(MSG_MATRIZ);
   if(p->mostrarLog) cout<<KYEL<<"\nLog de infomacoes ativado.\n"<<RST;
   cout<<endl;

   time_t inicio, fim;

    //pré-processamento da árvore de sufixo
   if(p->mostrarLog) cout<<"Pre-processando arvore de sufixo...";

    inicio = clock();
    bool log = false;
    const STRING *str1, *str2;

    str1 = new STRING;
    int tamanhoM = prime.m+1;
    CHAR_TYPE *sq1 = new CHAR_TYPE[tamanhoM];
    str1 = make_seqn("alpha", str_to_sequence(prime.alpha, sq1, tamanhoM), tamanhoM, p->mostrarLog);

    str2 = new STRING;
    int tamanhoN = prime.n+2;
    CHAR_TYPE *sq2 = new CHAR_TYPE[tamanhoN];
    char *beta = new char[tamanhoN];
    beta = prime.beta;
    beta[tamanhoN-2] = '$';
    beta[tamanhoN-1] = '\0';
    str2 = make_seqn("beta", str_to_sequence(beta, sq2, tamanhoN), tamanhoN, p->mostrarLog);

    prime.lce_ctx = prepare_longest_common_extension(str1, str2, p->mostrarLog);
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
