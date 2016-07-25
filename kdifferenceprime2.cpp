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
#include "auxiliar.h"

using namespace std;

//declaração da classe que é utilizada no KdifferencePrime
class KdifferenceInexactMatch2;

class KdifferencePrime{

    public:
      char *alpha, *beta;//seguindo o modelo proposto por GUSFIELD, nomes de variáveis que representam o padrão e o texto respectivamente
      int k;             //quantidade de diferênças
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      bool mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int versao;     //booleano 0, 1 usado para executar a versão otimizada do algoritmo
      string ocorrencia;
      int j;

      KdifferenceInexactMatch2 *c;
      list<Primer *> primers;

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usuário não escolher nada
         this->mostrarMatriz=false;
         this->versao=1; //apenas uma versão
         primers.clear();
      }
      void processar();

      void mostrarOcorrencias(){
         if(primers.size() == 0) cout<<MSG_0_OCCR<<k<<" diferenca(s)"<<endl;
         else{
              cout<<"Encontrado "<<primers.size()<<" ocorrencia(s) r de primers"<<endl;
              for(Primer *p : primers){
                  p->escreverTela();
              }
         }
      }
} prime;

//O algoritmo foi adaptado para entregar o resultado do primer que é o inverso da programação original
//Classe que resolve o problema k-mismatches problems em tempo O(nk)
class KdifferenceInexactMatch2{

  protected:
    int **L;   //Matriz L[-(k+1)..d, -1..e]
    char *a;  //padrão
    char *t;  //texto
    int k;     //quantidade de diferênças
    int m, n;  //tamanho de a e t respectivamente
  public:
    int linha; //linha que contém o menor valor >= k

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
    KdifferenceInexactMatch2(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);

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
    virtual void executar(){};
    virtual void imprimirMatrizTela(){};

    void setaTamanhoM(){
       this->m = prime.m - prime.j;
    };
};

class KdifferenceInexactMatch2Original: public KdifferenceInexactMatch2{

public:
    KdifferenceInexactMatch2Original(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    void executar();
    void imprimirMatrizTela();
};

//método que executa o comportamento original do algoritmo
void KdifferenceInexactMatch2Original::executar(){

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
    linha = -1;
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
           if(row == m && e < k) passou = false;
        }
    }

    /*
       se passou então existe uma coluna 'e' = k-1 com a maior índice
       basta localizar na linha qual é o maior valor e incrementar 1
       isso ocorre pois a matriz guarda o índice final do LCE, logo o inicio do LCE(k) é LCE(k-1)+1.
    */
    if(passou){
        row = -1;
        for(d = -(k+1); d <= n; d++){
            row = getL(d, k-1);
            if (row > linha) linha = row;
        }
        linha++;
    }
    /*
       (a) Para todo i, l, D[i,l] - D[i-1,l-1] é ou Zero ou 1
       (b) d > n - m + k + 1 ou d < -k são descartáveis para solução deste problema
       (c) a inicialização apenas atualiza a matriz com os limites
    */
}

class KdifferenceInexactMatch2Optimizado1: public KdifferenceInexactMatch2{

   private:
      int dToMatriz(int d){
      return d + k; //converte índice d para índices reais
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
      return k + n + 1; //k, n+1, 1 da posição 0
    }

    int getDimensaoE(){
      return 0; //não tem tamanho
    }
    KdifferenceInexactMatch2Optimizado1(char *a, char *t, int *k): KdifferenceInexactMatch2(a,t,k){};
    void imprimirMatrizTela();
    void executar();
};

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
void KdifferenceInexactMatch2Optimizado1::executar(){
    int d,e, row;

    //inicialização da coluna e = 0
    bool passou = true;
    linha = -1;
    for(d = -k; d <= n && passou; d++){
        if(d < 0 || d == n) setL(d, 0, -1);
        else {
            row = directCompLCE(prime.j + 1,d+1, a,t, prime.m,n);
            setL(d, 0, row);
            if(row == m) passou = false;
            if (passou && row > linha) linha = row;
        }
    }

    int long long pivo;

    for(e = 1; e < k; e++){
        pivo = -1;
        for(d = -e; d < n && passou; d++){
           row = maiorDeTres(getL(d-1, e-1),
                             getL(d,   e-1) + 1,
                             getL(d+1, e-1) + 1);

           row = menorDeDois(row, m);
           //LCE
           row += directCompLCE(prime.j + row+1, row+d+1, a, t, prime.m, n);

           setL(d-1, e, pivo); //guarda o anterior
           if(row + d > n) row = -1;
           if(d == n-1) setL(d, e, row); //se for o último guarda o atual
           else pivo = row;

           //se já alcancou 'm' e o erro é menor que 'k' pode parar e ir para o próximo 'j'
           if(row == m) passou = false;
           if (passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;
}

//método que imprimi a matriz na tela
void KdifferenceInexactMatch2Optimizado1::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espaçamento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -k; d <= n; d++){
        vr = getL(d,0);

        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}

//método que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: não há execução sem a invocação deste método
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorrências

   if(versao == 1)
       c = new KdifferenceInexactMatch2Optimizado1(alpha, beta, &k);
   else
       c = new KdifferenceInexactMatch2Original(alpha, beta, &k); //instancia a versão original

   for(j = 0; j < (m - k) + 1; j++){
       c->setaTamanhoM();
       c->executar();
       //if(j == 0) c->imprimirMatrizTela();

       if(c->linha > -1){ //se a linha foi obtida, significa que existe uma ocorrência
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

   cout<<"K-difference-primer-2 processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;

   time_t inicio, fim;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();

   if(tempo)
     cout<<"Tempo de execucao: "<< ((fim - inicio) / (CLOCKS_PER_SEC / 1000)) << " milisegundos"<<endl;

   return 1;
}
