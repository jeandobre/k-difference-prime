/*
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ci�ncia da Computa��o
   vers�o: 0.1
*/
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

#define M 100

class KdifferenceInexactMatch2{

  private:
    int L[M + 1][M + 1]; //melhorar depois, fazer alocamento din�mico
    string a;
    string t;
    int k;
    int m, n;

  private:
    int minimo(int, int);
    int maximo(int, int, int);

    void construirResultado(int, int);
    int getMinD(){
      return -(k + 1);
    }
    int getMaxD(){
      return n + 1;
    }
    int dToMatriz(int d){ //converte indice d para indice da matriz L
      return d + (k + 1);
    }

    int getMinE(){
      return -1;
    }

    int getMaxE(){
      return k;
    }

    int eToMatriz(int e){
      return e - 1;
    }

    void setL(int d, int e, int valor){
      L[dToMatriz(d)][eToMatriz(e)] = valor;
    }

    int getL(int d, int e){
      return L[dToMatriz(d)][eToMatriz(e)];
    }

  public:
    KdifferenceInexactMatch2(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();
    }
    int executar();
    void escreverMatrizTela();
    void acharResultado();
    //int getLinhaMenorK();
};

int KdifferenceInexactMatch2::minimo(int a, int b){
   return a <= b ? a : b;
}

int KdifferenceInexactMatch2::maximo(int a, int b, int c){
   if(a >= b && a >= c) return a;
   else if(b >= a && b >= c) return b;
   else return c;
}

void KdifferenceInexactMatch2::escreverMatrizTela(){

	cout<<"\n\n    ";

    for(int i = getMinD(); i <= getMaxD(); i++) cout<<dToMatriz(i)<< "  ";
    cout<<"\n    ";

	for(int i = getMinD(); i <= getMaxD(); i++) cout<<i<< "  ";
	cout<<"\n";

	int vr;

	for(int l = getMinE(); l <= getMaxE(); l++){
            //if(i == 0) cout<<"  ";
        cout<<l<<"  ";

        for(int i = getMinD(); i <= getMaxD(); i++){
            vr = getL(i,l);

            if (vr > 100 || vr < -1) cout<<"X  ";
			else cout<<vr<<"  ";
		}
		cout<<"\n";
	}
}

void KdifferenceInexactMatch2::construirResultado(int coluna, int k_linha){

		//reconstruir
		/*int i = m;
		int l = coluna;
		string a_linha;
	    string t_linha;
	    t_linha.insert(0, t, coluna, n);
		int e = k_linha;

		while(i > 0 && l > 0){
			bool igual = a.compare(i-1, 1, t, l-1, 1) == 0 ? true: false;

			if(D[i-1][l-1] == e){
			  if(igual){

				a_linha.insert(0, a, i-1, 1);
				t_linha.insert(0, t, l-1, 1);
				i--;
				l--;
			  } else{  // e-1
				if(D[i][l-1] == e-1){ //d-1
					a_linha.insert(0, " ");
					t_linha.insert(0, t, l-1, 1);
					l--;
				}else{ // d + 1
					a_linha.insert(0, a, i-1, 1);
					t_linha.insert(0, " ");
					i--;
				}
				e--;
			  }
			} else { // e-1
				if(D[i-1][l-1] == e-1) { //d
				  a_linha.insert(0, a, i-1, 1);
				  t_linha.insert(0, t, l-1, 1);
				  i--;
				  l--;
				}else if(D[i][l-1] == e-1){ // d -1
					a_linha.insert(0, " ");
					t_linha.insert(0, t, l-1 , 1);
					l--;
				}else if(D[i-1][l] == e-1){
					a_linha.insert(0, a, i-1, 1);
					t_linha.insert(0, " ");
					i--;
				}
				e--;
			}
		}
		//copia o sufixo
		if(l > 0){
		   t_linha.insert(0, t, 0, l);	//adicionar o prefixo de t.
		   a_linha.insert(0, l, ' '); //adicionar espa�o ao inicio n-l vezes
		}

		cout<<"\n\nt: " << t_linha;
		cout<<"\na: "<<a_linha;
*/
}

void KdifferenceInexactMatch2::acharResultado(){
	/*bool existe = false;

	//varre a linha m para encontrar um i <= k;
	int k_linha = k;
	int coluna = 0;
	for(int i = k; i <= n; i++){
		if(D[m][i] <= k){ 	//menos que k de tamanho n�o interessa
			existe = true;
			coluna = i;
			k_linha = D[m][i];
			construirResultado(coluna, k_linha);
		}
	}

	if(!existe){
		cout<<"\nNao foi encontrado um casamento com no maximo " <<
		k <<
		" diferencas, a menor diferenca foi " << k_linha;
	}*/
}

int KdifferenceInexactMatch2::executar(){
    int d,e, row;
/*
	for(d = -(k+1); d <= k+1; d++){
       setL(d, abs(d) - 2, abs(d) - 2);
       if(d < 0) setL(d, abs(d) - 1, abs(d) - 1);
       else      setL(d, abs(d) - 1, -1);
	}
*/
	for(d = 0; d <= n; d++)
        setL(d, - 1, -1);

    for(d = -(k+1); d <= -1; d++){
      setL(d, abs(d) - 2, abs(d) - 2);
      setL(d, abs(d) - 1, abs(d) - 1);
    }

    for(e = -1; e<= k; e++)
        setL(getMaxD(), e, -1);

    for(e = 0; e <= k; e++){
        for(d = -(e); d <= n; d++){
           row = maximo(getL(d-1, e-1),
                        getL(d,   e-1) + 1,
                        getL(d+1, e-1) + 1);
           while(row < m && row+d < n && a.compare(row+1, 1, t, row+d+1, 1) == 0)
             row++;
           row = minimo(row, m);
           setL(d,e, row);
           //if(d == k - 1 && row == m) return row; //algoritmo original
           //if(e == k) return row;
        }
    }
   return -1;
}

int main(int argc, char** argv) {
    string alpha, beta;
    int k;

    cout<<"**************************** K-DIFFERENCE PRIME 2 ******************************";

	cout<<"Entre com o padrao (a)....: ";
 	cin>>alpha;

 	cout<<"Entre com o texto (t).....: ";
 	cin>>beta;

 	cout<<"Entre qtd m�n. erros (k)..: ";
 	cin>>k;

	int m = alpha.size();
    int linha;
    int ocr = 0;

    KdifferenceInexactMatch2* c;

    for(int j = 0; j < (m - k) + 1; j++){ //aki o j est� varrendo todo o padr�o alpha
       c = new KdifferenceInexactMatch2(alpha.substr(j), beta, k); //melhorar aki, diminuir o uso de mem�ria
       linha = c->executar();
       c->escreverMatrizTela();
       if(linha > -1) //NO c++ a substr (indice, tamanho)
           cout<<"\n Ocorrencia "<< ++ocr <<" em: "<< j <<", "<<j + linha<<", "<<alpha.substr(j, linha);
   }
   if(ocr == 0)
      cout<<"\n N�o foi encontrado nenhuma ocorrencia com pelo menos "<<k<<" diferen�as";

	return 0;
}
