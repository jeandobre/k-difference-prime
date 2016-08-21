/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ciência da Computação
   Este projeto implementa a versão modificada do k-difference inexact match por meio da
   matriz L [d x e], algoritmo serial, proposto por Landau Gad M. e Vishkin Uzi no artigo
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
#include <list>
#include "classes.h"

using namespace std;

//O algoritmo foi adaptado para entregar o resultado do primer que é o inverso da programação original
//Classe que resolve o problema k-mismatches problems em tempo O(kmn)
class KdifferenceInexactMatch2: public KdifferenceInexactMatch{

  protected:
    int **L;   //Matriz L[-(k+1)..d, -1..e]

  private:
    int dToMatriz(int d){
      return d + (k + 1); //converte índice d para índices reais
    }

    int eToMatriz(int e){
      return e + 1; //converte índice e para índices reais
    }

  public:
    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor; //L[d,e] = valor
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //k, n+1, 1 da posição 0
    }

    int getDimensaoE(){
      return k + 2; // k + 1, n+1, 1 da posição 0
    }
  public:
    KdifferenceInexactMatch2(char *a, char *t, int *k): KdifferenceInexactMatch(a, t, k){

       //alocação dinâmica de inteiros
       this->L = new int*[getDimensaoD()];
       if(getDimensaoE() > 0)
           for (int j = 0; j < getDimensaoD(); ++j)
              this->L[j] = new int[getDimensaoE()];

    }

    ~KdifferenceInexactMatch2(){
       //Ao destruir a classe desaloca toda memória que foi usada
       if(getDimensaoE() > 0)
           for (int i = 0; i < getDimensaoD(); ++i)
             delete [] L[i];
       delete [] L;
    }
    virtual int executar(int m){};
    virtual void imprimirMatrizTela(){};
};

//Classe que implementa o algoritmo proposto por Vishkin e Landau
class KdifferenceInexactMatch2Original: public KdifferenceInexactMatch2{

public:
    KdifferenceInexactMatch2Original(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    int executar(int m);
    void imprimirMatrizTela();
};

//Classe que impelementa o comportamento original do algoritmo mas utilizando um array
//para computar o Lde
class KdifferenceInexactMatch2Optimizado: public KdifferenceInexactMatch2{

   private:
      int dToMatriz(int d){
      return d + k + 1; //converte índice d para índices reais da matriz fixa
    }

    int eToMatriz(int e){
      return 0; //converte sempre p coluna 0
    }

  public:
    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor; //L[d,e] = valor
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //-(k+1) -k..0 1..n (n+1)
    }

    int getDimensaoE(){
      return 0; //não tem tamanho
    }
    KdifferenceInexactMatch2Optimizado(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    int executar(int m);
    void imprimirMatrizTela();
};

class KdifferencePrime2: public KdifferencePrime{
    public:
      void instanciar(){
           if(versao == 1) c = new KdifferenceInexactMatch2Optimizado(alpha, beta, &k);
           else            c = new KdifferenceInexactMatch2Original  (alpha, beta, &k);
      };
} prime;

//método que imprimi a matriz na tela
void KdifferenceInexactMatch2Original::imprimirMatrizTela(){
cout << setfill(' ');     //setar o tamanho de espaçamento de elementos na tela

    cout<<endl<<setw(4)<<" "; //espaçamento necessário para o cabeçalho
	for(int i = -1; i <= k; i++) cout<<setw(2)<<i<< " "; //imprimi o cabeçalho da matriz com 2 espaços

	int vr;
    cout<<endl;
    for(int i = -(k+1), x = 0; i <= n+1; i++, x++){

        cout<<setw(3)<<i<<" ";
        for(int l = -1, y = 0; l <= k; l++, y++){
            vr = getL(i,l);

           // if ((x <= k || y <= k) && (x+y) <= k)cout<<setw(3)<<" ";
			//else
			cout<<setw(2)<<vr<<" ";
		}
		cout<<"\n";
	}
}

//método que executa o comportamento original do algoritmo
int KdifferenceInexactMatch2Original::executar(int m){
   this->m = m;
   int d,e, row;

	for(d = 0; d <= n; d++) /** for all d, 0 <= d <= n, L[d,-1] <= -1 */
        setL(d, - 1, -1);

    for(d = -(k+1); d <= -1; d++){ /** for all d, -(k+1) <= d <= -1 */
      setL(d, abs(d) - 2, abs(d) - 2); /** L[d,|d|-1] <= |d|-1 */
      setL(d, abs(d) - 1, abs(d) - 1); /** L[d,|d|-2] <= |d|-2 */
    }

    for(e = -1; e <= k; e++) /** for all e, -1 <= e <= k, L[n+1, e] <= -1 */
        setL(n+1, e, -1);

    bool passou = true;
    int linha = -1;
    for(e = 0; e <= k && passou; e++){
        for(d = -(e); d <= n; d++){
           row = maiorDeTres(getL(d-1, e-1),
                        getL(d,   e-1) + 1,
                        getL(d+1, e-1) + 1);
           row = menorDeDois(row, m);
           //LCE
           while(row < m && row+d < n && a[row + prime.j] == t[row+d])
             row++;

           setL(d, e, row);
           //se já alcancou 'm' e o erro é menor que 'k' pode parar e ir para o próximo 'j'
           if(row == m && e < k){passou = false; continue;}
        }
    }

    /*
       se passou então existe uma coluna 'e' = k-1 com a maior índice
       basta localizar na linha qual é o maior valor e incrementar 1
       isso ocorre pois a matriz guarda o índice final do LCE, logo o inicio do LCE(k) é LCE(k-1)+1.
       fazer uma comparação ao final é mais rápido que adicionar uma condição no algoritmo principal
    */
    if(passou){
        row = -1;
        for(d = -k; d <= n; d++){ //percorre toda a coluna k-1 procurando pelo maior valor
            row = getL(d, k-1);
            if (row > linha) linha = row;
        }
    }
    /*
       (a) Para todo i, l, D[i,l] - D[i-1,l-1] é ou Zero ou 1
       (b) d > n - m + k + 1 ou d < -k são descartáveis para solução deste problema
       (c) a inicialização apenas atualiza a matriz com os limites
    */
    return (passou ? ++linha : -1); //retorna a linha de ocorrência de primer ou -1 que indica não correência
}

//método que imprimi a matriz na tela
void KdifferenceInexactMatch2Optimizado::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espaçamento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -(k+1); d <= (n+1); d++){
        vr = getL(d,0);

        cout<<setw(2)<<d<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}


//método que executa a versão otimizada do algoritmo. A mudança é apenas na armazenagem de dados,
//pois nesta versão é utilizado um array no lugar da matriz Lde
int KdifferenceInexactMatch2Optimizado::executar(int m){
    this->m = m;
    int d,e, row;

    //inicialização da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1, -1);

    int long long pivo; //variável auxiliar para troca de posições
    bool passou = true; //flag para controlar o caso de alcançar o fim de m antes de k diferenças
             //variável que guarda sempre o maior valor da coluna e
    for(e = 0; e < k && passou; e++){
        pivo = -1; //a cada nova coluna a variável linha é reiniciada
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1, e-1),
                             getL(d,   e-1) + 1,
                             getL(d+1, e-1) + 1);

           row = menorDeDois(row, m);
           while(row < prime.m && row+d < n && a[prime.j + row] == t[row+d]) row++; //LCE

           //se já alcancou 'm' e o erro é menor que 'k' pode parar e ir para o próximo 'j'
           if(row == m){passou = false; continue;}
           setL(d-1, e, pivo); //atualiza a coluna e guardando o pivo no espaço anterior
           pivo = row;
        }
       setL(n, e, row);
    }
    int linha;
    //fazer uma comparação ao final é mais rápido que adicionar uma condição no algoritmo principal
    if(passou){
        row = -1;
        for(d = -k; d <= n; d++){ //percorre toda a coluna k-1 procurando pelo maior valor
            row = getL(d, k-1);
            if (row > linha) linha = row;
        }
    }
    return (passou ? ++linha : -1); //retorna a linha de ocorrência de primer ou -1 que indica não ocorrência
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

   if(prime.versao > 2 || prime.versao < 1){
     cout<<MSG_VERSAO_INCORRETA;
     cout<<MSG_VERSAO_K2_VS1<<MSG_VERSAO_K2_VS2;
     return 0;
   }

   cout<<"K-difference-primer-2 processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;

   time_t inicio, fim;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(prime.tempo) formataTempo(tempo_execucao);

   return 1;
}
