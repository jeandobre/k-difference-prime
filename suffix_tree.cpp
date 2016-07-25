#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include "auxiliar.h"

using namespace std;

typedef unsigned char byte;

/*

 * SuffixNode Declaration

 */

class SuffixNode
{

public:

    int depth = -1, begin = -1, end = -1;

    SuffixNode **children;

    SuffixNode *parent, *suffixLink;

    /*

     * Constructor

     */

    SuffixNode(int begin, int end, int depth, SuffixNode *parent)
    {

        children = new SuffixNode* [6];
        this->begin = begin;
        this->end = end;
        this->parent = parent;
        this->depth = depth;
    }


    bool contains(int d)
    {
        return depth <= d && d < depth + (end - begin);
    }

    void print()
    {
        cout<<"Inicio: "<<this->begin;
        cout<<", Fim: "<<this->end;
        cout<<", Profundidade: "<<this->depth<<endl;
    }

};

string alphabet;
int alphabetSize;
int lcsLength;
int lcsBeginIndex;
int lca;


/*

 * Class SuffixTree Declaration

 */

class SuffixTree
{

public:

    /*

     * Funtion to build suffix tree for given text

     */

    SuffixNode *buildSuffixTree(string s)
    {

        int n = s.length();

        int *a = new int[n];

        for (int i = 0; i < n; i++)
        {
            a[i] = alphabet.find(s.at(i));
        }

        SuffixNode *root = new SuffixNode(0, 0, 0, NULL);

        SuffixNode *cn = root;

        root->suffixLink = root;

        SuffixNode *needsSuffixLink = NULL;

        int lastRule = 0;

        int j = 0;

        for (int i = -1; i < n - 1; i++)
        {
            int cur = a[i + 1];
            for (; j <= i + 1; j++)
            {
                int curDepth = i + 1 - j;
                if (lastRule != 3)
                {
                    if (cn->suffixLink != NULL)

                        cn = cn->suffixLink;

                    else

                        cn = cn->parent->suffixLink;

                    int k = j + cn->depth;

                    while (curDepth > 0 && !cn->contains(curDepth - 1))

                    {

                        k += cn->end - cn->begin;

                        cn = cn->children[a[k]];

                    }

                }

                if (!cn->contains(curDepth))

                {

                    if (needsSuffixLink != NULL)

                    {

                        needsSuffixLink->suffixLink = cn;

                        needsSuffixLink = NULL;

                    }

                    if (cn->children[cur] == NULL)

                    {

                        cn->children[cur] = new SuffixNode(i + 1, n, curDepth, cn);

                        lastRule = 2;

                    }

                    else

                    {

                        cn = cn->children[cur];

                        lastRule = 3;

                        break;

                    }

                }

                else
                {

                    int end = cn->begin + curDepth - cn->depth;

                    if (a[end] != cur)
                    {

                        SuffixNode *newn = new SuffixNode(cn->begin, end, cn->depth, cn->parent);

                        newn->children[cur] = new SuffixNode(i + 1, n, curDepth, newn);

                        newn->children[a[end]] = cn;

                        cn->parent->children[a[cn->begin]] = newn;

                        if (needsSuffixLink != NULL)

                            needsSuffixLink->suffixLink = newn;

                        cn->begin = end;

                        cn->depth = curDepth;

                        cn->parent = newn;

                        cn = needsSuffixLink = newn;

                        lastRule = 2;

                    }

                    else if (cn->end != n || (cn->begin - cn->depth) < j)

                    {

                        lastRule = 3;

                        break;

                    }

                    else

                        lastRule = 1;

                }

            }

        }

        root->suffixLink = NULL;

        return root;

    }

    /*

     * Funtion to find longest common substring

     */

    int findLCS(SuffixNode *node, int i1, int i2)
    {

        if (node->begin <= i1 && i1 < node->end) return 1;

        if (node->begin <= i2 && i2 < node->end) return 2;

        int mask = 0;

        for (int f = 0; f < alphabetSize; f++)
        {
            if (node->children[f] != NULL)
            {
                mask |= findLCS(node->children[f], i1, i2);
            }
        }

        if (mask == 3)
        {

            int curLength = node->depth + node->end - node->begin;
            if (lcsLength < curLength)
            {
                lcsLength = curLength;
                lcsBeginIndex = node->begin;
            }
        }

        return mask;
    }



    /*

     * Funtion to find longest common substring

     */

    void findLCS(string s1, string s2)
    {

        string x = "#";

        string y = "$";

        string ns1 = s1;

        string ns2 = s2;

        string s = s1.append(x.append(s2.append(y)));

        SuffixNode *root = buildSuffixTree(s);

        lcsLength = 0;

        lcsBeginIndex = 0;

        findLCS(root, ns1.length(), ns1.length() + ns2.length() + 1);

        bool chk = lcsLength > 0;

        if (chk)
        {
            cout<<"\nLongest substring is "<<s.substr(lcsBeginIndex , lcsLength);
            cout<<endl;

        }
        else
        {
            cout<<"No substring found"<<endl;
        }

    }

    int findLCA(SuffixNode *node, int i1, int i2)
    {


        node->print();

        if (node->begin <= i1 && i1 < node->end) return 1;

        if (node->begin <= i2 && i2 < node->end) return 2;

        int mask = 0;

        for (int f = 0; f < alphabetSize; f++)
        {
            if (node->children[f] != NULL)
            {
                mask |= findLCA(node->children[f], i1, i2);
            }
        }

        if (mask == 3)
        {

            lca = node->depth;
        }

        return mask;

    }

    void findLCA(string s1, string s2)
    {
        string x = "#";

        string y = "$";

        string ns1 = s1;

        string ns2 = s2;

        string s = s1.append(x.append(s2.append(y)));
        lca = -1;
        cout<<s<<endl;

        SuffixNode *root = buildSuffixTree(s);
        //findLCA(root, 1, 6);
        print(root, "root");

        if(lca > -1) cout<<lca;

    }

    void print(SuffixNode *node, string a)
    {
        cout<<a<<"\t";
        node->print();
        for (int f = 0; f < alphabetSize; f++)
        {
            if (node->children[f] != NULL)
            {
              //  cout<<node->children[f]<<endl;
                print(node->children[f], alphabet.substr(f,1));
            }
        }

    }
};

int main(int argc, char** argv)
{
/*
    alphabet = "ACGT#$";
    alphabetSize = alphabet.length();

    string s1,s2;
    Parametro *p = parseParametros(argc, argv);

    s1 = p->alpha;
    s2 = p->beta;

    SuffixTree st;
    st.findLCA(s1, s2);
    //st.findLCS(s1, s2);


*/

  //teste de comparacao entre string e char

    const char *s1, *s2;
    Parametro *p = parseParametros(argc, argv);

    int m = p->alpha.size();
    int n = p->beta.size();
    int v = 0;

    time_t inicio, fim;
    const char *a;
    const char *t;

    a = p->alpha.c_str();
    t = p->beta.c_str();

   inicio = clock();
   for(int i = 0; i < m; i++)
       for(int j = 0; j < n; j++){
            if(a[i] == t[j]) v++;
            // if(p->alpha.at(i) == p->beta.at(j)) v++;
            //if(p->alpha.compare(i,1, "A") == 0) v++;
            //if(p->alpha.compare(i,1, p->beta, j, 1) == 0) v++;
       }

   fim = clock();
   cout<<v<<endl;
   cout<<"Tempo de execucao: "<< ((fim - inicio) / (CLOCKS_PER_SEC / 1000)) << " milisegundos"<<endl;

    return 0;

}
