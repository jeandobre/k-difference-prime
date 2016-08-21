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
#include "classes.h"

#include "cst_v_1_1/SSTree.h" //biblioteca arvore sufixo compressada
#include "cst_v_1_1/Tools.h"

using namespace std;

//declaração da classe que é utilizada no KdifferencePrime
class KdifferenceInexactMatch3CST: public KdifferenceInexactMatch234{
  public:
    KdifferenceInexactMatch3CST(int *k): KdifferenceInexactMatch234(a,t,k){};
    int executar(int m);
};


class KdifferencePrime3: public KdifferencePrime{

    public:
      SSTree *sst; //Arvore de Sufixo comprimida (CST)

    public:
      KdifferencePrime3(){
         //construtor seta os valores default se o usuário não escolher nada
         this->mostrarMatriz=false;
      }
      ~KdifferencePrime3(){
         delete c;
         delete sst;
      }
      void instanciar(){
        //implementação da arvore de sufixo compressada
        c = new KdifferenceInexactMatch3CST(&k);
      };
} prime;

//método que executa o comportamento original do algoritmo
int KdifferenceInexactMatch3CST::executar(int m){
    this->m = m;
    int d,e, row;

    //inicialização da matriz
    for(d = -(k+1); d <= (n+1); d++)
        setL(d, -1);

    int long long pivo; //variável auxiliar para troca de posições
    bool passou = true; //flag para controlar o caso de alcançar o fim de m antes de k diferenças
    int linha = -1;         //variável qwue guarda a primeira linha de ocorrência de primer
    for(e = 0; e < k && passou; e++){
        pivo = linha = -1; //a cada nova coluna a variável linha é reiniciada
        for(d = -e; d <= n && passou; d++){
           row = maiorDeTres(getL(d-1),
                             getL(d)   + 1,
                             getL(d+1) + 1);
           row = menorDeDois(row, m);

           if(row + d < n)
             row += prime.sst->lce(prime.j + row , prime.m + 1 + row + d);//LCE(0,m+1);

           if(row == m){passou = false; continue;} //se já alcancou 'm' e o erro é menor que 'k' pode parar e ir para o próximo 'j'
           setL(d-1, pivo); //atualiza a coluna e guardando o pivo no espaço anterior
           if (row > linha) linha = row;
           pivo = row;
        }
        setL(n, row);
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

   if(prime.versao != 1){
     cout<<MSG_VERSAO_INCORRETA;
     cout<<MSG_VERSAO_K3_VS1;
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
   cout<<"Versao:"<<prime.versao<<endl;

   inicio = clock();
   prime.processar();
   fim = clock();

   prime.mostrarOcorrencias();
   long long int tempo_execucao = ((fim - inicio) / (CLOCKS_PER_SEC / 1000));
   if(prime.tempo) formataTempo(tempo_execucao);

   return 0;
}
