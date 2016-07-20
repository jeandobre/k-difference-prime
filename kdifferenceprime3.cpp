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

#include "cst11/SSTree.h" //biblioteca arvore sufixo compressada
#include "cst11/Tools.h"

using namespace std;

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch3;

class KdifferencePrime{

    public:
      string alpha, beta;//seguindo o modelo proposto por GUSFIELD, nomes de vari�veis que representam o padr�o e o texto respectivamente
      int k;             //quantidade de difer�n�as
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      bool mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int versao;     //booleano 0, 1 usado para executar a vers�o otimizada do algoritmo

      KdifferenceInexactMatch3 *c;
      int j;             //�ndice de alpha necess�rio p/ posicionar corretamente na �rvore de sufixo
      SSTree *sst;

      list<Primer *> primers;

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usu�rio n�o escolher nada
         this->mostrarMatriz=false;
      }
      ~KdifferencePrime(){
         delete c;
         delete sst;
      }
      void processar();
} prime;

//O algoritmo foi adaptado para entregar o resultado do primer que � o inverso da programa��o original
//Classe que resolve o problema k-mismatches problems em tempo O(nk)
class KdifferenceInexactMatch3{

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
      return d + k; //converte �ndice d para �ndices reais
    }

  public:
    void setL(int d, int valor){
      L[dToMatriz(d)] = valor; //L[d,e] = valor
    }

    int getL(int d){
      return L[dToMatriz(d)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 1; //k, n+1, 1 da posi��o 0
    }

  public:
    KdifferenceInexactMatch3(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();

       //aloca��o din�mica de inteiros
       this->L = new int[getDimensaoD()];
    }

    ~KdifferenceInexactMatch3(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       delete [] L;
    }
    void executar();
    void imprimirMatrizTela();

    void setA(string a){
      this->a = a;
      this->m = a.size(); //atualiza o valor de 'm' com o novo tamanho de 'a'
    }
};

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch3::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -k; d <= n; d++){
        vr = getL(d);
        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}

//m�todo que executa o comportamento original do algoritmo
void KdifferenceInexactMatch3::executar(){
    int d,e, row;

    //inicializa��o da coluna e = 0
    bool passou = true;
    linha = -1;
    for(d = -k; d <= n && passou; d++){
        if(d < 0 || d == n) setL(d, -1);
        else {
            row = prime.sst->lce(prime.j, prime.m + 1 + d);
            setL(d, row);

            if(row == m) passou = false;
            if (passou && row > linha) linha = row;
        }
    }

    int long long pivo;

    for(e = 1; e < k; e++){
        pivo = -1;
        for(d = -e; d < n && passou; d++){
           row = maiorDeTres(getL(d-1),
                             getL(d) + 1,
                             getL(d+1) + 1);

           row = menorDeDois(row, m);
           //LCE
           if(row < m && row + d < n)
             row += prime.sst->lce(prime.j + row , prime.m + 1 + row + d);//LCE(0,m+1);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n) row = -1;
           if(d == n-1)setL(d, row); //se for o �ltimo guarda o atual
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
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorr�ncias

   c = new KdifferenceInexactMatch3(alpha, beta, k); //instancia apenas uma vez

   for(int j = 0; j < (m - k) + 1; j++){
       //cout<<j<<",";
       c->setA(alpha.substr(j));
       this->j = j;
       c->executar();

       if(c->linha > -1){ //se a linha foi obtida, significa que existe uma ocorr�ncia
           Primer *pr = new Primer(++ocr, j, c->linha, alpha.substr(j, c->linha));
           primers.insert(primers.end(), pr);
       }
   }
}

int main(int argc, char** argv) {

   if (argc < 7 || argc > 9) {
	  cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   Parametro *p = parseParametros(argc, argv);
   if(p->total != 3){
      cout<<endl<<ERR_ARGS<<endl;
	  return 0;
   }

   prime.alpha = p->alpha;
   prime.beta = p->beta;
   prime.k = p->k;
   prime.mostrarMatriz = p->mostrarMatriz;
   prime.versao = p->versao;

   prime.m = prime.alpha.size();
   prime.n = prime.beta.size();

   if(prime.k > prime.m){
     cout<<endl<<ERR_KMAIOR<<endl;
     return 0;
   }

   //pr�-processamento da �rvore de sufixo
   cout<<"K-difference-primer-3 pre-processando arvore de sufixo...";
   string texto;
   uchar *text = (uchar*) texto.append(prime.alpha).append("#").append(prime.beta).c_str();
   ulong n = strlen((char*)text);
   n++;

   time_t inicio, fim;

   inicio = clock();
   prime.sst = new SSTree(text, n);
   cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")"<<endl;
   fim = clock();

   cout<<"K-difference-primer-3 executando..."<<endl;
   cout<<"Versao: arvore de sufixo generalizada compressada"<<endl;



   inicio = clock();
   prime.processar();
   fim = clock();

   if(prime.primers.size() == 0) cout<<MSG_0_OCCR<<prime.k<<" diferenca(s)"<<endl;
   else{
        cout<<"Encontrado "<<prime.primers.size()<<" ocorrencia(s) r de primers"<<endl;
        for(Primer *p : prime.primers){
            p->escreverTela();
        }
   }

   if(p->mostrarTempo)
     cout<<"Tempo de execucao: "<< ((fim - inicio) / (CLOCKS_PER_SEC / 1000)) << " milisegundos"<<endl;

   return 0;
}
