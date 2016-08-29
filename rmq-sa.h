// #includes {{{
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>

using namespace std;
// }}}
// pre-written code {{{
#define REP(i,n) for(int i=0;i<(int)(n);++i)

// A utility function to get minimum of two numbers
static int minVal(int x, int y) { return (x < y)? x: y; }

// A utility function to get minimum of two numbers
static int maxVal(int x, int y) { return (x > y)? x: y; }

//{{{ Suffix Array
//{{{ for DEBUG
void writeSuffix(char *t,int i){
	cout<<(t+i)<<"$"<<endl;
}
//}}}
class SuffixArray{

 protected:
	char *t;
	int n,N;
	int *sa,*rmq, *b,*lcp;

 public:
	SuffixArray(char *t):t(t){
		n=strlen(t);N=n+1;
	}
	~SuffixArray(){
		if(sa)delete(sa);
		if(b)delete(b);
		if(lcp)delete(lcp);
		if(rmq)delete(rmq);
	}
	void setT(char *t){
	  this->t = t;
	}
	void builds(){
		buildSA();
		buildLCP();
		buildRMQ();
	}
	//{{{ Larsson-Sadakane's Suffix array Construction: O(n (log n)^2)
	struct SAComp {
		const int h, *g;
		SAComp(int h, int* g) : h(h), g(g) {}
		bool operator() (int a, int b) {
			return a == b ? false : g[a] != g[b] ? g[a] < g[b] : g[a+h] < g[b+h];
		}
	};
	void buildSA() {
		sa=new int[n+1];b=new int[n+1];
		int g[n+1];
		REP(i,n+1) sa[i] = i, g[i] = t[i];
		b[0] = 0; b[n] = 0;

		sort(sa, sa+n+1, SAComp(0, g));
		for(int h = 1; b[n] != n ; h *= 2) {
			SAComp comp(h, g);
			sort(sa, sa+n+1, comp);
			REP(i, n) b[i+1] = b[i] + comp(sa[i], sa[i+1]);
			REP(i, n+1) g[sa[i]] = b[i];
		}
		REP(i, n+1) b[sa[i]] = i;
	}
	//}}}
	//{{{ Naive matching O(m log n)
	int find(char *t, int n, char *p, int m, int *sa) {
		int a = 0, b = n;
		while (a < b) {
			int c = (a + b) / 2;
			if (strncmp(t+sa[c], p, m) < 0) a = c+1; else b = c;
		}
		return strncmp(t+sa[a], p, m) == 0 ? sa[a] : -1;
	}
	//}}}
	//{{{ Kasai-Lee-Arimura-Arikawa-Park's simple LCP computation: O(n)
	void buildLCP() {
		lcp=new int[n+1];
		int h = 0;
		REP(i, n+1) {
			if (b[i]){
				for (int j = sa[b[i]-1]; j+h<n && i+h<n && t[j+h] == t[i+h]; ++h);
				lcp[b[i]] = h;
			} else lcp[b[i]] = -1;
			if (h > 0) --h;
		}
	}
	//}}}
	//{{{ call RMQ = buildRMQ(lcp, n+1)
	void buildRMQ() {
		int logn = 1;
		for (int k = 1; k < N; k *= 2) ++logn;
		rmq=new int[N*logn];
		int *b = rmq; copy(lcp, lcp+N, b);
		for (int k = 1; k < N; k *= 2) {
			copy(b, b+N, b+N); b += N;
			REP(i, N-k) b[i] = min(b[i], b[i+k]);
		}
	}

   //compute LCE via RMQ
   //implementação feita por
	int LCEviaRMQ(int i, int j){
	    //b[i] inverso de sa[i]

        int low  = b[i];
        int high = b[j];

        i = minVal(low, high);
        j = maxVal(low, high);

        return minimum(i,j);
	}

	//compute LCE via directMin
   //implementação feita por Jean usando como base o artigo:
   //Tempo de execução: O(j - i) em que a diferença entre j e i pode ser n, ou seja, o tamanho da sequencia
   //logo, O(n)
   int LCEdirectMin(int i, int j){
      int low  = b[i];
      int high = b[j];

      i = minVal(low, high);
      j = maxVal(low, high);

      //low + 1 na versão original, mas aki o índice 1 é representado por 0
      int t = lcp[i+1]; //por causa do índice inicial
      int s;

      for(int k = i + 2; k <= j; k++){
         s = lcp[k]; //mesma coisa aki, índice começa em zero
         if(s < t) t = s;
      }
      return t;
   }
	//}}}
	//{{{ inner LCP computation with RMQ: O(1)
	int minimum(int x, int y) {
		x++;
		int z = y - x, k = 0, e = 1, s; // y - x >= e = 2^k なる最大 k
		s = ( (z & 0xffff0000) != 0 ) << 4; z >>= s; e <<= s; k |= s;
		s = ( (z & 0x0000ff00) != 0 ) << 3; z >>= s; e <<= s; k |= s;
		s = ( (z & 0x000000f0) != 0 ) << 2; z >>= s; e <<= s; k |= s;
		s = ( (z & 0x0000000c) != 0 ) << 1; z >>= s; e <<= s; k |= s;
		s = ( (z & 0x00000002) != 0 ) << 0; z >>= s; e <<= s; k |= s;
		return min( rmq[x+N*k], rmq[y+N*k-e+1] );
	}
	//}}}
	//{{{ outer LCP computation: O(m - o)
	static int computeLCP(char *t, int n, char *p, int m, int o, int k) {
		int i = o;
		for (; i < m && k+i < n && p[i] == t[k+i]; ++i);
		return i;
	}
	//}}}
	//{{{ Mamber-Myers's O(m + log n) string matching with LCP/RMQ
#define COMP(h, k) (h == m || (k+h<n && p[h]<t[k+h]))
	int find(char *p, int m) {
		int l = 0, lh = 0, r = n, rh = computeLCP(t,N,p,m,0,sa[n]);
		if (!COMP(rh, sa[r])) return -1;
		for (int k = (l+r)/2; l+1 < r; k = (l+r)/2) {
			int A = minimum(l, k), B = minimum(k, r);
			if (A >= B) {
				if (lh < A) l = k;
				else if (lh > A) r = k, rh = A;
				else {
					int i = computeLCP(t, N, p, m, A, sa[k]);
					if (COMP(i, sa[k])) r = k, rh = i; else l = k, lh = i;
				}
			} else {
				if (rh < B) r = k;
				else if (rh > B) l = k, lh = B;
				else {
					int i = computeLCP(t, N, p, m, B, sa[k]);
					if (COMP(i, sa[k])) r = k, rh = i; else l = k, lh = i;
				}
			}
		}
		return rh == m ? sa[r] : -1;
	}


};
/*
int main(){

  SuffixArray *sa = new SuffixArray("CCCGGCCC#CCCGTGCCC");
                                     //012345678901234567
  sa->builds();
   //string txt = "CCCGGCCC#CCCGTGCCC";

  cout<<sa->LCE(0,9)<<endl;
  cout<<sa->LCE(1,9)<<endl;
  cout<<sa->LCE(4,14)<<endl;
  cout<<sa->LCE(4,13)<<endl;
  cout<<sa->LCE(3,13)<<endl;

   return 0;
}; */
//}}}
