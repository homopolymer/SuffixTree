#include "SuffixTree.hpp"

SuffixTreeLeafIterator::SuffixTreeLeafIterator(SuffixTreeNode* root, int _n)
{
    n = _n;
    to_visit.push(root);
    visited_count.push(0);
    while(!to_visit.empty())
    {
        SuffixTreeNode* node = to_visit.top();
        int c = visited_count.top();
        if (node->is_leaf())
        {
            n += 1;
            iter = node;
            to_visit.pop();
            visited_count.pop();
            break;
        }else if (c<1)
        {
            visited_count.pop();
            visited_count.push(1);
            for (auto rit=node->children.rbegin(); rit!=node->children.rend(); rit++){
                to_visit.push(rit->second);
                visited_count.push(0);
            }
        }
    }
}

SuffixTreeLeafIterator& SuffixTreeLeafIterator::operator++()
{
    while(!to_visit.empty())
    {
        SuffixTreeNode* node = to_visit.top();
        int c = visited_count.top();
        if (node->is_leaf())
        {
            n += 1;
            iter = node;
            to_visit.pop();
            visited_count.pop();
            return *this;
        }else
        {
            if (c==1)
            {
                to_visit.pop();
                visited_count.pop();
            }else if (c<1)
            {
                visited_count.pop();
                visited_count.push(1);
                for (auto rit=node->children.rbegin(); rit!=node->children.rend(); rit++)
                {
                    to_visit.push(rit->second);
                    visited_count.push(0);
                }
            }
        }
    }
    n += 1;
}
