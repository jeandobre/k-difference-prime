/**
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2016
   FACOM: Mestrado em Ciência da Computação

   Este projeto implementa a versão modificada do k-difference inexact match por meio da
   matriz L [d x e], algoritmo híbrido, proposto por Landau Gad M. e Vishkin Uzi no artigo
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
#include <fstream>
#include <list>
#include "auxiliar.h"

#include "cst11/SSTree.h" //biblioteca arvore sufixo compressada
#include "cst11/Tools.h"

using namespace std;

//declaração da classe que é utilizada no KdifferencePrime
class KdifferenceInexactMatch3;

class KdifferencePrime{

    public:
      char *alpha, *beta;//seguindo o modelo proposto por GUSFIELD, nomes de variáveis que representam o padrão e o texto respectivamente
      int k;             //quantidade de diferênças
      int m;             //tamanho de alpha
      int n;             //tamanho de beta
      bool mostrarMatriz; //booleano 0,1 usado para imprime a matriz na tela
      int versao;     //booleano 0, 1 usado para executar a versão otimizada do algoritmo
      int j;             //índice de alpha necessário p/ posicionar corretamente na árvore de sufixo
      string ocorrencia;

      KdifferenceInexactMatch3 *c;

      SSTree *sst; //Arvore de Sufixo comprimida (CST)

      list<Primer *> primers; //lista que guarda as ocorrências de primers

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usuário não escolher nada
         this->mostrarMatriz=false;
      }
      ~KdifferencePrime(){
         delete c;
         delete sst;
      }
      void processar();

      void mostrarOcorrencias(){
         if(primers.size() == 0) cout<<MSG_0_OCCR<<k<<" diferenca(s)"<<endl;
         else{
              cout<<"Encontrado "<<primers.size()<<" ocorrencia(s) de primers ";
              if(primers.size() > 10){
                fstream out;
                string fileName = "dados/saida_a" + to_string(m) + "_b" + to_string(n) + "_k" + to_string(k);
                out.open(fileName, ios::out | ios::trunc);
                for(Primer *p : primers){
                  //out<<p->escreverArquivoReduzido();
                  out<<p->escreverArquivoCompleto();
                }
                out.close();
                cout<<"(Arquivo: " + fileName + ")"<<endl;
              }else {
                 for(Primer *p : primers){
                     p->escreverTela();
                 }
              }
         }
      }
} prime;

//O algoritmo foi adaptado para entregar o resultado do primer que é o inverso da programação original
//Classe que resolve o problema k-mismatches problems em tempo O(nk)
class KdifferenceInexactMatch3{

  private:
    int *L;   //Matriz L[-(k+1)..d]
    string a;  //padrão
    string t;  //texto

    int k;     //quantidade de diferênças
    int m, n;  //tamanho de a e t respectivamente
  public:
    int linha; //linha que contém o menor valor >= k

  private:
      int dToMatriz(int d){
      return d + k + 1; //converte índice d para índices reais
    }

  public:
    void setL(int d, int valor){
      L[dToMatriz(d)] = valor; //L[d,e] = valor
    }

    int getL(int d){
      return L[dToMatriz(d)]; // L[d,e]
    }

    int getDimensaoD(){
      return k + n + 3; //k, n+1, 1 da posição 0
    }

  public:
    KdifferenceInexactMatch3(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);

       //alocação dinâmica de inteiros
       this->L = new int[getDimensaoD()];
    }

    ~KdifferenceInexactMatch3(){
       //Ao destruir a classe desaloca toda memória que foi usada
       delete [] L;
    }
    void executar();
    void imprimirMatrizTela();

    void setaTamanhoM(){
      this->m = prime.m - prime.j;
    }
};

//método que imprimi a matriz na tela
void KdifferenceInexactMatch3::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espaçamento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -k; d <= n; d++){
        vr = getL(d);
        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}

//método que executa o comportamento original do algoritmo
void KdifferenceInexactMatch3::executar(){
    int d,e, row;

    //inicialização da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1);

    int long long pivo; //variável auxiliar para troca de posições
    bool passou = true; //flag para controlar o caso de alcançar o fim de m antes de k diferenças
    linha = -1;         //variável qwue guarda a primeira linha de ocorrência de primer
    for(e = 0; e < k; e++){
        pivo = -1;
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1),
                             getL(d) + 1,
                             getL(d+1) + 1);

           row = menorDeDois(row, m);
           //LCE (Segundo os autores o tempo é determinado por um valor K que eu não encontrei
           //para diminuir o tempo
           if(row < m && row + d < n)
             row += prime.sst->lce(prime.j + row , prime.m + 1 + row + d);//LCE(0,m+1);

           setL(d-1, pivo); //guarda o anterior
           if(row + d > n+1) row = -1;
           if(d == n) setL(d, row); //se for o último guarda o atual
           else pivo = row;

           //se já alcancou 'm' e o erro é menor que 'k' pode parar e ir para o próximo 'j'
           if(row == m) passou = false;
           if (passou && row > linha) linha = row;
        }
    }
    if(passou) linha++;
    else linha = -1;
}

//método que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: não há execução sem a invocação deste método
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorrências

   c = new KdifferenceInexactMatch3(alpha, beta, &k); //instancia apenas uma vez

   for(j = 0; j < (m - k) + 1; j++){
       c->setaTamanhoM();
       c->executar();

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

   time_t inicio, fim;

      //pré-processamento da árvore de sufixo
      cout<<"Pre-processando arvore de sufixo...";
      string texto;
      uchar *text = (uchar*) texto.append(prime.alpha).append("#").append(prime.beta).c_str();
      ulong n = strlen((char*)text);
      n++;

      inicio = clock();
      //constroi a árvore de sufixo comprimida para a sequencia generalizada: alpha # beta $
      prime.sst = new SSTree(text, n);

      fim = clock();

   cout<<"(Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")"<<endl;
   cout<<"K-difference-primer-3 executando..."<<endl;
   cout<<"Versao: CST"<<endl;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(tempo) formataTempo(tempo_execucao);

   return 0;
}
