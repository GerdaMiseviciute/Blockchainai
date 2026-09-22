#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <string>
using namespace std;
using namespace boost::multiprecision;
void funkcija(string s)
{
    cpp_int maximum = (cpp_int(1) << 256) - 1;
    // unsigned int maximum=115792089237316195423570985008687907853269984665640564039457584007913129639935;
    cout<<maximum<<endl;
    int a=3;
    cpp_int hash=1;
    for(unsigned char c:s)
    {
        cout<<(int)c<<" ";
        hash=hash*c+a;
        cout<<hash<<endl;
    }
    int l=to_string(hash).length();
    cout<<l<<endl;
    cpp_dec_float_100 Hash=cpp_dec_float_100(hash);
    for(int i=0; i<l; i++)
        Hash/=10;
    cout<<Hash<<endl;
    Hash *= cpp_dec_float_100(maximum);
    // cpp_int result =cpp_int(Hash);
    hash=cpp_int(Hash);
    cout<<Hash<<endl;
    cout<<hash<<endl;
    cout<<hex<<hash<<endl;
}
int main()
{
    ifstream fd("ivestis.txt");
    ofstream fr("isvestis.txt");

    int choice;
    cout<<"===Meniu==="<<endl;
    cout<<"1. Nuskaityti duomenis is failo"<<endl;
    cout<<"2. Suvesti duomenis ranka"<<endl;
    while(true)
    {
        try
        {
            cin>>choice;
            if(cin.fail() || choice<1 || choice>2)
                throw std::runtime_error("Iveskite skaiciu 1-2: ");

            cin.ignore(1000, '\n');
            break;
        }
        catch(const std::exception& e)
        {
            cerr<<"Klaida! "<<e.what()<<endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    switch(choice)
    {
        case 1:
        {
            string filename;
            system("dir *.txt");
            cout<<"Ivesk failo pavadinima: ";
            cin>>filename;
            try
            {
                string eil;
                ifstream fd(filename);
                if(!fd)
                {
                    throw std::runtime_error("Nepavyko atidaryti failo.");
                }
                getline(fd, eil);
                funkcija(eil);

                fd.close();
            }
            catch(const std::exception& e)
            {
                cerr<<"Klaida: "<<e.what()<<endl;
                terminate();
            }
            break;
        }
        case 2:
        {    
            string s;
            cin>>s;
            funkcija(s);
            break;
        }
    }
    return 0;
}