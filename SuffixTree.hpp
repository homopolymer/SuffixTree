// Suffix Tree Class
// -----------------
// It provides an engine to construct and manipulate a 
// suffix tree for an input string using the Ukkonen's 
// construction algorithm.
//
//
// Example
// -------
// [1] Construct the suffix tree
// 
//     string s = "cacao$";
//     SuffixTree* stree = new SuffixTree(s);
//     
// [2] Visit the nodes following the post-order
//     
//     SuffixTree::postorder_iterator iter = stree->postorder_begin();
//     for (; iter<stree->postorder_end();++iter)
//         cout << "node_id: " << iter->id << "\t"
//              << "node_label: " << iter->path_label.substr(iter->parent->path_length,
//                                         iter->path_length-iter->parent->path_length)
//              << endl;
//
// [3] Visit the leaf nodes
//
//     SuffixTree::leaf_iterator iter = stree->leaf_begin();
//     for (; iter<stree->leaf_end();++iter)
//         cout << "path_label: " << iter->path_label << endl;                                        
//
// [4] Newick representation
//
//     string newick = stree->newick();
//     
// [5] Generalized suffix tree
//
//     vector<string> seqs = {"xabxa$","babxba$"};
//     GeneralizedSuffixTree* stree = new GeneralizedSuffixTree(seqs);
//     cout << stree->newick() << endl;    
//
//
// TODO
// ----
// [1] Support multiple strings (generalized suffix tree) [DONE]
// [2] Prefix-suffix overlap searching for a set of strings
//
//
// References
// ----------
// Ukkonen, E. On-line construction of suffix tree. 1995.
// Gusfield, D. Algorithms on strings, trees, and sequences. 1997.
//
// 
// Last Changed 
// ------------
// July 12, 2015    Feng Zeng    Create it.
// 
// July 13, 2015    Feng Zeng    Add postorder iterator.              
//                               Add leaf node iterator.
//                               Add preorder iterator.
//                               Add newick format output.
//                               Add generalized suffix tree.
// July 20, 2015    Feng Zeng    Add Range type.
//
//
//

#ifndef _SUFFIXTREE_HPP
#define _SUFFIXTREE_HPP

#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <cstddef> 
#include <stack>
using namespace std;

class SuffixTreeNode;

// Postorder iterator
class SuffixTreePostorderIterator:public iterator<forward_iterator_tag,
                                                  SuffixTreeNode,
                                                  ptrdiff_t,
                                                  SuffixTreeNode*,
                                                  SuffixTreeNode&>
{
public:
    SuffixTreeNode*             iter;
    stack<SuffixTreeNode*>      to_visit;
    stack<int>                  visited_count;
    int                         n;

public:
    explicit SuffixTreePostorderIterator(SuffixTreeNode* root,int _n);

public:
    SuffixTreePostorderIterator& operator++();
    SuffixTreeNode& operator*() const
    {
        return *iter;
    }
    SuffixTreeNode* operator->() const
    {
        return iter;
    }
    bool operator!= (const SuffixTreePostorderIterator& rhs) const
    {
        return n != rhs.n;
    }
    bool operator< (const SuffixTreePostorderIterator& rhs) const
    {
        return n < rhs.n;
    }
};


// Leaf node iterator
class SuffixTreeLeafIterator:public iterator<forward_iterator_tag,
                                             SuffixTreeNode,
                                             ptrdiff_t,
                                             SuffixTreeNode*,
                                             SuffixTreeNode&>
{
public:
    SuffixTreeNode*        iter;
    stack<SuffixTreeNode*> to_visit;
    stack<int>             visited_count;
    int                    n;

public:
    explicit SuffixTreeLeafIterator(SuffixTreeNode* root, int _n);

public:
    SuffixTreeLeafIterator& operator++();
    SuffixTreeNode& operator*() const
    {
        return *iter;
    }
    SuffixTreeNode* operator->() const
    {
        return iter;
    }
    bool operator!= (const SuffixTreeLeafIterator& rhs) const
    {
        return n != rhs.n;
    }
    bool operator< (const SuffixTreeLeafIterator& rhs) const
    {
        return n < rhs.n;
    }
};

// Preorder iterator
class SuffixTreePreorderIterator:public iterator<forward_iterator_tag,
                                                 SuffixTreeNode,
                                                 ptrdiff_t,
                                                 SuffixTreeNode*,
                                                 SuffixTreeNode&>
{
public:
    SuffixTreeNode* iter;
    stack<SuffixTreeNode*> to_visit;
    int n;

public:
    explicit SuffixTreePreorderIterator(SuffixTreeNode* root, int _n);

public:
    SuffixTreePreorderIterator& operator++();
    SuffixTreeNode& operator*() const
    {
        return *iter;
    }
    SuffixTreeNode* operator->() const
    {
        return iter;
    }
    bool operator!=(const SuffixTreePreorderIterator& rhs) const
    {
        return n != rhs.n;
    }
    bool operator<(const SuffixTreePreorderIterator& rhs) const
    {
        return n < rhs.n;
    }
};

// suffix tree
class SuffixTreeNode;
struct test_split_t{
    bool split;                // split an edge or create a new leaf node
    SuffixTreeNode* leaf;      // used for common suffix in generalized suffix tree
    SuffixTreeNode* P;         // updated pointer to the deepest node containing implicit suffixes
    int K;                     // updated number of implicit suffixes
    int d;                     // distance of spliting position from P
};
typedef test_split_t TestSplitResult;


typedef tuple<int,int> Range;

class SuffixTreeNode{

public:
    SuffixTreeNode();
    SuffixTreeNode(string& sequence);
    SuffixTreeNode(vector<string>& sequences);
    SuffixTreeNode(int _seq_id,int _id,char _key,Range& _range,string& _path_label,int _path_length);
    ~SuffixTreeNode(){;}

public:
    // tree manipulations
    void insert_child(char key, SuffixTreeNode* node);
    SuffixTreeNode* get_root();
    bool is_leaf();
    string newick();

    // iterator
    typedef SuffixTreePostorderIterator postorder_iterator;
    postorder_iterator postorder_begin() {return postorder_iterator(this,0);}
    postorder_iterator postorder_end() {return postorder_iterator(this,N);}

    typedef SuffixTreeLeafIterator leaf_iterator;
    leaf_iterator leaf_begin() {return leaf_iterator(this,0);}
    leaf_iterator leaf_end() {return leaf_iterator(this,L);}

public:
    // suffix tree construction
    TestSplitResult* test_split(string& s,int i,int k);
    void build(int seq_id,string& sequence);
    void build(vector<string>& sequences);
    
public:    
    // variables
    int                       id;
    vector<int>               seq_id;
    char                      key;
    map<char,SuffixTreeNode*> children;
    vector<Range>             range;
    string                    path_label;
    int                       path_length;
    SuffixTreeNode*           parent;
    SuffixTreeNode*           suffix_link;
    int                       N;
    int                       L;
    vector<int>               seq_len;

};

typedef SuffixTreeNode SuffixTree;
typedef SuffixTreeNode GeneralizedSuffixTree;

#endif
