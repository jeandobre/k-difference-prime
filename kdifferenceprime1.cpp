/* *******************************************************************
 *  Autor: Jean Alexandre Dobre                                      *
 *  Orientador: Said Sadique Adi                                     *
 *  Ano: 2015/2016                                                   *
 *  FACOM: Mestrado em Ciência da Computação                         *
 *                                                                   *
 * Este projeto implementa a versão original do k-difference inexact *
 * match por meio da matriz D [m x n], algoritmo serial, proposto    *
 * por Landau Gad M. e Vishkin Uzi no artigo INTRODUCING EFFICIENT   *
 * PARALLELISM INTO APROXIMATE STRING MATCHING AND A NEW SERIAL      *
 * ALGORITHM, página 222.                                            *
 *                                                                   *
 * A versão do k-difference primer foi idealizada pelo               *
 * Prof. Dr. Said Sadique Adi cuja idéia é verificar toda linha da   *
 * matriz D tem valor >= k.                                          *
 * A versão otimizada traz a i´deia de implementar a matriz D com    *
 * apenas 2 linhas [2 x n] que é o mínimo necessário para que a      *
 * recorrência aconteça. Essa versão diminui muito o uso de memória  *
 * mas não de processamento.
 *
 * Atualizado em 09/03/2016 versão com opt-2 que deve computar a     *
 * matriz  D1..m + 1 pivo e impentantação de leitura arquivo FASTA   *
 * e agrupamento em forma de projeto, removendo as funções auxilares *
 * para uma biblioteca de uso geral e por último a implementação de  *
 * um vetor<string> para guardar os resultados indexando pelo sufixo *
 * *******************************************************************/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "classes.h"
#include <stdexcept>


//O algoritmo foi adaptado para entregar o resultado do primer que é o inverso da programação original
//Classe que resolve o problema k-mismatches problems em tempo O(mn)
//Essa classe é abstrata permitindo a implementação do comportamento do algoritmo (Original e Otimizado)
class KdifferenceInexactMatch1: public KdifferenceInexactMatch{

  protected:
    int **D;      //Matriz D[0..m, 0..n] ou D[0..1, 0..n]

  public:
    KdifferenceInexactMatch1(char *a, char *t, int *k): KdifferenceInexactMatch(a,t,k){};
    ~KdifferenceInexactMatch1(){}

    virtual void executar(){};
    void imprimirMatrizTela(); //permite o cacheamento pois não muda o comportamento
};

//implementação original Matriz D[0..m, 0..n]
class KdifferenceInexactMatch1Original: public KdifferenceInexactMatch1{
 public:
   KdifferenceInexactMatch1Original(char *a, char *t, int *k): KdifferenceInexactMatch1(a,t,k) {
       //alocação dinâmica de inteiros, necessário mais um que é a posição 0
       this->D = new (nothrow) int*[m + 1];
       for (int j = 0; j <= m; ++j)
          this->D[j] = new (nothrow) int[n + 1];
   }
   ~KdifferenceInexactMatch1Original(){
       //Ao destruir a classe desaloca toda memória que foi usada
       for (int i = 0; i <= m; ++i)
         delete [] D[i];
       delete [] D;
    }

    string name() const {return "K1v3";};
    int executar(int m);

};

//implementação Matriz D[0..1, 0..n]
class KdifferenceInexactMatch1Optimizado1: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado1(char *a, char *t, int *k):KdifferenceInexactMatch1(a,t,k){

       //alocação de 2 linhas de inteiros por n + 1
       this->D = new (nothrow) int*[2];
       this->D[0] = new (nothrow) int[n + 1];
       this->D[1] = new (nothrow) int[n + 1];

   }

   ~KdifferenceInexactMatch1Optimizado1(){
       delete [] D[0];
       delete [] D[1];
       delete [] D;
    }

    string name() const {return "K1v2";};
    int executar(int m);
    int getRowPrint(){
      return 1;
    }
};

//implementação Matriz D[0, 0..n]
class KdifferenceInexactMatch1Optimizado2: public KdifferenceInexactMatch1{
  public:
   KdifferenceInexactMatch1Optimizado2(char *a, char *t, int *k):KdifferenceInexactMatch1(a,t,k){

       //alocação de 1 linha de inteiros por n + 1
       this->D = new (nothrow) int*[1];
       this->D[0] = new (nothrow) int[n + 1];
   }

   ~KdifferenceInexactMatch1Optimizado2(){
       delete [] D[0];
       delete [] D;
    }

   string name() const {return "K1v1";};
   int executar(int m);
   int getRowPrint(){
      return 0;
    }
};

class KdifferencePrime1: public KdifferencePrime{

    public:
      void instanciar(){
         if(versao == 1) //se o usuário escolheu a versão otimizada 2
            c = new KdifferenceInexactMatch1Optimizado2(alpha, beta, &k); //instancia a classe Otimizada
         else if(versao == 2) //se o usuário escolheu a versão otimizada
            c = new KdifferenceInexactMatch1Optimizado1(alpha, beta, &k); //instancia a classe Otimizada
         else  //se o usuário escolheu a versão normal
            c = new KdifferenceInexactMatch1Original(alpha, beta, &k);  //instancia a classe Original
      }

} prime; // é necessário apenas uma instancia de prime, já declarada aqui

//método que imprimi a matriz na tela
void KdifferenceInexactMatch1::imprimirMatrizTela(){

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
int KdifferenceInexactMatch1Original::executar(int m){
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
	for(i = 1; i <= m && linha == -1; i++){ /** for i <- 1 to m */
	   passou = true;
		for(l = 1; l <= n; l++){ /** for l <- 1 to n */
         /** relação de recorrência */
			D[i][l] = menorDeTres(D[i-1][l] + 1,
                               D[i][l-1] + 1,
                               D[i-1][l-1] + (a[prime.j + i-1] == t[l-1] ? 0 : 1));

			if(passou && D[i][l] < k) passou = false; //se algum valor da linha é < k essa linha é descartada
		}
      if(passou && linha == -1) linha = i; //se a linha não foi descartada e ela é a primeira, guarda ela
	}
	return linha;
}

//método que executa o comportamento otimizado do algoritmo
int KdifferenceInexactMatch1Optimizado1::executar(int m){
   this->m = m;
   int l;

   for(l = 0; l <= n; l++) /** necessário inicializar apenas a primeira linha */
	  D[0][l] = 0;

   bool passou; //flag para verificar se toda a linha tem valores >= k
   int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
   for(int i = 1; i <= m && linha == -1; i++){
     D[1][0] = i;
     passou = true;
	 for(l = 1; l <= n; l++){
        D[1][l] = menorDeTres(D[0][l]   + 1,
                              D[1][l-1] + 1,
                              D[0][l-1] + (a[prime.j + i-1] == t[l-1] ? 0 : 1));

        if(passou && D[1][l] < k) passou = false; //se algum valor da linha é < k essa linha é descartada
        //aqui reorganiza a matriz para o próximo laço
        D[0][l-1] = D[1][l-1];
        D[1][l-1] = 0;
     }
     D[0][n] = D[1][n];
     D[1][n] = 0;
     if(passou) linha = i; //se a linha não foi descartada ela é a primeira
   }
   return linha;
}

//método que executa o comportamento otimizado do algoritmo
int KdifferenceInexactMatch1Optimizado2::executar(int m){
   this->m = m;

   for(int l = 0; l <= n; l++) /** necessário inicializar apenas a primeira linha */
	  D[0][l] = 0;

   bool passou;     //flag para verificar se toda a linha tem valores >= k
   int linha = -1;  //flag que marca a primeira linha em que a condição acima foi satisfeita
   int pivo,aux;
   char tt;

   for(int i = 1; i <= m && linha == -1; i++){
     pivo = i; passou = true;
     tt = a[prime.j + i-1];
	  for(int l = 1; l <= n; l++){
       aux = menorDeTres(D[0][l] + 1,
                         pivo + 1,
                         D[0][l-1] + (tt == t[l-1] ? 0 : 1));
	    D[0][l-1] = pivo;
	    pivo = aux;
	    if(passou && pivo < k) passou = false;
	  }
	  D[0][n] = aux;
     if(passou) linha = i; //se a linha não foi descartada ela é a primeira
   }
   return linha;
}

int main(int argc, char** argv) {

   Parametro *p = parseParametros(argc, argv);
   if(p == NULL) return 0;

   prime.setaParametros(p);

   //se o usuário escolheu o J, devemos fazer algumas checagens:
   //1 - o j é valido (entre 0 e m)
   //2 - a distância é valida (entre 0 e j + distancia <= m)
   if(p->Jsetado){
     if(p->Jselecionado < 0 || p->Jselecionado > prime.m){ cout<<FRED(ERR_J)<<"\n"; return 0; }
     else if(p->Jdistancia < 1 || (p->Jdistancia + p->Jselecionado) > prime.m){ cout<<FRED(ERR_DISTANCIA)<<"\n"; return 0; }
   }

   if(prime.k > prime.m){
     cout<<"\n"<<FRED(ERR_KMAIOR)<<prime.m<<"\n";
     return 0;
   }

   if(prime.versao > 3 || prime.versao < 1){
     cout<<FRED(MSG_VERSAO_INCORRETA);
     cout<<FCYN(MSG_VERSAO_K1_VS1)<<MSG_VERSAO_K1_VS2<<MSG_VERSAO_K1_VS3;
     return 0;
   }

   cout<<"K-difference-primer-1 processando...\n";
   cout<<"Versao do algoritmo: ";
   !(p->escolheuVersao) ? cout<<FCYN("delfaut") : cout<<prime.versao;
   if(prime.mostrarMatriz) cout<<"\n"<<FMAG(MSG_MATRIZ);
   if(p->mostrarLog) cout<<KYEL<<"\nLog de infomacoes ativado.\n"<<RST;
   if(p->Jsetado) {
      if(p->Jdistancia > 1){
       cout<<KCYN<<"\nProcessando os indices "<<RST<<p->Jselecionado;
       cout<<KCYN<<" ate "<<RST<<(p->Jdistancia + p->Jselecionado) -1;
      } else cout<<KCYN<<"\nProcessando o indice "<<RST<<p->Jselecionado;
      cout<<KCYN<<" de alfa.\n"<<RST;
   }
   cout<<endl;

   time_t inicio, fim;

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
