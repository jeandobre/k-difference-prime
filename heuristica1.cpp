/* *******************************************************************
 *  Autor: Jean Alexandre Dobre                                      *
 *  Orientador: Said Sadique Adi                                     *
 *  Ano: 2015/2016                                                   *
 *  FACOM: Mestrado em Ciência da Computação                         *
 *                                                                   *
 * Este projeto implementa a versão otimizada do k-difference inexact*
 * match por meio da matriz D com apenas uma linha.                  *
 *                                                                   *
 * Foi adicionado ao algoritmo k difference primer uma verificação a *
 * um vetor V que contém o maior LCE por linha de alfa.              *
 *                                                                   *
 * *******************************************************************/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "classes.h"

//LCE entre s[i] e t[j]
static int directCompLCE(char *a, char *b, int _i, int _j, int m, int n){
   int o = 0;
   while(a[o + _i] == b[o + _j] && o+_j < n && o+_i < m) o++;

   return o;
}

//implementação Matriz D[0, 0..n]
class KdifferenceInexactMatch1heuristica1: public KdifferenceInexactMatch{
  protected:
    int *D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]

  public:
    KdifferenceInexactMatch1heuristica1(char *a, char *t, int *k):KdifferenceInexactMatch(a,t,k){
       //alocação de 1 linha de inteiros por n + 1
       this->D = new int [n + 1];
   }

   ~KdifferenceInexactMatch1heuristica1(){
       delete [] D;
    }

    string name() const {
        return "hca1";
    }

   int executar(int m, int j);

};

class KdifferencePrime1: public KdifferencePrime{
   protected:
     int rr;

   public:
     // retorno *Mlce;
      int *V;
      void instanciar(){
         int mpl, vr, dgn;
         c = new KdifferenceInexactMatch1heuristica1(alpha, beta, &k);  //instancia a classe Original

         //this->mPorLinha = new maiorLCEporLinha [m];
         V = new int [m];
         //this->diagonal = new int [k + m + 1];
         //encontrar o maior LCE por linha de alpha e guardar em lce
         int l, dlg;
         for(int i = 0; i != m; i++){
            mpl = 0;
            for(l = 0; l != n; l++){
               vr = directCompLCE(alpha, beta, i, l, m, n);
               if(vr > mpl){
                 mpl = vr;
                 dlg = l;
               }
            }
            //mPorLinha[i].lce = mpl;
            //mPorLinha[i].diagonal = dlg;
            V[i] = dlg; //guarda a coluna do maior LCE
            //cout<<i<<", lce: "<<mpl<<", coluna: "<<dlg<<"\n";
         }
      }

      void processar(){
         //diff -y --suppress-common-lines file1 file2 | grep '^' | wc -l

         j = 0;
         rr = 0;
         int r,l;

         while(j < (m - k) + 1){
          c->primerJ = j;
          r = c->executar(m - j, j);
          rr++;
          ocr[j] = r;
          if(r == -1) break; //quando nao encontrou mais ocorrencias deve parar

          //aqui é efetuado a verificação se é possível pular as próximas posições de j
          //utilizando o mesmo resultado computado e guardado em r.
          int v = 1;
          int jOriginal = j;
          while(V[jOriginal+v] == V[jOriginal] + v){
            ocr[++j] = (r - v++) * (-1);
           // cout<<"j:"<<j<<"ocr:"<<ocr[j];

          }
          j++;

         }
         int rp = m-k+1;
         int vl = (rr * 100)/rp;
         cout<<"Jotas processados: "<<rr<<" de "<<rp<<" possiveis. Ganho de "<<100 - vl<<"%."<<endl;
     }
} prime; // é necessário apenas uma instancia de prime, já declarada aqui


//método que executa o comportamento original do algoritmo
int KdifferenceInexactMatch1heuristica1::executar(int m, int j){
   this->m = m;

   for(int l = 0; l <= n; l++) /** necessário inicializar apenas a primeira linha */
	  D[l] = 0;

   bool passou;     //flag para verificar se toda a linha tem valores >= k
   int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
   int pivo, aux, jMax = -1;
   char tt;
   int coluna = -1;

   for(int i = 1; i <= m && linha == -1; i++){
     pivo = i; passou = true;
     tt = a[prime.j + i-1];
	  for(int l = 1; l <= n; l++){
       aux = menorDeTres(D[l] + 1,
                         pivo + 1,
                         D[l-1] + (tt == t[l-1] ? 0 : 1));
	    D[l-1] = pivo;
	    pivo = aux;
	    if(passou && pivo < k) passou = false;
	  }
	  D[n] = aux;
     if(passou) linha = i; //se a linha não foi descartada ela é a primeira
   }
	//cout<<"Maior linha lce: "<< jMax;
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
     cout<<MSG_VERSAO_K1_VS1<<MSG_VERSAO_K1_VS2<<MSG_VERSAO_K1_VS3;
     return 0;
   }

   cout<<"K-difference-primer-1 processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;
   prime.instanciar();

   time_t inicio, fim;

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
