#pragma once
#include <omp.h>
#include "suffix_array/DC3.hpp"
#define k 256   // number of characters
#define z 15     // tally table factor
#define e 5     // SA factor


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
        assert(str.length() != 0);
        
        vector<int> s(str.length());
        # pragma omp parallel for
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

        // shrink SA
        int ll=0;
        for(int i = 0; i< SA.size(); i+=e)
        {
            SA[ll++] = SA[i];
        }
    
        SA.resize(ll);
        SA.shrink_to_fit();
    

        // build tally table
        int tally_len = 1 + L.length()/z;
        if(L.length() % z == 0) tally_len--;
    
        vector<int>::iterator it; 
        # pragma omp parallel for private(it)
        for(int j = 0; j < 256; j++)
        {
            tally[j].resize( tally_len );
            tally[j].shrink_to_fit();
            // init tally
            //for(auto& i: tally[j])
            //    i = 0;
            # pragma omp parallel for
            for(it=tally[j].begin();it<tally[j].end();it++)
                *it=0;
        }

        tally[ L[0] ][0]++;

        for(int i = 1, ii = 1; i < L.length(); i++)
        {
            tally[ L[i] ][ii]++;
            if(i % z == 0)
            {
                for(int j = 0; j < 256; j++)
                    tally[j][ii] += tally[j][ii-1];
                ii++;
                if(ii == tally_len)
                    break;
            }
        }
        
        // init F
        # pragma omp parallel for
        for(int i = 0; i<256; i++)
        {
            F[i] = 0;
        }

        // build F
        //for(int i : s)
        //    F[i]++;
        # pragma omp parallel for
        for(it = s.begin(); it< s.end(); it++)
            # pragma omp atomic
            F[*it]++;

        for(int i = 1; i<256; i++)
        {
            F[i] += F[i-1];   // tail)
        }
    }
    
    int deduce_idx(int i)
    {  
        int ch, s;
        int cnt = 0;
        while(i % e != 0)
        {
            ch = L[i];
            
            s =  tally[ ch ][(i-1)/z];
            s  += tally_offset(ch, i-1);
            
            i = F[ ch-1 ] + s;
            cnt++;
        }
        if(i/e == 0)    // boundary case: reach '$'
            return cnt-1;
        return SA[i/e] + cnt;
    }

    int tally_offset(int ch, int i)
    {
        int r = i%z, off = 0;
        int base = i-r+1;
        # pragma omp parallel for reduction(+:off)
        for(int j = 0; j < r; j++){
            if( ch == L[ base+j ])
                off++;
        }
        return off;
    }

    vector<int> search(string& str, int I, int a, int b)
    {
        int i = str[I];
        assert(i != 0);

        // [start, end) of F
        int f, ff;

        f = F[ i-1 ] + a;
        ff = F[ i-1 ] + b;

        if( !(ff>f) )
            return {};
        else if( I == 0 )
        {
            vector<int> res;
            for(int i = f; i < ff; i++)
                res.push_back(deduce_idx(i));
            
            return res;
        }
            

        // str[I-1] range of a~b in L
        int s, ss;
        s =  tally[ str[I-1] ][(f-1)/z];
        ss = tally[ str[I-1] ][(ff-1)/z];

        s  += tally_offset(str[I-1], f-1);
        ss += tally_offset(str[I-1], ff-1);

        return search(str, I-1, s, ss);

    }

    vector<int> query(string& str)
    {
        int I = str.length()-1;
        int i = str[I];
        assert(i != 0);
        // [start, end) of F
        int a, b;
         
        a = F[ i-1 ];
        b = F[ i ];

        if( !(b>a) )
            return {};
        else if( I == 0)
        {
            vector<int> res;
            
            for(int i = a; i < b; i++)
                res.push_back(deduce_idx(i));
            
            return res;
        }

        // str[I-1] range of a~b in L
        int s, ss;
        s =  tally[ str[I-1] ][(a-1)/z];
        ss = tally[ str[I-1] ][(b-1)/z];

        s  += tally_offset(str[I-1], a-1);
        ss += tally_offset(str[I-1], b-1);


        return search(str, I-1, s, ss);
    }

    int F[256];
    vector<int> tally[256];
    vector<int> SA;     
    string L;
};
