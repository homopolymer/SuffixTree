#include "SuffixTree.hpp"

SuffixTreePreorderIterator::SuffixTreePreorderIterator(SuffixTreeNode* root, int _n)
{
    iter = root;
    n = _n+1;
    for (auto it=root->children.begin(); it!=root->children.end(); ++it)
        to_visit.push(it->second);
}


SuffixTreePreorderIterator& SuffixTreePreorderIterator::operator++()
{
    while(!to_visit.empty())
    {
        n += 1;
        iter = to_visit.top();
        for (auto it=iter->children.begin(); it!=iter->children.end(); ++it)
            to_visit.push(it->second);
        return *this;
    }
    n += 1;
}
