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
#include "auxiliar.h"
//#include "sa.h" //biblioteca array sufixo
#include "rmq-sa.h"
using namespace std;

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch4;

class KdifferencePrime4: public KdifferencePrime{

    public:
      KdifferenceInexactMatch4 *c;
      vector<int> invSuff;
      vector<int> lcp;
      int nTotal;

      void processar();

} prime;

//O algoritmo foi adaptado para entregar o resultado do primer que � o inverso da programa��o original
//Classe que resolve o problema k-mismatches problems em tempo O(nk)
class KdifferenceInexactMatch4{

  private:
    int *L;   //Matriz L[-(k+1)..d]
    string a;  //padr�o
    string t;  //texto

    int k;     //quantidade de difer�n�as
    int m, n;  //tamanho de a e t respectivamente
  public:
    int linha; //linha que cont�m o menor valor >= k

  private:
      int dToMatriz(int d){
      return d + k + 1; //converte �ndice d para �ndices reais
    }

  public:
    void setL(int d, int valor){
      L[dToMatriz(d)] = valor; //L[d,e] = valor
    }

    int getL(int d){
      return L[dToMatriz(d)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //k, n+1, 1 da posi��o 0
    }

  public:
    KdifferenceInexactMatch4(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);

       //aloca��o din�mica de inteiros
       this->L = new int[getDimensaoD()];
    }

    ~KdifferenceInexactMatch4(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       delete [] L;
    }
    void executar();
    void imprimirMatrizTela();

    void setaTamanhoM(){
      this->m = prime.m - prime.j;
    }
};

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch4::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -(k+1); d <= (n+1); d++){
        vr = getL(d);
        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}

//m�todo que executa o comportamento original do algoritmo
void KdifferenceInexactMatch4::executar(){
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
                             getL(d) + 1,
                             getL(d+1) + 1);

           row = menorDeDois(row, m);
           //LCE
           //if(row < m && row + d < n)
           //  row += LCE(prime.lcp, prime.invSuff, prime.nTotal, prime.j + row, prime.m + 1 + row + d);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n) row = -1;
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

//m�todo que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: n�o h� execu��o sem a invoca��o deste m�todo
void KdifferencePrime4::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorr�ncias

   c = new KdifferenceInexactMatch4(alpha, beta, &k); //instancia apenas uma vez

   for(j = 0; j < (m - k) + 1; j++){
       c->setaTamanhoM();
       c->executar();

       if(c->linha > -1){ //se a linha foi obtida, significa que existe uma ocorr�ncia
           Primer *pr = new Primer(++ocr, j, c->linha, ocorrencia.substr(j, c->linha));
           primers.insert(primers.end(), pr);
           if(prime.mostrarMatriz) c->imprimirMatrizTela();
       }
   }
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

   prime.alpha = new char [p->alpha.length() + 1];
   strcpy(prime.alpha, p->alpha.c_str());
   prime.beta = new char [p->beta.length() + 1];
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
     cout<<endl<<ERR_KMAIOR<<endl;
     return 0;
   }

   time_t inicio, fim;

    //pr�-processamento do array de sufixo
    cout<<"Pre-processando array de sufixo...";

    inicio = clock();
    string texto = prime.alpha;
    texto.append("#").append(prime.beta);
    prime.nTotal = texto.length();

//    vector<int> suffixArr = buildSuffixArray(texto, prime.nTotal);
//    vector<int> invSuff(prime.nTotal, 0);
//    prime.invSuff = invSuff;
//    prime.lcp = kasai(texto, suffixArr, prime.invSuff);

   SuffixArray *sa = new SuffixArray((char *) texto.c_str());
   sa->builds();

   cout<<sa->LCE(0,9)<<endl;
   cout<<sa->LCE(1,9)<<endl;
   cout<<sa->LCE(3,13)<<endl;
   cout<<sa->LCE(4,14)<<endl;

    fim = clock();

   cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")"<<endl;
   cout<<"K-difference-primer-4 executando..."<<endl;
   cout<<"Versao: SA"<<endl;

   inicio = clock();
   //prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(tempo) formataTempo(tempo_execucao);

   return 0;
}
