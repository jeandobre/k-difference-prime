#include <fstream>
#include <sstream>
#include <string.h>
#include <iomanip>

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-vs%] \tversao do algoritmo (1,2 ou 3)\n [-st] \tmostrar o tempo de execucao"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 1 e "

using namespace std;

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
unsigned long long int directCompLCE(unsigned long long int _i,
                                    unsigned long long int _j,
                                    char *a, char *t, int _m, int _n){
    //deve ser passsado os índices reais e aqui será decido
    if(_i == 0 || _j == 0) return 0;
    _i--;
    _j--; //apenas por conta das strings
    unsigned int o = 0;
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
  if(hor > 0)  tempo += to_string(hor)  + " hora(s) ";
  if(minu > 0) tempo += to_string(minu) + " minuto(s) ";
  if(sec > 0)  tempo += to_string(sec)  + " segundo(s) ";
  if(mili > 0) tempo += to_string(mili) + " milesimos(s)";

  cout<<"Tempo de execucao: "<< tempo <<endl;
}
