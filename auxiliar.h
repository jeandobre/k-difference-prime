#include <fstream>
#include <sstream>
#include <string.h>
#include <iomanip>

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-vs%] \tversao do algoritmo (1,2 ou 3)\n [-st] \tmostrar o tempo de execucao"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 1 e "
#define MSG_0_OCCR "\nNao foi encontrado nenhuma ocorrencia com pelo menos "
#define MSG_N_OCCR "\nOcr "

using namespace std;

class Primer{

private:
   int ocr;
   int j;
   int r;
   string sequencia;

public:
   Primer(int ocr, int j, int r, string sequencia){
      this->ocr = ocr;
      this->j = ++j;
      this->r = r;
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
};

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
unsigned long long int LCEiterativo(unsigned long long int _i,
                                    unsigned long long int _j,
                                    string &a, string &t){
    //deve ser passsado os índices reais e aqui será decido
    if(_i == 0 || _j == 0) return 0;
    int _m = a.size();
    int _n = t.size();
    _i--;
    _j--; //apenas por conta das strings
    unsigned int o = 0;
    while(o+_i < _m && o+_j < _n && a.compare(o+_i, 1, t, o+_j, 1) == 0)
      o++;

    return o;
}

//TODO devemos abrir o arquivo em tempo de execução, ou seja, processar enquanto abre o arquivo,
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
        //if(argc == 7) return 0;
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
