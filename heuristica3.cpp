/* *******************************************************************
 *  Autor: Jean Alexandre Dobre                                      *
 *  Orientador: Said Sadique Adi                                     *
 *  Ano: 2015/2016                                                   *
 *  FACOM: Mestrado em Ciência da Computação                         *
 *                                                                   *
 * Este projeto implementa a versão original do k-difference inexact *
 * match por meio da matriz D [m x n] com //TODO                     *
 *                                                                   *
 *                                                                   *
 *                                                                   *
 * *******************************************************************/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "classes.h"

/*
struct retorno{
    int Jmax;
    int Imax;
    int diagonal;
    int lce;
};
*/
/*
struct maiorLCEporLinha{
   int lce;
   int diagonal;
};
*/

//LCE entre s[i] e t[j]
static int directCompLCE(char *a, char *b, int _i, int _j, int m, int n){
   int o = 0;
   while(a[o + _i] == b[o + _j] && o+_j < n && o+_i < m) o++;

   return o;
}

class KdifferenceInexactMatch1heuristica1: public KdifferenceInexactMatch{
  public:
    int **D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]

  public:
    KdifferenceInexactMatch1heuristica1(char *a, char *t, int *k):KdifferenceInexactMatch(a,t,k){
       //alocação de 1 linha de inteiros por n + 1
       //alocação dinâmica de inteiros, necessário mais um que é a posição 0
       this->D = new (nothrow) int*[m + 1];
       for (int j = 0; j <= m; ++j)
          this->D[j] = new (nothrow) int[n + 1];
   }

   ~KdifferenceInexactMatch1heuristica1(){
       for (int i = 0; i <= m; ++i)
         delete [] D[i];
       delete [] D;
    }

    string name() const {
        return "hca1";
    }

   void imprimirMatrizTela();
   int executar(int m, int j);
   int getMinLeft(int r, int j){
     //percorre da lnha r até o inicio
     //menor linha iMax
     //cout<<"r"<<r<<"k:"<<k<<endl;
     int iMax = r;
     int cc, ii;
     int coluna = r - k;
     int iniCol;
     for(int l = coluna; l <= n; l++){
        int c = l;
        //int b = c;
        int i = r;
        int kk = 0;
        while(kk < k){

           //cout<<"\n i:"<<i<<",c:"<<c;
           //cout<<"=>"<<a[j+i-1]<<" "<<t[c-1];
           //cout<<"(val="<<D[i][c]<<",1="<<D[i-1][c-1]<<",2="<<D[i-1][c]<<",3="<<D[i][c-1]<<")";

           if(a[j+i-1] == t[c-1]){
              --i;
              --c;
           } else {
              int val = D[i][c]-1;
              ++kk;
              if(kk == k){
                //cout<<"\n";
                continue;
              }
              if(D[i-1][c-1] == val){
                 --i;
                 --c;
              } else {
                 if(D[i-1][c] == val) --i;
                 else if(D[i][c-1] == val) --c;
              }
           }
          //cout<<"[i:"<<i<<",c:"<<c<<",k:"<<kk<<"]";
        }
        if(i < iMax){
          iMax = i;
          //iniCol = b;
          //cc = c;
          //ii = i;
        }
     }
     //if(j == 3)
       //cout<<"Maior linha iMax: "<<iMax<<", col inicio: "<<iniCol<<", i: "<<ii<<", c: "<<cc<<"\n";
     return iMax;
   }

};

class KdifferencePrime1: public KdifferencePrime{
   protected:
     int rr;

   public:
     // retorno *Mlce;
     // maiorLCEporLinha *mPorLinha;
      int *V;
      void instanciar(){
         int mpl, vr, dgn;
         c = new KdifferenceInexactMatch1heuristica1(alpha, beta, &k);  //instancia a classe Original

         //this->mPorLinha = new maiorLCEporLinha [m];
       /*  V = new int [m];
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
           // cout<<i<<", lce: "<<mpl<<", coluna: "<<dlg<<"\n";
         } */
      }

      void processar(){
         instanciar(); //essa chamada depende diretamente da implementação do kdifferenceInexactMatch que será utilizado

         //diff -y --suppress-common-lines file1 file2 | grep '^' | wc -l

         j = 0;
         rr = 0;
         int r,l;

         while(j < (m - k) + 1){
         //while(j < 3){
          c->primerJ = j;
          r = c->executar(m - j, j);
          rr++;
          ocr[j] = r;
          if(r == -1) break; //quando nao encontrou mais ocorrencias deve parar
          l = c->getMinLeft(r, j);
          if(l > 1){
             for(int cr = 1; cr < l; cr++)
               ocr[++j] = r - cr;
          }

         /* int v = 1;
          while(V[j+v] == V[j] + v)
            ocr[++j] = r - v++; */

          j++;

         }
         int rp = m-k+1;
         int vl = (rr * 100)/rp;
         cout<<"Jotas processados: "<<rr<<" de "<<rp<<" possiveis. Ganho de "<<100 - vl<<"%."<<endl;
     }
} prime; // é necessário apenas uma instancia de prime, já declarada aqui


//método que imprimi a matriz na tela
void KdifferenceInexactMatch1heuristica1::imprimirMatrizTela(){

   cout << setfill(' ');          //setar o tamanho de espaçamento de elementos na tela
	cout<<"\n     ";               //espaçamento necessário para o cabeçalho

	for(int i = 0; i <= n; i++)
	   cout<<KMAG<<setw(2)<<t[i] << " "; //imprimi o cabeçalho da matriz com 2 espaços

	cout<<RST<<"\n";

    //percorre toda a matriz D
	for(int i = 0; i <= getRowPrint(); i++){

		if(i == 0) cout<<"  ";
		else cout<<KGRN<<a[prime.j + i - 1]<<" "; // Coluna cabeçalho lateral
      cout<<RST;
		for(int l = 0; l <= n; l++){
		   int va = D[i][l];
		   if(va >= this->k && l > 0) cout<<KCYN; else cout<<RST;
			cout<<setw(2)<<va<< " "; //imprimi o valor da matriz
		}
		cout<<RST<<endl;
	}
}


//método que executa o comportamento original do algoritmo
int KdifferenceInexactMatch1heuristica1::executar(int m, int j){
   /**
      desalocar a última linha da matriz D
      quando um novo índice j de 'a' for passado. Isso garante que a primeira chamada do algoritmo
      usa a maior quantidade de memória já alocada (m+1 x n+1) e a cada nova chamada
      uma linha 'desnecessária' é removida
    */

    while(this->m > m)
       delete [] D[this->m--];

   this->m = m;

   int i, l;

   for(l = 0; l <= n; l++) /** for all l, 0 <= l <= n, D[0,l] <- 0 */
	  D[0][l] = 0;
   for(i = 1; i <= m; i++) /** for all i, 1 <= i <= m, D[i,0] <- i */
	  D[i][0] = i;

   bool passou;     //flag para verificar se toda a linha tem valores >= k
   int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
  // int jMax = 0;
	for(i = 1; i <= m && linha == -1; i++){ /** for i <- 1 to m */
	   passou = true;
		for(l = 1; l <= n; l++){ /** for l <- 1 to n */
         /** relação de recorrência */
			D[i][l] = menorDeTres(D[i-1][l] + 1,
                               D[i][l-1] + 1,
                               D[i-1][l-1] + (a[prime.j + i-1] == t[l-1] ? 0 : 1));
     //    if(D[i][l] == 0) jMax = i;
			if(passou && D[i][l] < k) passou = false; //se algum valor da linha é < k essa linha é descartada

		}
      if(passou && linha == -1) linha = i; //se a linha não foi descartada e ela é a primeira, guarda ela
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
