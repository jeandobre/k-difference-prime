#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcr "

using namespace std;

class KdifferenceInexactMatch;

class Primer{

public:
   int ocr;
   int j;
   int r;
   string sequencia;

public:
   Primer(int ocr, int j, int r, string sequencia){
      this->ocr = ocr;
      this->j = ++j;
      this->r = --r;
      this->sequencia = sequencia;
   };

   void escreverTela(){
      cout<<setfill('0');
      cout<<MSG_N_OCCR<<setw(3)<<ocr<<" em ";

      cout<< setfill(' ');
      cout<<setw(3)<<j<<".."<<setw(3)<<j + r;
      cout<<"-Tamanho("<<setw(3)<<sequencia.size()<<"): ";
      cout<<sequencia<<endl;
   };


   string escreverArquivoCompleto(){
     string *retorno = new string(to_string(ocr));
     retorno->append(";");
     retorno->append(to_string(j));
     retorno->append(";");
     retorno->append(to_string(r));
     retorno->append(";");
     retorno->append(sequencia);
     retorno->append(";");
     retorno->append(to_string(sequencia.size()));
     retorno->append("\n");
     return *retorno;
   }

   string escreverArquivoReduzido(){
     string *retorno = new string(to_string(j));
     retorno->append(";");
     retorno->append(to_string(j + r));
     retorno->append("\n");
     return *retorno;
   }
};

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

      KdifferenceInexactMatch *c;

      list<Primer *> primers; //lista que guarda as ocorrências de primers

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usuário não escolher nada
         this->mostrarMatriz=false;
      }
      ~KdifferencePrime(){};
      virtual void processar(){};

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
};

class KdifferenceInexactMatch{

  protected:
    char *t;     //texto
    char *a;     //padrão
    int k;        //quantidade de diferênças
    int m, n;     //tamanho de a e t respectivamente
    int rowPrint; //variável auxiliar que recebe o valor limite da linha de impressão na tela, linha máxima

  public:
    KdifferenceInexactMatch(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);
    };

    ~KdifferenceInexactMatch(){};

    virtual int executar(){};//executa o algoritmo kdifference inexact match e retorna a linha da ocorrência de prime
    virtual void imprimirMatrizTela()const {}; //permite o cacheamento pois não muda o comportamento
    virtual void setaTamanhoM(){};
};
