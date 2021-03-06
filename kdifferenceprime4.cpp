/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015-2016
   FACOM: Mestrado em Ci�ncia da Computa��o

   Este projeto implementa a vers�o modificada do k-difference inexact match por meio da
   matriz L [d x e], algoritmo h�brido, proposto por Landau Gad M. e Vishkin Uzi no artigo
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
#include <list>
#include "classes.h"

//biblioteca array sufixo
#include "rmq-sa.h"


using namespace std;

class KdifferenceInexactMatch4SA: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch4SA(char *a, char *t, int *k): KdifferenceInexactMatch234(a,t,k){};
    string name() const {return "K4dmin";};
    void LCE(int x, int y, int &aux);
};

class KdifferenceInexactMatch4RMQ: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch4RMQ(char *a, char *t, int *k): KdifferenceInexactMatch234(a,t,k){};
    string name() const {return "K4rmq";};
    void LCE(int x, int y, int &aux);
};

class KdifferencePrime4: public KdifferencePrime{

    public:
      SuffixArray *sa;

      ~KdifferencePrime4(){
        delete sa;
      }

      void instanciar(){
        if(versao == 2) //arrays de sufixo e lcp com directMin
          c = new KdifferenceInexactMatch4SA(alpha, beta, &k);
        else //sa e lcp com RMQ em tempo O(1)
          c = new KdifferenceInexactMatch4RMQ(alpha, beta, &k);
      };

} prime;

void KdifferenceInexactMatch4SA::LCE(int x, int y, int &aux){
    aux += prime.sa->LCEdirectMin(x, y);
};

void KdifferenceInexactMatch4RMQ::LCE(int x, int y, int &aux){
    aux += prime.sa->LCEviaRMQ(x, y);
};

int main(int argc, char** argv) {

   if (argc < 7 || argc > 14) {
	  cout<<FRED(ERR_ARGS);
	  cout<<USO;
	  return 0;
   }

   Parametro *p = parseParametros(argc, argv);
   if(p->argumentoErrado) return 0;

   if(p->total != 3){
      cout<<FRED(ERR_ARGS);
      cout<<USO;
	   return 0;
   }

   if(p->tipoSaida < 1 || p->tipoSaida > 5){
     cout<<"\n"<<FRED(ERR_TSAIDA)<<"\n";
     return 0;
   }

   prime.setaParametros(p);

   if(prime.k > prime.m){
     cout<<"\n"<<FRED(ERR_KMAIOR)<<prime.m<<"\n";
     return 0;
   }

   if(prime.versao > 2 || prime.versao < 1){
     cout<<FRED(MSG_VERSAO_INCORRETA);
     cout<<MSG_VERSAO_K4_VS1<<MSG_VERSAO_K4_VS2;
     return 0;
   }

   cout<<"K-difference-primer-4 executando...\n";
   cout<<"Versao do algoritmo: ";
   !(p->escolheuVersao) ? cout<<FCYN("delfaut") : cout<<prime.versao;
   if(prime.mostrarMatriz) cout<<"\n"<<FMAG(MSG_MATRIZ);
   if(p->mostrarLog) cout<<KYEL<<"\nLog de infomacoes ativado.\n"<<RST;
   cout<<endl;

   time_t inicio, fim;

   //pr�-processamento do array de sufixo
   if(p->mostrarLog) cout<<"Pre-processando array de sufixo...";
   inicio = clock();

   string texto = prime.alpha;
   texto.append("#").append(prime.beta);

   char *cstr = new char[texto.length() + 1];
   strcpy(cstr, texto.c_str());
   texto.clear();

   prime.sa = new SuffixArray(cstr);
   prime.sa->builds();

   fim = clock();

   if(p->mostrarLog) cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")\n";

   time(&inicio);
   if(prime.tempo) formataTempo(inicio, true);
   prime.processar(p->Jselecionado, p->Jdistancia);
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
