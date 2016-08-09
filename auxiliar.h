
#ifndef _auxiliar_H_
#define _auxiliar_H_

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>

//constantes definidas para uso de mensagens na tela
#define ERR_ARGS "Uso correto:\n -a -alpha -pattern -padrao -p -P \t(String) \n -b -beta -text -texto -t -T \t\t(String) \n -k -K \t\t\t\t\t(int) maior que 0 e menor que m\n [-sm] \tmostrar a matriz\n [-vs%] \tversao do algoritmo (1,2 ou 3)\n [-st] \tmostrar o tempo de execucao"
#define ERR_KMAIOR "O parametro k deve estar em um intervalo entre 1 e "

using namespace std;

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

  std::stringstream out;
  out << _tempo;

  string tempo = "(" + out.str() + " milisegundo(s)) - ";
  out << dia;
  if(dia > 0)  tempo += out.str()  + " dia(s) ";
  out << hor;
  if(hor > 0)  tempo += out.str() + " hora(s) ";
  out << minu;
  if(minu > 0) tempo += out.str() + " minuto(s) ";
  out << sec;
  if(sec > 0)  tempo += out.str()  + " segundo(s) ";
  out << mili;
  if(mili > 0) tempo += out.str() + " milesimos(s)";

  cout<<"Tempo de execucao: "<< tempo <<endl;
}

#endif
