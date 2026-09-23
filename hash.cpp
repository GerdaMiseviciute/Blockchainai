#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <string>
using namespace std;
using namespace boost::multiprecision;
cpp_int funkcija(string s)
{
    cpp_int maximum = (cpp_int(1) << 256) - 1;
    cpp_int hash=1;
    for(unsigned char c:s)
    {
        hash=hash*257+c+1;
        if(c!='\0')
            hash*=c;
    }
    int l=to_string(hash).length();
    cpp_dec_float_100 Hash=cpp_dec_float_100(hash);
    for(int i=0; i<l; i++)
        Hash/=10;
    Hash *= cpp_dec_float_100(maximum);
    hash=cpp_int(Hash);
    return hash;
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
                    throw std::runtime_error("Nepavyko perskaityti failo.");
                }
                getline(fd, eil);
                if (!eil.empty())
                    fr<<hex<<funkcija(eil)<<endl;
                else fr<<"Failas tuščias"<<endl;

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
            getline(cin, s);
            if (!s.empty())
                fr<<hex<<funkcija(s)<<endl;
            break;
        }
    }
    return 0;
}