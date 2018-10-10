#pragma once
#include "suffix_array/DC3.hpp"
#define k 256

class Aligner
{
  public:
    Aligner() = default;
    Aligner(string& str)
    {
        build(str);
    }

    void build(string& str)
    {
        vector<int> s(str.length());
        for(int i = 0; i<str.length(); i++)
            s[i] = str[i];
        
        SA = DC3(s);

        // init L
        L.clear();

        // get_L_via_SA
        for(int& i: SA)
            if(i == 0)
                L += char(0);   // sentinel
            else
                L += str[i-1];
        
        // build tally table
        for(int j = 0; j < 256; j++)
        {
            tally[j].resize(L.length());
            // init tally
            for(auto& i: tally[j])
                i = 0;
        }

        tally[ L[0] ][0]++;


        for(int i = 1; i < L.length(); i++)
        {
            for(int j = 0; j < 256; j++)
            {
                tally[j][i] = tally[j][i-1];
            }
            tally[ L[i] ][i]++;
        }
        
        // init F
        for(int i = 0; i<256; i++)
        {
            F[i] = 0;
        }

        // build F
        for(int i : s)
            F[i]++;

        for(int i = 1; i<256; i++)
        {
            F[i] += F[i-1];   // tail)
        }
    }
    
    friend ostream& operator<<(ostream& out, Aligner a)
    {

    }

    vector<int> search(string& str, int I, int a, int b)
    {
        int i = str[I];

        // [start, end) of F
        int f, ff;

        f = F[ i-1 ] + a;
        ff = F[ i-1 ] + b;

        if( !(ff>f) )
            return {};
        else if( I == 0 )
            return { begin(SA)+f, begin(SA)+ff };

        // str[I-1] range of a~b in L
        int s, ss;
        s =  tally[ str[I-1] ][f-1];
        ss = tally[ str[I-1] ][ff-1];


        return search(str, I-1, s, ss);

    }

    vector<int> query(string& str)
    {
        int I = str.length()-1;
        int i = str[I];
        // [start, end) of F
        int a, b;
         
        a = F[ i-1 ];
        b = F[ i ];

        if( !(b>a) )
            return {};
        else if( I == 0)
            return { begin(SA)+a, begin(SA)+b };

        // str[I-1] range of a~b in L
        int s, ss;
        s =  tally[ str[I-1] ][a-1];
        ss = tally[ str[I-1] ][b-1];


        return search(str, I-1, s, ss);
    }

    int F[256];
    vector<int> tally[256];
    vector<int> SA;     
    string L;
};