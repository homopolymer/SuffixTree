#include "SuffixTree.hpp"
#include <utility>
#include <iostream>
using namespace std;

SuffixTreeNode::SuffixTreeNode()
{
    id          = 0;
    seq_id      = vector<int>(1,0);
    key         = 0;
    parent      = 0;
    range       = vector<int>(2,0);
    children    = map<char,SuffixTreeNode*>();
    path_label  = "";
    path_length = 0;
    N           = 0;
    L           = 0;
}

SuffixTreeNode::SuffixTreeNode(string& sequence)
{
    id          = 0;
    seq_id      = vector<int>(1,0);
    key         = 0;
    parent      = 0;
    range       = vector<int>(2,0);
    children    = map<char,SuffixTreeNode*>();
    path_label  = "";
    path_length = 0;
    N           = 0;
    L           = 0;

    build(0,sequence);
}

SuffixTreeNode::SuffixTreeNode(vector<string>& sequences)
{
    id          = 0;
    seq_id      = vector<int>(1,0);
    key         = 0;
    parent      = 0;
    range       = vector<int>(2,0);
    children    = map<char,SuffixTreeNode*>();
    path_label  = "";
    path_length = 0;
    N           = 0;
    L           = 0;

    build(sequences);
}

SuffixTreeNode::SuffixTreeNode(int _seq_id,int _id,char _key,vector<int>& _range,string&_path_label,int _path_length)
{
    id          = _id;
    seq_id      = vector<int>(1,_seq_id);
    key         = _key;
    parent      = 0;
    range       = _range;
    children    = map<char,SuffixTreeNode*>();
    path_label  = _path_label;
    path_length = _path_length;
}

//---------------------------------------------------------------------
// Tree Manipulators
//
void SuffixTreeNode::insert_child(char key,SuffixTreeNode* node)
{
    auto it = this->children.find(key);
    if (it == this->children.end())
    {
        node->parent = this;
        this->children.emplace(make_pair(key,node));
    }else
    {
        SuffixTreeNode* child = it->second;
        char new_key = child->path_label[node->path_length];
        child->key = new_key;
        child->parent = node;
        node->children.emplace(make_pair(child->key,child));

        node->parent = this;
        this->children[key] = node;
    }
}

SuffixTreeNode* SuffixTreeNode::get_root()
{
    SuffixTreeNode* root = this;
    while(root->parent!=0)
    {
        root = root->parent;
    }
    return root;
}

bool SuffixTreeNode::is_leaf()
{
    return this->children.empty();
}

//---------------------------------------------------------------------
// Suffix Tree Constructor
//
TestSplitResult* SuffixTreeNode::test_split(string& s,int i,int k)
{
    TestSplitResult* result = new TestSplitResult;

    // find the deepest node containing k-th implicit suffix
    SuffixTreeNode* n = this;
    while(n->path_length<=k)
    {
        char key = s[i-k+n->path_length];
        auto it = n->children.find(key);
        if (it!=n->children.end() && it->second->path_length<=k)
        {
            if (it->second->is_leaf())
                break;
            n = it->second;
        }else 
        {
            break;
        }
    }    

    // test
    if (k==n->path_length)
    {
        auto c = n->children.find(s[i-k+n->path_length]);
        if (c==n->children.end())
        {
            result->split = true;
            result->leaf = 0;
            result->P = n;
            result->K = k-1;
            if (result->K<0) result->K = 0;
            result->d = k-n->path_length;
        }else{
            if (c->second->is_leaf() && i+1==s.length()) // used for generalized suffix tree
            {
                result->split = false;
                result->leaf = c->second;
                result->P = n;
                result->K = k-1;
                if (result->K<0) result->K = 0;
                result->d = 0;
            }else
            {
                result->split = false;
                result->leaf = 0;
                result->P = n;
                result->K = k+1;
                result->d = 0;
            }
        }
    }else
    {
        auto c = n->children.find(s[i-k+n->path_length]);
        if (c->second->path_label[k] != s[i])
        {
            result->split = true;
            result->leaf = 0;
            result->P = n;
            result->K = k-1;
            if (result->K<0) result->K = 0;
            result->d = k-n->path_length;
        }else{
            if (c->second->is_leaf() && i+1==s.length()) // used for generalized suffix tree
            {
                result->split = false;
                result->leaf = c->second;
                result->P = n;
                result->K = k-1;
                if (result->K<0) result->K = 0;
                result->d = 0;
            }else
            {
                result->split = false;
                result->leaf = 0;
                result->P = n;
                result->K = k+1;
                result->d = 0;
            }
        }
    }

    return result;
}

void SuffixTreeNode::build(int seq_id,string& sequence)
{
    SuffixTreeNode* P; // pointer to the deepest node containing implicit suffixes
    int K = 0;         // the number of implicit suffixes
    int id = N-1;
    if (id<0) id=0;
    int n = 0;
    int LL = sequence.length();
    TestSplitResult* test = 0;
    SuffixTreeNode* SL; // used for suffix link

    P = this->get_root();
    P->suffix_link = P;

    // scan though sequence
    for (int i=0; i<LL; i++)
    {
        // scan through implicit suffixes
        for (int k=K; k>=0; k--)
        {
            SL = 0;
            test = test_split(sequence,i,k);
            if (!test->split)
            {
                if (test->leaf!=0) // used for generalized suffix tree
                {
                    test->leaf->seq_id.push_back(seq_id);
                    P = test->P->suffix_link;
                    K = test->K;
                }else{
                    P = test->P;
                    K = test->K;
                    break;
                }
            }else{
                SuffixTreeNode* Q = test->P;
                if (test->d>0) // create an internal node
                {
                    n += 1;
                    id += 1;
                    char i_key = sequence[i-k+Q->path_length];
                    vector<int> i_range = {i-k,i-k+Q->path_length+test->d};
                    string i_path_label = sequence.substr(i-k,Q->path_length+test->d);
                    int i_path_length = Q->path_length+test->d;
                    SuffixTreeNode* internal = new SuffixTreeNode(seq_id,id,i_key,i_range,i_path_label,i_path_length);
                    internal->suffix_link = Q;
                    if (SL!=0)
                        SL->suffix_link = internal;
                    SL = internal;
                    Q->insert_child(i_key,internal);
   
                    n += 1;
                    id += 1;
                    char key = sequence[i];
                    vector<int> range = {i-k,LL};
                    string path_label = sequence.substr(i-k,LL-i+k);
                    int path_length = LL-i+k;
                    SuffixTreeNode* leaf = new SuffixTreeNode(seq_id,id,key,range,path_label,path_length);
                    leaf->suffix_link = 0;
                    internal->insert_child(key,leaf);
                }else{
                    n += 1;
                    id += 1;
                    char key = sequence[i];
                    vector<int> range = {i-k,LL};
                    string path_label = sequence.substr(i-k,LL-i+k);
                    int path_length = LL-i+k;
                    SuffixTreeNode* leaf = new SuffixTreeNode(seq_id,id,key,range,path_label,path_length);
                    leaf->suffix_link = 0;
                    Q->insert_child(key,leaf);
                }
                P = Q->suffix_link;
                K = test->K;
            }
        }
    }

    if (seq_id==0) this->N = n+1;
    else this->N += n;
    if (seq_id>0)
        this->L += LL-1;
    this->seq_len.push_back(LL);
}

// generalized suffix tree builder
void SuffixTreeNode::build(vector<string>& sequences)
{
    for (int i=0; i<sequences.size(); i++)
        build(i,sequences[i]);
}

string SuffixTreeNode::newick()
{
    stack<string> nw;
    auto it = this->postorder_begin();
    for (;it<this->postorder_end();++it)
    {
        if (it->is_leaf())
        {
            string s = "";
            for (int j=0; j<it->seq_id.size(); j++)
                s = s + to_string(it->seq_id[j]) + "_";
            nw.push(s+"_"+it->path_label);
        }else{
            string t = "";
            for (int i=0; i<it->children.size()-1; i++)
            {
                t = t + nw.top() + ",";
                nw.pop();
            }
            t = t + nw.top();
            nw.pop();
            nw.push("("+t+")");
        }
    }
    string result = nw.top() + ";";
    return result;
}

