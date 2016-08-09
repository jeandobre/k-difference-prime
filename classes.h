//biblioteca de classes utilizadas nos programas k-difference prime

#ifndef _classes_H_
#define _classes_H_

#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <list>
#include <cstdlib>
#include <iostream>

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-vs%] \tversao do algoritmo (1,2 ou 3)\n [-st] \tmostrar o tempo de execucao"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 1 e "
#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcr "

#define MSG_VERSAO_INCORRETA "\nVersao incorreta de implementacao do algoritmo\nUso correto:\n"
#define MSG_VERSAO_K1_VS1 "\t-vs1 (default) versao que utiliza apenas uma linha para computar a matriz D.\n"
#define MSG_VERSAO_K1_VS2 "\t-vs2           versao que utiliza duas linhas para computar a matriz D.\n"
#define MSG_VERSAO_K1_VS3 "\t-vs3           versao original do algoritmo com m linhas para computar a matriz D.\n"
#define MSG_VERSAO_K2_VS1 "\t-vs1 (default) versao que utiliza apenas uma coluna para computar a matriz Lde.\n"
#define MSG_VERSAO_K2_VS2 "\t-vs2           versao original do algoritmo com e colunas para computar a matriz Lde.\n"
#define MSG_VERSAO_K3_VS1 "\t-vs1 (default) versao que utiliza arvore de sufixo compressada e consulta LCE em tempo O(k).\n"
#define MSG_VERSAO_K4_VS1 "\t-vs1           versao que utiliza array de sufixo compressada + lcp + DirectMin.\n"
#define MSG_VERSAO_K4_VS2 "\t-vs2           versao que utiliza array de sufixo + lcp + DirectMin.\n"
#define MSG_VERSAO_K4_VS3 "\t-vs3 (default) versao que utiliza array de sufixo + lcp + consulta RMQ ao LCE em tempo O(1).\n"

using namespace std;

class KdifferenceInexactMatch;

string lerArquivo(const char *local);

class Parametro{

public:
   string alpha, beta;
   int k;
   bool mostrarMatriz;
   bool mostrarTempo;
   int versao;

   int total;

   Parametro(){
     k = 0;
     mostrarMatriz = false;
     mostrarTempo = false;
     versao = 1;
     total = 0;
   };
};

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
   void escreverTela() const;
   string escreverArquivoCompleto();
   string escreverArquivoReduzido();
};

classs KdifferenceInexactMatch{
  protected:
    char *a;  //padrão
    char *t;  //texto
    int m;  //tamanho de a
    int n;  //tamanho de t
    int k;  //quantidade de diferênças

public:
    KdifferenceInexactMatch(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);
    }
    ~KdifferenceInexactMatch(){};
    virtual int executar(int m){};//executa o algoritmo kdifference Inexact Match e retorna a linha da ocorrência de prime
    virtual void imprimirMatrizTela() const{};//escrever a matriz na tela
};

class KdifferenceInexactMatch2: public KdifferenceInexactMatch{

  private:
    int *L;   //Matriz L[-(k+1)..d]

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
    KdifferenceInexactMatch2(char *a, char *t, int *k): KdifferenceInexactMatch(a, t, k){
       //alocação dinâmica de inteiros
       this->L = new int[getDimensaoD()];
    }

    ~KdifferenceInexactMatch2(){
       //Ao destruir a classe desaloca toda memória que foi usada
       delete [] L;
    }
    virtual int executar(int m){};
    void imprimirMatrizTela();
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
      bool tempo; //booleano para mostrar o tempo de execução na tela se o usuário setar o parâmetro

      KdifferenceInexactMatch *c;

      list<Primer *> primers; //lista que guarda as ocorrências de primers

      KdifferenceInexactMatch2 *c;

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usuário não escolher nada
         this->mostrarMatriz=false;
         this->versao=1; //apenas uma versão
         primers.clear();
      }
      ~KdifferencePrime(){};
      virtual void instanciar(){};
      void processar();
      void mostrarOcorrencias();
      void setaParametros(Parametro *p);
};

//função auxiliar recebe os parâmetros que o usuário digitou, valida e transforma os valores
Parametro *parseParametros(int argc, char** argv){
   string argA[6] = {"-a", "-alpha", "-pattern", "-padrao", "-p", "-P"};
   string argB[6] = {"-b", "-beta", "-text", "-texto", "-t", "-T"};
   string argK[2] = {"-k", "-K"};

   Parametro *p = new Parametro();

   int temA, temB, temK;
   temA = temB = temK = 0;

   for(int z = 1; z < argc; z++){
      if(strcmp(argv[z], "-sm") == 0){
        p->mostrarMatriz = true;
        continue;
      }

      //versão vs1 = Matriz Completa, vs2 = Melhorada 1 e vs3 Molhorada 2
      if(strcmp(argv[z], "-vs1") == 0){
        //if(argc == 7) return 0;
        p->versao=1;
        continue;
      }
      if(strcmp(argv[z], "-vs2") == 0){
        //if(argc == 7) return 0;
        p->versao=2;
        continue;
      }

      if(strcmp(argv[z], "-vs3") == 0){
        //if(argc == 7) return 0;
        p->versao=3;
        continue;
      }

      if(strcmp(argv[z], "-st") == 0){
        //if(argc == 7) return 0;
        p->mostrarTempo=true;
        continue;
      }

      for(int w = 0; w < 6; w++){
        if(!temA && argA[w].compare(argv[z]) == 0){
            p->alpha = argv[z + 1];
            temA = 1;
            continue;
        }
        else if(!temB && argB[w].compare(argv[z]) == 0){
            p->beta = argv[z + 1];
            temB = 1;
            continue;
        }
        else if(w < 2 && !temK && argK[w].compare(argv[z]) == 0){
            try{
              p->k = atoi(argv[z + 1]);
              if(p->k > 0) temK = 1;
              continue;
            }catch(int e){
             // return 0;
            }
        }
      }
   }
   p->total = temA + temB + temK;

   p->alpha = lerArquivo(p->alpha.c_str());
   p->beta  = lerArquivo(p->beta.c_str());

   return p;
}

//função auxiliar compara três inteiros e devolve o maior
unsigned long long int menorDeTres(unsigned long long int x,
                                   unsigned long long int b,
                                   unsigned long long int c){
  if((x <= b) && (x <= c)) return x;
  else if((b <= x) && (b <= c)) return b;
  else return c;
}

long long int menorDeDois(long long int a,
                          long long int b){
   return a <= b ? a : b;
}

//não pode ser unsigned pois compara valores -1
//retorna o menor entre três valores
long long int maiorDeTres(long long int a,
                          long long int b,
                          long long int c){
  if(a >= b && a >= c) return a;
  else if(b >= a && b >= c) return b;
  else return c;
}

//LCE entre s[i] e t[j]
int directCompLCE(int _i,int _j,
                                    char *a, char *t, int _m, int _n){
    //deve ser passsado os índices reais e aqui será decido
    if(_i == 0 || _j == 0) return 0;
    _i--;
    _j--; //apenas por conta das strings
    int o = 0;
    //while(o+_i < _m && o+_j < _n && a->compare(o +_i, 1, *t, o +_j, 1) == 0)
    while(o+_i < _m && o+_j < _n && a[o +_i] == t[o +_j])
      o++;

    return o;
}

//devemos abrir o arquivo em tempo de execução, ou seja, processar enquanto abre o arquivo,
 string lerArquivo(const char *local){

  ifstream arquivo(local, std::ifstream::in);

  if(!arquivo.good()) {
    arquivo.close();

    return local;
  }
  cout<<"Abrindo arquivo "<<local<<"...\n";

  std::ostringstream contents;
  contents << arquivo.rdbuf();

  arquivo.close();

  return(contents.str());
}

void formataTempo(long long int valor){
  int mili = 0, sec = 0, minu = 0, dia = 0, hor = 0;
  long long int _tempo = valor;
  if(valor > 1000){//segundos
      mili = valor % 1000;
      valor = (valor - mili)/1000;
  } else{
    mili = valor;
    valor = 0;
  }

  if(valor > 60){//minutos
   sec = valor % 60;
   valor = (valor - sec)/60;
  } else{
    sec = valor;
    valor = 0;
  }

  if(valor > 60){//horas
   minu = valor % 60;
   valor = (valor - minu)/60;
  } else{
    minu = valor;
    valor = 0;
  }

  if(valor > 24){//dias
    hor = valor % 24;
    valor = (valor - hor)/24;
  } else{
    hor = valor;
    valor = 0;
  }
  if(valor > 0) dia = valor;

  string tempo = "(" + to_string(_tempo) + " milisegundo(s)) - ";
  if(dia > 0)  tempo += to_string(dia)  + " dia(s) ";
  if(hor > 0)  tempo += to_string(hor) + " hora(s) ";
  if(minu > 0) tempo += to_string(minu) + " minuto(s) ";
  if(sec > 0)  tempo += to_string(sec)  + " segundo(s) ";
  if(mili > 0) tempo += to_string(mili) + " milesimos(s)";

  cout<<"Tempo de execucao: "<< tempo <<endl;
}

void KdifferencePrime::mostrarOcorrencias(){
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
        }else{
            for(Primer *p : primers){
                p->escreverTela();
            }
        }
    }
}

void KdifferencePrime::setaParametros(Parametro *p){
    alpha = new char [p->alpha.length() + 1];
    strcpy(alpha, p->alpha.c_str());
    beta = new char [p->beta.length() + 1];
    strcpy(beta, p->beta.c_str());
    k = p->k;
    mostrarMatriz = p->mostrarMatriz;
    versao = p->versao;

    m = p->alpha.size();
    n = p->beta.size();
    ocorrencia = p->alpha;

    tempo = p->mostrarTempo;

    delete p;
}

//método que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: não há execução sem a invocação deste método
void KdifferencePrime::processar(){
   int ocr = 0; //flag que guarda a quantidade de ocorrências
   int r;

   instanciar(); //essa chamada depende diretamente da implementação do kdifferenceInexactMatch que será utilizado

   for(j = 0; j < (m - k) + 1; j++){
       r = c->executar(m - j);

       if(r > -1){ //se a linha foi obtida, significa que existe uma ocorrência
           Primer *pr = new Primer(++ocr, j, r, ocorrencia.substr(j, r));
           primers.insert(primers.end(), pr);
           if(mostrarMatriz) c->imprimirMatrizTela();
       }
   }
}

void Primer::escreverTela() const{
  cout<<setfill('0');
  cout<<MSG_N_OCCR<<setw(3)<<ocr<<" em ";

  cout<< setfill(' ');
  cout<<setw(3)<<j<<".."<<setw(3)<<j + r;
  cout<<"-Tamanho("<<setw(3)<<sequencia.size()<<"): ";
  cout<<sequencia<<endl;
}

string Primer::escreverArquivoCompleto(){
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

string Primer::escreverArquivoReduzido(){
    string *retorno = new string(to_string(j));
    retorno->append(";");
    retorno->append(to_string(j + r));
    retorno->append("\n");
    return *retorno;
}

//método que imprimi a matriz na tela
void KdifferenceInexactMatch2::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espaçamento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -(k+1); d <= (n+1); d++){
        vr = getL(d);
        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}

#endif
