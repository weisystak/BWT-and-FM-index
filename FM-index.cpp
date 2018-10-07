#include "DC3.hpp"
#include <iostream>
#include <map>
#define tally_fraction 10
#define SA_fraction 3

vector<string> get_all_sorted_rotate(string& str)
{
    vector<string> all_rotate;
    for(int i = 0; i< str.length(); ++i)
    {
        all_rotate.push_back(str);
        rotate(begin(str), begin(str)+1, end(str));
    }
    sort(begin(all_rotate), end(all_rotate));

    return all_rotate;
}

string get_L_via_all_rotate(string& str)
{
    auto all_rotate = get_all_sorted_rotate(str);
    string L;
    for(auto a: all_rotate)
    {
        L += *rbegin(a);
    }
    return L;
}

string get_L_via_SA(vector<int>& SA, string& str)
{
    string L;
    for(int& i: SA)
        if(i == 0)
            L += '$';
        else
            L += str[i-1];
    return L;
}

struct Idx
{
    idx() = default;
    idx(int b, int e): begin(b), end(e){}
    int begin, end;
};

map<char, Idx> get_char_idx(vector<int>& SA, string& str)
{
    map<char, int> cnt;

    for(int i: SA)
    {
        cnt[ str[i] ] ++;
    }
    
    int begin = 0;
    map<char, idx> m;
    Idx idx;

    for(auto& [ch, i] : cnt)
    {
        idx.begin = begin;
        idx.end = begin + i;
        m[ch] = idx;
        
        begin = idx.end;
    }
    return m;
}

int main()
{
    string str = "mississippi";
    str.push_back(0);

    vector<int> v;
    for(char& ch : str)
        v.push_back(ch);
    
    auto SA = DC3(v);
    *str.rbegin() = '$';



    string L = get_L_via_SA(SA, str);
    string t = get_L_via_all_rotate(str);
    

    cout<<L<<endl;
    cout<<t<<endl;
    
    map<char, Idx> idx_table = get_char_idx(SA, str);
}