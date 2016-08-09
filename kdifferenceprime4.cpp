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
#include "classes.h"

//bibliotecas array sufixo
#include "sa.h"  //KdifferenceInexactMatch4SA
#include "rmq-sa.h"
#include "cst_v_1_1/CSA.h" //KdifferenceInexactMatch4CSA
#include "cst_v_1_1/CHgtArray.h"

using namespace std;

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch4;

//compute LCE com cst1.1 CSA.h e CHgtArray.h
const int directMin(CHgtArray *lcp, int i, int j){
   int low =  lcp->csa->inverse(i);
   int high = lcp->csa->inverse(j);

   i = minVal(low, high);
   j = maxVal(low, high);

   //low + 1 na vers�o original, mas aki o �ndice 1 � representado por 0
   int t = lcp->GetPos(i); //por causa do �ndice inicial
   int s;

   for(int k = i + 1; k < j; k++){
      s = lcp->GetPos(k); //mesma coisa aki, �ndice come�a em zero
      if(s < t) t = s;
   }
   return t;
}

//implementa��o utilizando a abordagem 2
class KdifferenceInexactMatch4CSA: public KdifferenceInexactMatch2{
  public:
    KdifferenceInexactMatch4CSA(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    int executar(int m);
};

class KdifferenceInexactMatch4SA: public KdifferenceInexactMatch2{
  public:
    KdifferenceInexactMatch4SA(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    int executar(int m);
};

class KdifferenceInexactMatch4RMQ: public KdifferenceInexactMatch2{
  public:
    KdifferenceInexactMatch4RMQ(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    int executar(int m);
};

class KdifferencePrime4: public KdifferencePrime{

    public:
      vector<int> invSuff;
      vector<int> LCP;
      int nTotal;

      CSA *sa; //declara��o do array de sufixo
      CHgtArray *lcp; //declara��o do LCP

      SuffixArray *rmqSA;

      ~KdifferencePrime(){
        delete c;
        delete sa;
        delete lcp;
        delete rmqSA;
      }

      void instanciar(){
        if(versao == 1) //vers�o da biblioteca de �rvores de sufixo
          c = new KdifferenceInexactMatch4CSA(alpha, beta, &k);
        else if(versao == 2) //vers�o encontrada na internet de arrays de sufixo e lcp
          c = new KdifferenceInexactMatch4SA(alpha, beta, &k);
        else //vers�o encontrada na internet de sa/lcp e RMQ em tempo O(1)
          c = new KdifferenceInexactMatch4RMQ(alpha, beta, &k);
      };

} prime;

int KdifferenceInexactMatch4CSA::executar(int m){
    this->m = m;
    int d,e, row;

    //inicializa��o da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1);

    int long long pivo; //vari�vel auxiliar para troca de posi��es
    bool passou = true; //flag para controlar o caso de alcan�ar o fim de m antes de k diferen�as
    int linha = -1;         //vari�vel qwue guarda a primeira linha de ocorr�ncia de primer
    for(e = 0; e < k; e++){
        pivo = -1;
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1), getL(d) + 1, getL(d+1) + 1);
           row = menorDeDois(row, m);
           //LCE
           if(row < m && row + d < n)
             row += directMin(prime.lcp, prime.j + row, prime.m + 1 + row + d);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n+1) row = -1;
           if(d == n) setL(d, row); //se for o �ltimo guarda o atual
           else pivo = row;

           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m) passou = false;
           if(passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;

    return linha;
}

int KdifferenceInexactMatch4SA::executar(int m){
    this->m = m;
    int d,e, row;

    //inicializa��o da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1);

    int long long pivo; //vari�vel auxiliar para troca de posi��es
    bool passou = true; //flag para controlar o caso de alcan�ar o fim de m antes de k diferen�as
    int linha = -1;         //vari�vel qwue guarda a primeira linha de ocorr�ncia de primer
    for(e = 0; e < k; e++){
        pivo = -1;
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1), getL(d) + 1, getL(d+1) + 1);
           row = menorDeDois(row, m);
           //LCE
           if(row < m && row + d < n)
             row += LCE(prime.LCP, prime.invSuff, prime.nTotal, prime.j + row, prime.m + 1 + row + d);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n+1) row = -1;
           if(d == n) setL(d, row); //se for o �ltimo guarda o atual
           else pivo = row;

           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m) passou = false;
           if(passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;

    return linha;
}

int KdifferenceInexactMatch4RMQ::executar(int m){
    this->m = m;
    int d,e, row;

    //inicializa��o da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1);

    int long long pivo; //vari�vel auxiliar para troca de posi��es
    bool passou = true; //flag para controlar o caso de alcan�ar o fim de m antes de k diferen�as
    int linha = -1;         //vari�vel qwue guarda a primeira linha de ocorr�ncia de primer
    for(e = 0; e < k; e++){
        pivo = -1;
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1), getL(d) + 1, getL(d+1) + 1);
           row = menorDeDois(row, m);
           //LCE
           if(row < m && row + d < n)
             row += prime.rmqSA->LCE(prime.j + row, prime.m + 1 + row + d);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n+1) row = -1;
           if(d == n) setL(d, row); //se for o �ltimo guarda o atual
           else pivo = row;

           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m) passou = false;
           if(passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;

    return linha;
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
     cout<<MSG_VERSAO_K4_VS1<<MSG_VERSAO_K4_VS2<<MSG_VERSAO_K4_VS3;
     return 0;
   }

   time_t inicio, fim;

    //pr�-processamento do array de sufixo
    cout<<"Pre-processando array de sufixo...";

    inicio = clock();
    string texto = prime.alpha;
    texto.append("#").append(prime.beta);
    prime.nTotal = texto.length();

    if(prime.versao == 1){
       uchar *text = (uchar*) texto.c_str();
       int n = prime.nTotal + 1;
       unsigned floorLog2n = Tools::FloorLog2(n);
       if (floorLog2n < 4) floorLog2n = 4;
       unsigned rmqSampleRate = floorLog2n / 2;

       prime.sa  = new CSA(text, n, rmqSampleRate);
       prime.lcp = new CHgtArray(prime.sa, text, n);
    } else if(prime.versao == 2){
        vector<int> suffixArr = buildSuffixArray(texto, prime.nTotal);
        vector<int> invSuff(prime.nTotal, 0);
        prime.invSuff = invSuff;
        prime.LCP = kasai(texto, suffixArr, prime.invSuff);
    }else{
         char *cstr = new char[texto.length() + 1];
         strcpy(cstr, texto.c_str());
         prime.rmqSA = new SuffixArray(cstr);
         prime.rmqSA->builds();
    }

   fim = clock();

   cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")"<<endl;
   cout<<"K-difference-primer-4 executando..."<<endl;
   cout<<"Versao:"<<prime.versao<<endl;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(prime.tempo) formataTempo(tempo_execucao);

   return 0;
}
