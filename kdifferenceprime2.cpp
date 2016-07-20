/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ci�ncia da Computa��o
   Este projeto implementa a vers�o modificada do k-difference inexact match por meio da
   matriz L [d x e], algoritmo serial, proposto por Landau Gad M. e Vishkin Uzi no artigo
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
#include <list>
#include "auxiliar.h"

using namespace std;

//declara��o da classe que � utilizada no KdifferencePrime
class KdifferenceInexactMatch2;

class KdifferencePrime{

    public:
      string alpha, beta;//seguindo o modelo proposto por GUSFIELD, nomes de vari�veis que representam o padr�o e o texto respectivamente
      int k;             //quantidade de difer�n�as
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      bool mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int versao;     //booleano 0, 1 usado para executar a vers�o otimizada do algoritmo

      KdifferenceInexactMatch2 *c;
      list<Primer *> primers;

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usu�rio n�o escolher nada
         this->mostrarMatriz=false;
         this->versao=1; //apenas uma vers�o
         primers.clear();
      }
      void processar();
} prime;

//O algoritmo foi adaptado para entregar o resultado do primer que � o inverso da programa��o original
//Classe que resolve o problema k-mismatches problems em tempo O(nk)
class KdifferenceInexactMatch2{

  protected:
    int **L;   //Matriz L[-(k+1)..d, -1..e]
    string a;  //padr�o
    string t;  //texto
    int k;     //quantidade de difer�n�as
    int m, n;  //tamanho de a e t respectivamente
  public:
    int linha; //linha que cont�m o menor valor >= k

  private:
    int dToMatriz(int d){
      return d + (k + 1); //converte �ndice d para �ndices reais
    }

    int eToMatriz(int e){
      return e + 1; //converte �ndice e para �ndices reais
    }

  public:
    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor; //L[d,e] = valor
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //k, n+1, 1 da posi��o 0
    }

    int getDimensaoE(){
      return k + 2; // k + 1, n+1, 1 da posi��o 0
    }
  public:
    KdifferenceInexactMatch2(string &a, string &t, int &k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();

       //aloca��o din�mica de inteiros
       this->L = new int*[getDimensaoD()];
       if(getDimensaoE() > 0)
           for (int j = 0; j < getDimensaoD(); ++j)
              this->L[j] = new int[getDimensaoE()];

    }

    ~KdifferenceInexactMatch2(){
       //Ao destruir a classe desaloca toda mem�ria que foi usada
       if(getDimensaoE() > 0)
           for (int i = 0; i < getDimensaoD(); ++i)
             delete [] L[i];
       delete [] L;
    }
    virtual void executar(){};
    virtual void imprimirMatrizTela(){};

    void setA(string a){
      this->a = a;
      this->m = a.size(); //atualiza o valor de 'm' com o novo tamanho de 'a'
    }
};

class KdifferenceInexactMatch2Original: public KdifferenceInexactMatch2{

public:
    KdifferenceInexactMatch2Original(string a, string t, int k): KdifferenceInexactMatch2(a,t,k){};
    void executar();
    void imprimirMatrizTela();
};

//m�todo que executa o comportamento original do algoritmo
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
           while(row < m && row+d < n && a.compare(row, 1, t, row+d, 1) == 0)
             row++;

           setL(d, e, row);
           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m && e < k) passou = false;
        }
    }

    /*
       se passou ent�o existe uma coluna 'e' = k-1 com a maior �ndice
       basta localizar na linha qual � o maior valor e incrementar 1
       isso ocorre pois a matriz guarda o �ndice final do LCE, logo o inicio do LCE(k) � LCE(k-1)+1.
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
       (a) Para todo i, l, D[i,l] - D[i-1,l-1] � ou Zero ou 1
       (b) d > n - m + k + 1 ou d < -k s�o descart�veis para solu��o deste problema
       (c) a inicializa��o apenas atualiza a matriz com os limites
    */
}

class KdifferenceInexactMatch2Optimizado1: public KdifferenceInexactMatch2{

   private:
      int dToMatriz(int d){
      return d + k; //converte �ndice d para �ndices reais
    }

    int eToMatriz(int e){
      return e; //converte �ndice e para �ndices reais
    }

  public:
    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor; //L[d,e] = valor
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 1; //k, n+1, 1 da posi��o 0
    }

    int getDimensaoE(){
      return k; // k + 1, 1 da posi��o 0
    }
    KdifferenceInexactMatch2Optimizado1(string a, string t, int k): KdifferenceInexactMatch2(a,t,k){};
    void imprimirMatrizTela();
    void executar();
};

class KdifferenceInexactMatch2Optimizado2: public KdifferenceInexactMatch2{

   private:
      int dToMatriz(int d){
      return d + k; //converte �ndice d para �ndices reais
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
      return k + n + 1; //k, n+1, 1 da posi��o 0
    }

    int getDimensaoE(){
      return 0; //n�o tem tamanho
    }
    KdifferenceInexactMatch2Optimizado2(string a, string t, int k): KdifferenceInexactMatch2(a,t,k){};
    void imprimirMatrizTela();
    void executar();
};
//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch2Optimizado1::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela

    cout<<endl<<setw(4)<<" "; //espa�amento necess�rio para o cabe�alho
	for(int i = 0; i < k; i++) cout<<setw(2)<<i<< " "; //imprimi o cabe�alho da matriz com 2 espa�os

	int vr;
    cout<<endl;
    for(int i = -k, x = 0; i <= n; i++, x++){

        cout<<setw(3)<<i<<" ";
        for(int l = 0, y = 0; l < k; l++, y++){
            vr = getL(i,l);

			if(vr > m || vr < 0) cout<<setw(3)<<" ";
			else cout<<setw(2)<<vr<<" ";
		}
		cout<<"\n";
	}
}

//m�todo que executa o comportamento original do algoritmo
void KdifferenceInexactMatch2Optimizado1::executar(){
    int d,e, row;

    //inicializa��o das bordas limites
    for(e = 1; e < k; e++){
       setL(-k, e, -1);
       setL( n, e, -1);
    }

    //inicializa��o da coluna e = 0
    bool passou = true;
    linha = -1;
    for(d = -k; d <= n && passou; d++){
        if(d < 0 || d == n) setL(d, 0, -1);
        else {
            row = LCEiterativo(1,d+1,a,t);
            setL(d, 0, row);
            if(row == m) passou = false;
            if (passou && row > linha) linha = row;
        }
    }

    for(e = 1; e < k; e++){
        for(d = -k+1; d < n && passou; d++){
           row = maiorDeTres(getL(d-1, e-1),
                             getL(d,   e-1) + 1,
                             getL(d+1, e-1) + 1);

           row = menorDeDois(row, m);
           //LCE
           row += LCEiterativo(row+1, row+d+1, a, t);

           if (row + d <= n) setL(d, e, row);
           else setL(d, e, -1);

           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m) passou = false;
           if (passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;
}

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch2Original::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela

    cout<<endl<<setw(4)<<" "; //espa�amento necess�rio para o cabe�alho
	for(int i = -1; i <= k; i++) cout<<setw(2)<<i<< " "; //imprimi o cabe�alho da matriz com 2 espa�os

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

//m�todo que executa o comportamento original do algoritmo
void KdifferenceInexactMatch2Optimizado2::executar(){
    int d,e, row;

    //inicializa��o da coluna e = 0
    bool passou = true;
    linha = -1;
    for(d = -k; d <= n && passou; d++){
        if(d < 0 || d == n) setL(d, 0, -1);
        else {
            row = LCEiterativo(1,d+1,a,t);
            setL(d, 0, row);
            if(row == m) passou = false;
            if (passou && row > linha) linha = row;
        }
    }
    //int long long aux;
    int long long pivo;

    for(e = 1; e < k; e++){
        pivo = -1;
        for(d = -e; d < n && passou; d++){
           row = maiorDeTres(getL(d-1, e-1),
                             getL(d,   e-1) + 1,
                             getL(d+1, e-1) + 1);

           row = menorDeDois(row, m);
           //LCE
           row += LCEiterativo(row+1, row+d+1, a, t);

           setL(d-1, e, pivo); //guarda o anterior
           if(row + d > n) row = -1;
           if(d == n-1)setL(d, e, row); //se for o �ltimo guarda o atual
           else pivo = row;

           //se j� alcancou 'm' e o erro � menor que 'k' pode parar e ir para o pr�ximo 'j'
           if(row == m) passou = false;
           if (passou && row > linha) linha = row;
        }
        //if (e >= 1) imprimirMatrizTela();
    }
    if(passou) linha++;
    else linha = -1;
}

//m�todo que imprimi a matriz na tela
void KdifferenceInexactMatch2Optimizado2::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espa�amento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -k; d <= n; d++){
        vr = getL(d,0);

        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}

//m�todo que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: n�o h� execu��o sem a invoca��o deste m�todo
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorr�ncias

   if(versao == 1)
       c = new KdifferenceInexactMatch2Optimizado2(alpha, beta, k); //instancia a vers�o original
   else if(versao == 2)
       c = new KdifferenceInexactMatch2Optimizado1(alpha, beta, k);
   else
       c = new KdifferenceInexactMatch2Original(alpha, beta, k); //instancia a vers�o original

   for(int j = 0; j < (m - k) + 1; j++){
       //cout<<j<<","; //TODO retirar depois, apenas para
       c->setA(alpha.substr(j));
       c->executar();
       //if(j == 0) c->imprimirMatrizTela();

       if(c->linha > -1){ //se a linha foi obtida, significa que existe uma ocorr�ncia
           Primer *pr = new Primer(++ocr, j, c->linha, alpha.substr(j, c->linha));
           primers.insert(primers.end(), pr);
//           if(mostrarMatriz) c->escreverMatrizTela();
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

   cout<<"K-difference-primer-2 processando..."<<endl;
   cout<<"Versao do algoritmo: "<<prime.versao<<endl;

   time_t inicio, fim;

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
     cout<<"Tempo de execucao: "<< ((fim - inicio) / (CLOCKS_PER_SEC / 1000)) << " milisegundos";

   return 1;
}
