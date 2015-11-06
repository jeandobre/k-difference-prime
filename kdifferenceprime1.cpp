/*
   Autor: Jean Alexandre Dobre
   Orientador: Said Sadique Adi
   Ano: 2015
   FACOM: Mestrado em Ciência da Computação
   versão: 0.7
*/
#include <iostream>
#include <string>
using namespace std;

#define M 100

class KdifferenceInexactMatch1{

  private:
    int D[M + 1][M + 1]; //melhorar depois, fazer alocamento dinâmico
    string a;
    string t;
    int k;
    int m, n;

  private:
    int minimo(int, int);
    void construirResultado(int, int);

  public:
    KdifferenceInexactMatch1(string a, string t, int k){
       this->a = a;
       this->t = t;
       this->k = k;

       this->m = a.length();
       this->n = t.length();
    }
    void executar();
    void escreverMatrizTela();
    void acharResultado();
    int getLinhaMenorK();
};

int KdifferenceInexactMatch1::minimo(int i, int l){
	//l linha
	//c coluna

	int x = D[i-1][l] + 1;
	int b = D[i][l-1] + 1;

	int c = (a.compare(i-1, 1, t, l-1, 1) == 0) ? D[i-1][l-1] : D[i-1][l-1] + 1;
	//A < B < C then A

	if((x <= b) && (x <= c)) return x;
	else if((b <= x) && (b <= c)) return b;
	else return c;

}

void KdifferenceInexactMatch1::escreverMatrizTela(){

	cout<<"\n\n    ";

	for(int i = 0; i <= n; i++)
	   cout<<t[i] << " ";

	cout<<"\n";

	for(int i = 0; i <= m; i++){

		if(i == 0 ) cout<<"  ";
		else cout<<a[i - 1]<<" ";

		for(int l = 0; l <= n; l++){
			cout<<D[i][l] << " ";
		}
		cout<<"\n";
	}
}

void KdifferenceInexactMatch1::construirResultado(int coluna, int k_linha){

		//reconstruir
		int i = m;
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
		   a_linha.insert(0, l, ' '); //adicionar espaço ao inicio n-l vezes
		}

		cout<<"\n\nt: " << t_linha;
		cout<<"\na: "<<a_linha;

}

void KdifferenceInexactMatch1::acharResultado(){
	bool existe = false;

	//varre a linha m para encontrar um i <= k;
	int k_linha = k;
	int coluna = 0;
	for(int i = k; i <= n; i++){
		if(D[m][i] <= k){ 	//menos que k de tamanho não interessa
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
	}
}

void KdifferenceInexactMatch1::executar(){
   for(int l = 0; l <= n; l++)
	  D[0][l] = 0;

	for(int i = 1; i <= m; i++)
	   D[i][0] = i;


	for(int i = 1; i <= m; i++){
		for(int l = 1; l <= n; l++){
			D[i][l] = minimo(i, l);
		}
	}
}

int KdifferenceInexactMatch1::getLinhaMenorK(){
    int linha = -1;
    for(int z = m; z >= 0; z--){
        bool passou = true;
        for(int x = 0; x < n; x++){
           if(D[z][x] < k) passou = false;
        }
        if(passou) linha = z;
    }
    return linha;
}

int main(int argc, char** argv) {
    string alpha, beta;
    int k;

	 cout<<"**************************** K-DIFFERENCE PRIME 1 ******************************";

	cout<<"Entre com o padrao (a)....: ";
 	cin>>alpha;

 	cout<<"Entre com o texto (t).....: ";
 	cin>>beta;

 	cout<<"Entre qtd mín. erros (k)..: ";
 	cin>>k;

	int m = alpha.size();
	int n = beta.size();
    int linha;
    int ocr = 0;

    KdifferenceInexactMatch1* c;

   for(int j = 0; j < (m - k) + 1; j++){ //aki o j está varrendo todo o padrão alpha
       c = new KdifferenceInexactMatch1(alpha.substr(j), beta, k); //melhorar aki, diminuir o uso de memória
       c->executar();
       linha = c->getLinhaMenorK(); //retorna -1 se não encontrou

       if(linha > -1){
           cout<<"\n Ocorrencia "<< ++ocr <<" em: "<< j <<", "<<j + linha<<", "<<alpha.substr(j, linha);
           //c->escreverMatrizTela();
       }
   }
   if(ocr == 0)
      cout<<"\n Nao foi encontrado nenhuma ocorrencia com pelo menos "<<k<<" diferenças";

	return 0;
}
