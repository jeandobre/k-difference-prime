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
#include <new>

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
class KdifferencePrime;

string lerArquivo(const char *local);


long long int menorDeDois(long long int a,
                          long long int b){
   return a < b ? a : b;
}
static long long int maiorDeDois(long long int a,
                          long long int b){
   return a > b ? a : b;
}

//função auxiliar compara três inteiros e devolve o menor
static long long int menorDeTres(unsigned long long int x,
                                   unsigned long long int b,
                                   unsigned long long int c){

  return menorDeDois(menorDeDois(x,b),c);
}

//não pode ser unsigned pois compara valores -1
//retorna o maior entre três valores
static long long int maiorDeTres(long long int a,
                          long long int b,
                          long long int c){

  return maiorDeDois(maiorDeDois(a,b), c);
}

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
      this->j = j;
      this->r = r;
      this->sequencia = sequencia;
   };
   void escreverTela() const;
   string escreverArquivoCompleto();
   string escreverArquivoReduzido();
};

bool comparar(const Primer* a, const Primer* b){
   return a->j < b->j;
}

class KdifferenceInexactMatch{
  protected:
    char *a;  //padrão
    char *t;  //texto
    int m;  //tamanho de a
    int n;  //tamanho de t
    int k;  //quantidade de diferênças

    int rowPrint;//linha máxima de impressao da matriz

  public:
    int primerM; //tamanho original de alfa, extremamente necessário p/ computar corretamente o LCE
    int primerJ; //índice atual j de alfa

public:
    KdifferenceInexactMatch(char *a, char *t, int *k){
       this->a = a;
       this->t = t;
       this->k = *k;

       this->m = strlen(a);
       this->n = strlen(t);

       primerJ = 0;
       primerM = m;
    }
    ~KdifferenceInexactMatch(){};
    virtual string name() const {return "K";};
    virtual int executar(int m){return -1;};//executa o algoritmo kdifference Inexact Match e retorna a linha da ocorrência de prime
    //virtual int executar(int m, int j){return -1;};
    virtual void imprimirMatrizTela(){};//escrever a matriz na tela
};

class KdifferenceInexactMatch234: public KdifferenceInexactMatch{

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
    KdifferenceInexactMatch234(char *a, char *t, int *k): KdifferenceInexactMatch(a, t, k){
       //alocação dinâmica de inteiros
       try{
         this->L = new int[getDimensaoD()];

       }catch(bad_alloc& ba){
          cout<<"Erro ao alocar memória: " << ba.what()<<endl;
       }
    }

    ~KdifferenceInexactMatch234(){
       //Ao destruir a classe desaloca toda memória que foi usada
       delete [] L;
    }

    int executar(int m){
       this->m = m;
       int d,e, row;

       //inicialização da matriz
       for(d = -(k+1); d <= (n+1); d++)
           setL(d, -1);

       int long long pivo; //variável auxiliar para troca de posições
       bool passou = true; //flag para controlar o caso de alcançar o fim de m antes de k diferenças
       int linha = -1;     //variável que guarda sempre o maior valor da coluna e
       for(e = 0; e < k && passou; e++){
           pivo = linha = -1;  //a cada nova coluna a variável linha é reiniciada
           for(d = -e; d <= n && passou; d++){
              row = maiorDeTres(getL(d-1), getL(d) + 1, getL(d+1) + 1);
              row = menorDeDois(row, m);
              if(row + d < n)
                row += LCE(primerJ + row, primerM + 1 + row + d); //LCE
              //se já alcancou 'm' e o erro é menor que 'k' pode parar e ir para o próximo 'j'
              if(row == m){passou = false; continue;}
              setL(d-1, pivo); //atualiza a coluna e guardando o pivo no espaço anterior
              if (row > linha) linha = row;
              pivo = row;
           }
           setL(n, row);
       }
       return (passou ? ++linha : -1); //retorna a linha de ocorrência de primer ou -1 que indica não ocorrência
   }
   void imprimirMatrizTela();

   virtual long int LCE(int x, int y){
      return 0;
   }
};

class KdifferencePrime{

    public:
      char *alpha, *beta;//seguindo o modelo encontrado no livro do GUSFIELD, nomes de variáveis que representam o padrão e o texto respectivamente
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

      int *ocr; //vetor que guarda as ocorrências

    public:
      KdifferencePrime(){
         //construtor seta os valores default se o usuário não escolher nada
         this->mostrarMatriz=false;
         this->versao=1; //apenas uma versão
         primers.clear();
      }
      ~KdifferencePrime(){};
      virtual void instanciar(){};
      virtual void processar();
      void mostrarOcorrencias();
      void mostrarOcorrencias(bool);
      void setaParametros(Parametro *p);
    private:
      void mostrar(int j, int r){
        cout<<setfill(' ');
        cout<<setw(3)<<j<<".."<<setw(3)<<j + r;
        string o = ocorrencia.substr(j, r);
        cout<<"-Tamanho("<<setw(3)<<o.size()<<"): ";
        cout<<o<<endl;
      };
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

//LCE entre s[i] e t[j]
static int directCompLCE(int _i,int _j, char *a, char *t, int _m, int _n){
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

void formataTempo(time_t hora, bool inicio){
   struct tm *timeinfo;

   timeinfo = localtime(&hora);
   char buffer[80];
   if(inicio) strftime(buffer, 80, "Inicio: %d/%m/%y %X", timeinfo);
   else       strftime(buffer, 80, "Fim   : %d/%m/%y %X", timeinfo);
   cout<<buffer<<endl;
}

void formataSegundos(double segundos){
  long int valor = (int) segundos;
  int sec = 0, minu = 0, dia = 0, hor = 0;
  double _tempo = valor;

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

  string tempo = "";
  if(dia > 0)  tempo += to_string(dia)  + " dia(s) ";
  if(hor > 0)  tempo += to_string(hor) + " hora(s) ";
  if(minu > 0) tempo += to_string(minu) + " minuto(s) ";
  if(sec > 0)  tempo += to_string(sec)  + " segundo(s) ";

  if(minu > 0){
     tempo += "(" + to_string(segundos) + " segundos )";
  }

  cout<<"Tempo de execucao: "<< tempo  << endl;
}

void KdifferencePrime::mostrarOcorrencias(bool formaSimples){
  int r;
  for(int v = 0; v < m-k+1; v++){
     r = ocr[v];
     if(r > 0){
         cout<<v<<";"<<r<<"-"<<ocorrencia.substr(v, r)<<endl;
      }
  }
}

void KdifferencePrime::mostrarOcorrencias(){
  int nOcr = 0; int r;
  for(int v = 0; v < m-k+1; v++){
     r = ocr[v];
     if(r > 0){
         Primer *pr = new Primer(++nOcr, v, r, ocorrencia.substr(v, r));
         primers.insert(primers.end(), pr);
      }
  }

  if(primers.size() == 0) cout<<MSG_0_OCCR<<k<<" diferenca(s)"<<endl;
  else{
        cout<<"Encontrado "<<primers.size()<<" ocorrencia(s) de primers ";
        if(primers.size() > 10){
            fstream out;
            string fileName = "saida/a"
                           + to_string(m) + "_b"
                           + to_string(n) + "_k"
                           + to_string(k) + "_"
                           + c->name();
            out.open(fileName, ios::out | ios::trunc);
            for(Primer *p : primers){
                out<<p->escreverArquivoReduzido();
                //out<<p->escreverArquivoCompleto();
            }
            out.close();
            cout<<"(Arquivo: " + fileName + ")"<<endl;
        }else{
            primers.sort(comparar);
            for(Primer *p : primers){
                p->escreverTela();
            }
        }
    }
}

void KdifferencePrime::setaParametros(Parametro *p){

    try{
      alpha = new char [p->alpha.length() + 1];
      strcpy(alpha, p->alpha.c_str());
      beta = new char [p->beta.length() + 1];
      strcpy(beta, p->beta.c_str());
    } catch(bad_alloc& ba){
      cout<<"Erro ao alocar memória: " << ba.what()<<endl;
    }
    k = p->k;
    mostrarMatriz = p->mostrarMatriz;
    versao = p->versao;

    m = p->alpha.size();
    n = p->beta.size();
    ocorrencia = p->alpha;

    int maxOcr = abs(m-k+1); //calcula o total de ocorrências de primer possíveis
    ocr = new int[maxOcr];   //aloca o máximo espaço para todas as possíveis ocorrências de primer
    memset(ocr, -1, maxOcr); //seta todas as posições com valores -1 (não ocorrência)

    tempo = p->mostrarTempo;

    delete p;
}

//método que processa o algoritmo principal chamado a partir do procedimento MAIN
//IMPORTANTE: não há execução sem a invocação deste método
void KdifferencePrime::processar(){
   instanciar(); //essa chamada depende diretamente da implementação do kdifferenceInexactMatch que será utilizado
   //O(m), onde m é o tamanho de alpha
   c->primerM = m;
   for(j = 0; j < (m - k) + 1; j++){
       //guardo em um array todos as ocorrências r de primer para cada j
       c->primerJ = j;
       ocr[j] = c->executar(m - j);
       if(mostrarMatriz && ocr[j] != -1){
         mostrar(j, ocr[j]);
         c->imprimirMatrizTela();
       }
      // if(ocr[j] > m) break; //então já chegou  fim de alfa
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
void KdifferenceInexactMatch234::imprimirMatrizTela(){
    cout << setfill(' ');     //setar o tamanho de espaçamento de elementos na tela
	int vr;
    cout<<endl;
    for(int d = -(k+1); d <= (n+1); d++){
        vr = getL(d);
        cout<<setw(2)<<vr<<endl;
    }
    cout<<endl;
}



/*
   construído o main apenas para testar as funções
   int main(){
     formataSegundos(59590);
     return 0;
   }
*/

#endif
