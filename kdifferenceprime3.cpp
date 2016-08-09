/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2016
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
#include "auxiliar.h"
#include "classes.h"

#include "cst_v_1_1/SSTree.h" //biblioteca arvore sufixo compressada
#include "cst_v_1_1/Tools.h"

using namespace std;

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch3CST: public KdifferenceInexactMatch2{
  public:
    KdifferenceInexactMatch3CST(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    int executar(int m);
};


class KdifferencePrime{

    public:
      SSTree *sst; //Arvore de Sufixo comprimida (CST)

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usu�rio n�o escolher nada
         this->mostrarMatriz=false;
      }
      ~KdifferencePrime(){
         delete c;
         delete sst;
      }
      void instanciar(){
        //implementa��o da arvore de sufixo compressada
        c = new KdifferenceInexactMatch3CST(alpha, beta, &k);
      };
} prime;

//m�todo que executa o comportamento original do algoritmo
int KdifferenceInexactMatch3CST::executar(int m){
    this->m = m;
    int d,e, row;

    //inicializa��o da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1);

    int long long pivo; //vari�vel auxiliar para troca de posi��es
    bool passou = true; //flag para controlar o caso de alcan�ar o fim de m antes de k diferen�as
    linha = -1;         //vari�vel qwue guarda a primeira linha de ocorr�ncia de primer
    for(e = 0; e < k; e++){
        pivo = -1;
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1),
                             getL(d)   + 1,
                             getL(d+1) + 1);

           row = menorDeDois(row, m);
           //LCE (Segundo os autores o tempo � determinado por um valor K que eu n�o encontrei
           //para diminuir o tempo
           if(row < m && row + d < n)
             row += prime.sst->lce(prime.j + row , prime.m + 1 + row + d);//LCE(0,m+1);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n+1) row = -1;
           if(d == n) setL(d, row); //se for o �ltimo guarda o atual
           else pivo = row;

           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m) passou = false;
           if (passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;
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
     cout<<endl<<ERR_KMAIOR<<endl;
     return 0;
   }

   if(prime.versao != 1){
     cout<<MSG_VERSAO_INCORRETA;
     cout<<MSG_VERSAO_K3_VS1;
     return 0;
   }

   time_t inicio, fim;

    //pr�-processamento da �rvore de sufixo
    cout<<"Pre-processando arvore de sufixo...";
    string texto;
    uchar *text = (uchar*) texto.append(prime.alpha).append("#").append(prime.beta).c_str();
    ulong n = strlen((char*)text);
    n++;

    inicio = clock();
    //constroi a �rvore de sufixo comprimida para a sequencia generalizada: alpha # beta $
    prime.sst = new SSTree(text, n);

    fim = clock();

   cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")"<<endl;
   cout<<"K-difference-primer-3 executando..."<<endl;
   cout<<"Versao:"<<prime.versao<<endl;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(tempo) formataTempo(tempo_execucao);

   return 0;
}
