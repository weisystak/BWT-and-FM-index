#include <BWT.hpp>
#include <gtest/gtest.h>
#include <fstream>





TEST(BWT, file)
{
    ifstream ifs("unit_test/test_data/article.txt");
    string str;
    
    int c = 0;

    while(getline(ifs, str))
    {
        cout<<"**************** CASE "<<c++<<" *********************"<<endl;
        cout<<str<<endl;
        
        str.push_back(0);
        Aligner a(str);

        
        string query_str = "h";
        auto v = a.query(query_str);
        cout<<"search for \""<<query_str<<"\""<<endl;
        cout<<v.size()<<endl;
        for(int i: v)
            cout<<i<<": "<<str.substr(i, 20)<<endl;
        cout<<endl;

        query_str = "th";
        v = a.query(query_str);
        cout<<"search for \""<<query_str<<"\""<<endl;
        cout<<v.size()<<endl;
        for(int i: v)
            cout<<i<<": "<<str.substr(i, 20)<<endl;
        cout<<endl;

        query_str = "the";
        cout<<"search for \""<<query_str<<"\""<<endl;
        v = a.query(query_str);
        cout<<v.size()<<endl;
        for(int i: v)
            cout<<i<<": "<<str.substr(i, 20)<<endl;
        cout<<endl;
    }
}