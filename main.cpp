#include "SuffixTree.hpp"
#include <string>
#include <iostream>
using namespace std;

int main(int argc,char** argv){
    vector<string> seqs;
    for (int i=1; i<argc; i++)
        seqs.push_back(argv[i]);
    GeneralizedSuffixTree* stree = new GeneralizedSuffixTree(seqs);
    cout << stree->newick() << endl;
    return 0;
}
