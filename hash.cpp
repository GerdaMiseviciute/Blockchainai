#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <random>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <iomanip>
#include <windows.h>
using namespace std;
using namespace chrono;
using namespace boost::multiprecision;

const uint64_t SEED = 123456789ULL;
std::mt19937_64 generatorius(SEED);

string funkcija(string s)
{
    if(s.empty())
    {
        return "Failas yra tuščias";
    }
        // throw std::runtime_error("Failas tuščias");
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
    stringstream ss;
    ss<<hex<<hash;
    return ss.str();
}
struct Istrauka
{
    size_t eiluciu_sk;
    string tekstas;
};
struct Statistika
{
    double bituMin = 100.0;
    double bituMax = 0.0;
    double bituSuma = 0.0;

    double hexMin = 100.0;
    double hexMax = 0.0;
    double hexSuma = 0.0;

    long long kiekis = 0;

    void prideti(double bitai, double hex)
    {
        bituMin = min(bituMin, bitai);
        bituMax = max(bituMax, bitai);
        bituSuma += bitai;

        hexMin = min(hexMin, hex);
        hexMax = max(hexMax, hex);
        hexSuma += hex;

        kiekis++;
    }

    double bituVidurkis() const
    {
        return bituSuma / kiekis;
    }

    double hexVidurkis() const
    {
        return hexSuma / kiekis;
    }
};
struct AtakosRezultatas
{
    long long bandymai;
    double laikas;
    vector<string> sutapimai;
};
char atsitiktineRaide()
{
    // uniform_int_distribution<int> dist(0, 51);

    // int x = dist(gen);

    // if (x < 26)
    //     return 'A' + x;
    // else
    //     return 'a' + (x - 26);
    static const string ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    uniform_int_distribution<int> dist(0, ALPHABET.size() - 1);
    return ALPHABET[dist(generatorius)];
}
string generuotiString(int ilgis)
{
    static const string ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    uniform_int_distribution<int> dist(0, ALPHABET.size() - 1);

    string s;
    s.reserve(ilgis);

    for (int i = 0; i < ilgis; i++)
        s += ALPHABET[dist(generatorius)];

    return s;
}
string pakeistiVienaSimboli(string&s)
{
    string pakeistas=s;
    uniform_int_distribution<int> pozicija(0, s.size() - 1);

    int p = pozicija(generatorius);

    char senas = s[p];
    char naujas;

    do
    {
        naujas=atsitiktineRaide();
    }
    while (naujas==senas);

    pakeistas[p] = naujas;
    return pakeistas;
}
double hexSkirtumas(const string h1, const string h2)
{
    int skirtingi = 0;

    for (int i = 0; i < 64; i++)
    {
        if (h1[i] != h2[i])
            skirtingi++;
    }

    return 100.0 * skirtingi / 64.0;
}
string hexToBits(const string& hex)
{
    string bits;
    bits.reserve(hex.size() * 4);

    for (char c : hex)
    {
        switch (c)
        {
            case '0': bits += "0000"; break;
            case '1': bits += "0001"; break;
            case '2': bits += "0010"; break;
            case '3': bits += "0011"; break;
            case '4': bits += "0100"; break;
            case '5': bits += "0101"; break;
            case '6': bits += "0110"; break;
            case '7': bits += "0111"; break;
            case '8': bits += "1000"; break;
            case '9': bits += "1001"; break;
            case 'A': case 'a': bits += "1010"; break;
            case 'B': case 'b': bits += "1011"; break;
            case 'C': case 'c': bits += "1100"; break;
            case 'D': case 'd': bits += "1101"; break;
            case 'E': case 'e': bits += "1110"; break;
            case 'F': case 'f': bits += "1111"; break;
        }
    }

    return bits;
}
double bituSkirtumas(const string h1, const string h2)
{
    int skirtingi = 0;

    for (int i = 0; i < 256; i++)
    {
        if (h1[i] != h2[i])
            skirtingi++;
    }
    return 100.0 * skirtingi / 256.0;
}
Statistika atlikti6Eksperimenta(int ilgis, vector<long long>&histograma)
{
    Statistika statistika;
    for (int i = 0; i < 25000; i++)
    {
        string s1 = generuotiString(ilgis);
        string s2 = pakeistiVienaSimboli(s1);

        string hash1 = funkcija(s1);
        string hash2 = funkcija(s2);

        double bitai = bituSkirtumas(hexToBits(hash1), hexToBits(hash2));
        double hex = hexSkirtumas(hash1, hash2);

        statistika.prideti(bitai, hex);
        // pridetiIHisto(histograma, bitai);
        int indeksas;
        if (bitai < 20)
            indeksas = 0;
        else if (bitai < 40)
            indeksas = 1;
        else if (bitai < 60)
            indeksas = 2;
        else if (bitai < 80)
            indeksas = 3;
        else indeksas=4;

        histograma[indeksas]++;
    }
    return statistika;
}
void koliziju_sk(int ilgis, map<string, set<string>>hashai, long long &poruKolizijos)
{
    for (int i = 0; i < 100000; i++)
    {

        string a = generuotiString(ilgis);
        // sugeneruotosIvestys.push_back(a);
        string b = generuotiString(ilgis);

        string hashA=funkcija(a);
        string hashB=funkcija(b);

        hashai[hashB].insert(b);
        hashai[hashB].insert(a);
        // sugeneruotosIvestys.push_back(b);

        while (a == b)
            b = generuotiString(ilgis);

        if (hashA==hashB)
        {
            poruKolizijos++;
        }
    }
}
AtakosRezultatas beDruskos(const string& tikslinisHash, const vector<string>& kandidatai)
{
    AtakosRezultatas rezultatas;
    rezultatas.bandymai=0;

    auto pradzia=chrono::high_resolution_clock::now();

    for (const string& kandidatas : kandidatai)
    {
        rezultatas.bandymai++;
        string h=funkcija(kandidatas);

        if (h==tikslinisHash)
        {
            rezultatas.sutapimai.push_back(kandidatas);
        }
    }

    auto pabaiga=chrono::high_resolution_clock::now();

    rezultatas.laikas=chrono::duration<double>(pabaiga - pradzia).count();
    return rezultatas;
}
string generuotiSalt()
{
    random_device rd;
    mt19937 generuoti(rd());
    uniform_int_distribution<int> dist(0, 255);

    string salt;

    for (int i = 0; i < 16; i++)
        salt += static_cast<char>(dist(generuoti));

    return salt;
}
AtakosRezultatas SuViesaDruska(const string& tikslinisHash, const string& viesaDruska, const vector<string>& kandidatai)
{
    AtakosRezultatas rezultatas;
    rezultatas.bandymai=0;
    
    auto pradzia=chrono::high_resolution_clock::now();

    for (const string& kandidatas : kandidatai)
    {
        rezultatas.bandymai++;
        string h=funkcija(kandidatas+viesaDruska);
        if (h==tikslinisHash)
        {
            rezultatas.sutapimai.push_back(kandidatas);
        }
    }

    auto pabaiga=chrono::high_resolution_clock::now();

    rezultatas.laikas=chrono::duration<double>(pabaiga - pradzia).count();
    return rezultatas;
}
int main()
{
    SetConsoleCP(CP_UTF8);        // konsolės įvestis → UTF-8
    SetConsoleOutputCP(CP_UTF8);
    ifstream fd("ivestis.txt");
    ofstream fr("isvestis.txt");
    int choice;
    cout<<"===Meniu==="<<endl;
    cout<<"1. Nuskaityti duomenis is failo"<<endl;
    cout<<"2. Suvesti duomenis ranka"<<endl;
    cout<<"3. Atlikti pirma eksperimenta"<<endl;
    cout<<"4. Atlikti antra eksperimenta"<<endl;
    cout<<"5. Atlikti ketvirta eksperimenta"<<endl;
    cout<<"6. Atlikti penkta eksperimenta"<<endl;
    cout<<"7. Atlikti sesta eksperimenta"<<endl;
    cout<<"8. Atlikti septinta eksperimenta"<<endl;
    while(true)
    {
        try
        {
            cin>>choice;
            if(cin.fail() || choice<1 || choice>8)
                throw std::runtime_error("Iveskite skaiciu 1-8: ");

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
                fr<<funkcija(eil)<<endl;

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
            cout<<"Ivesk teksta"<<endl;
            string s;
            getline(cin, s);
            fr<<funkcija(s)<<endl;
            break;
        }
        case 3:
        {
            ifstream fd1("ivestis1.txt");
            ifstream fd2("ivestis2.txt");
            ifstream fd2a("ivestis2a.txt");
            ifstream fd3("ivestis3.txt");
            ifstream fd3a("ivestis3a.txt");
            ifstream fd4("ivestis4.txt");
            ifstream fd4a("ivestis4a.txt");
            ifstream fd5("ivestis5.txt");
            ifstream fd5a("ivestis5a.txt");
            ifstream fd6("ivestis6.txt");
            ifstream fd6a("ivestis6a.txt");
            ifstream fd6b("ivestis6b.txt");
            ifstream fd6c("ivestis6c.txt");
            ifstream fd6d("ivestis6d.txt");
            ifstream fd7("ivestis7.txt");
            fr<<"1 eksperimentas"<<endl<<endl;
            string eil1, eil2, eil2a, eil3, eil3a, eil4, eil4a, eil5, eil5a, eil6, eil6a, eil6b, eil6c, eil6d, eil7;
            getline(fd1, eil1);
            getline(fd2, eil2);
            getline(fd2a, eil2a);
            getline(fd3, eil3);
            getline(fd3a, eil3a);
            getline(fd4, eil4);
            getline(fd4a, eil4a);
            getline(fd5, eil5);
            getline(fd5a, eil5a);
            getline(fd6, eil6);
            getline(fd6a, eil6a);
            getline(fd6b, eil6b);
            getline(fd6c, eil6c);
            getline(fd6d, eil6d);
            getline(fd7, eil7);

            fr<<"Tuščias failas: "<<funkcija(eil1)<<endl;
            fr<<"Vieno simbolio failai:"<<endl;
            fr<<"'"<<eil2<<"': "<<funkcija(eil2)<<endl;
            fr<<"'"<<eil2a<<"': "<<funkcija(eil2a)<<endl<<endl;

            fr<<"Didelės apimties failai:"<<endl;
            fr<<"Pirmas failas: "<<funkcija(eil3)<<endl;
            fr<<"Tas pats failas, su pakeistu baitu failo gale: "<<funkcija(eil3a)<<endl;
            fr<<"Antras failas: "<<funkcija(eil4)<<endl;
            fr<<"Tas pats failas, su pakeistu baitu failo viduryje: "<<funkcija(eil4a)<<endl;
            fr<<"Trečias failas: "<<funkcija(eil5)<<endl;
            fr<<"Tas pats failas, su pakeistu baitu failo pradžioje: "<<funkcija(eil5a)<<endl<<endl;
            
            fr<<"Struktūruoti atvejai"<<endl;
            fr<<left<<setw(35)<<"Originalus tekstas: "<<funkcija(eil6)<<endl;
            fr<<left<<setw(35)<<"Tekstas su enter: "<<funkcija(eil6a)<<endl;
            fr<<left<<setw(35)<<"Tekstas su sukeistomis raidėmis: "<<funkcija(eil6b)<<endl;
            fr<<left<<setw(35)<<"Tekstas su tarpu pradžioje: "<<funkcija(eil6c)<<endl;
            fr<<left<<setw(35)<<"Tekstas su tarpu pabaigoje: "<<funkcija(eil6d)<<endl<<endl;

            fr<<"UTF-8 pavyzdys (338 simboliai, 392 baitai): "<<funkcija(eil7)<<endl;
            break;
        }
        case 4:
        {
            ifstream fd1("ivestis1.txt");
            ifstream fd2("ivestis2.txt");
            ifstream fd2a("ivestis2a.txt");
            ifstream fd3("ivestis3.txt");
            ifstream fd3a("ivestis3a.txt");
            ifstream fd4("ivestis4.txt");
            ifstream fd4a("ivestis4a.txt");
            ifstream fd5("ivestis5.txt");
            ifstream fd5a("ivestis5a.txt");
            ifstream fd6("ivestis6.txt");
            ifstream fd6a("ivestis6a.txt");
            ifstream fd6b("ivestis6b.txt");
            ifstream fd6c("ivestis6c.txt");
            ifstream fd6d("ivestis6d.txt");
            ifstream fd7("ivestis7.txt");
            fr<<"2 eksperimentas"<<endl<<endl;
            string eil1, eil2, eil2a, eil3, eil3a, eil4, eil4a, eil5, eil5a, eil6, eil6a, eil6b, eil6c, eil6d, eil7;
            getline(fd1, eil1);
            getline(fd2, eil2);
            getline(fd2a, eil2a);
            getline(fd3, eil3);
            getline(fd3a, eil3a);
            getline(fd4, eil4);
            getline(fd4a, eil4a);
            getline(fd5, eil5);
            getline(fd5a, eil5a);
            getline(fd6, eil6);
            getline(fd6a, eil6a);
            getline(fd6b, eil6b);
            getline(fd6c, eil6c);
            getline(fd6d, eil6d);
            getline(fd7, eil7);

            fr<<"Vieno simbolio failai:"<<endl;
            fr<<"'"<<eil2<<"': "<<funkcija(eil2).length()<<" hex simboliai"<<endl;
            fr<<"'"<<eil2a<<"': "<<funkcija(eil2a).length()<<" hex simboliai"<<endl<<endl;

            fr<<"Didelės apimties failai:"<<endl;
            fr<<"Pirmas failas: "<<funkcija(eil3).length()<<" hex simboliai"<<endl;
            fr<<"Tas pats failas, su pakeistu baitu failo gale: "<<funkcija(eil3a).length()<<" hex simboliai"<<endl;
            fr<<"Antras failas: "<<funkcija(eil4).length()<<" hex simboliai"<<endl;
            fr<<"Tas pats failas, su pakeistu baitu failo viduryje: "<<funkcija(eil4a).length()<<" hex simboliai"<<endl;
            fr<<"Trečias failas: "<<funkcija(eil5).length()<<" hex simboliai"<<endl;
            fr<<"Tas pats failas, su pakeistu baitu failo pradžioje: "<<funkcija(eil5a).length()<<" hex simboliai"<<endl<<endl;
            
            fr<<"Struktūruoti atvejai"<<endl;
            fr<<left<<setw(35)<<"Originalus tekstas: "<<funkcija(eil6).length()<<" hex simboliai"<<endl;
            fr<<left<<setw(35)<<"Tekstas su enter: "<<funkcija(eil6a).length()<<" hex simboliai"<<endl;
            fr<<left<<setw(35)<<"Tekstas su sukeistomis raidėmis: "<<funkcija(eil6b).length()<<" hex simboliai"<<endl;
            fr<<left<<setw(35)<<"Tekstas su tarpu pradžioje: "<<funkcija(eil6c).length()<<" hex simboliai"<<endl;
            fr<<left<<setw(35)<<"Tekstas su tarpu pabaigoje: "<<funkcija(eil6d).length()<<" hex simboliai"<<endl<<endl;

            fr<<"UTF-8 pavyzdys (338 simboliai, 392 baitai): "<<funkcija(eil7).length()<<" hex simboliai"<<endl;
            break;
        }
        case 5:
        {
            fr<<"4 eksperimentas"<<endl<<endl;
            ifstream fd("konstitucija.txt");
            string eil, visas_tekstas="";
            vector<string>eilutes;
            while(getline(fd, eil))
            {
                visas_tekstas+=eil;
                visas_tekstas+='\n';
                eilutes.push_back(eil);
            }

            vector<Istrauka> istraukos;
            size_t n=1;
            while(n<=eilutes.size())
            {
                string tekstas="";
                for(size_t i=0; i<n; i++)
                {
                    tekstas+=eilutes[i];
                    tekstas+='\n';
                }
                istraukos.push_back({n, tekstas});
                n*=2;
            }
            if(istraukos.back().eiluciu_sk!=eilutes.size())
                istraukos.push_back({eilutes.size(), visas_tekstas});
            cout<<left<<setw(25)<<"Eilučių skaičius"<<setw(20)<<"Maišos rezultatas"<<endl;
            for(size_t i=0; i<istraukos.size(); i++)
            {
                cout<<left<<setw(25)<<istraukos[i].eiluciu_sk<<setw(20)<<funkcija(istraukos[i].tekstas)<<endl;
            }

            fr<<left<<setw(25)<<"Eilučių skaičius"<<setw(20)<<"Baitai"<<setw(20)<<"Maišos funkcijos trukmė"<<endl;
            for(size_t i=0; i<istraukos.size(); i++)
            {
                auto pradzia = high_resolution_clock::now();
                string rezultatas = funkcija(istraukos[i].tekstas);
                auto pabaiga = high_resolution_clock::now();
                duration<double> laikas=pabaiga-pradzia;
                fr<<left<<setw(25)<<istraukos[i].eiluciu_sk<<setw(20)<<istraukos[i].tekstas.size()<<setw(20)<<laikas.count()<<endl;
            }
            break;
            // cout<<laikas.count()<<endl;
            // auto laikas = duration<double, std::micro>(pabaiga - pradzia).count();
        }
        case 6:
        {
            fr<<"5 eksperimentas"<<endl<<endl;
            map<string, set<string>>hash10;
            map<string, set<string>>hash100;
            map<string, set<string>>hash500;
            map<string, set<string>>hash1000;

            long long poruKolizijos=0;
            
            koliziju_sk(10, hash10, poruKolizijos);
            cout<<"Iš 10 baitų ilgio 100000 porų rasta kolizijų: "<<poruKolizijos<<endl;

            for (auto it = hash10.begin(); it != hash10.end(); )
            {
                if (it->second.size() == 1)
                    it = hash10.erase(it);
                else ++it;
            }
            cout<<"Tarp visų 10 baitų ivesčių rasta "<<hash10.size()<<" hash reikšmių, turinčių kolizijų"<<endl;

            poruKolizijos=0;
            koliziju_sk(100, hash100, poruKolizijos);
            cout<<"Iš 100 baitų ilgio 100000 porų rasta kolizijų: "<<poruKolizijos<<endl;

            for (auto it = hash100.begin(); it != hash100.end(); )
            {
                if (it->second.size() == 1)
                    it = hash100.erase(it);
                else ++it;
            }
            cout<<"Tarp visų 100 baitų ivesčių rasta "<<hash100.size()<<" hash reikšmių, turinčių kolizijų"<<endl;

            poruKolizijos=0;
            koliziju_sk(500, hash500, poruKolizijos);
            cout<<"Iš 500 baitų ilgio 100000 porų rasta kolizijų: "<<poruKolizijos<<endl;

            for (auto it = hash500.begin(); it != hash500.end(); )
            {
                if (it->second.size() == 1)
                    it = hash500.erase(it);
                else ++it;
            }
            cout<<"Tarp visų 500 baitų ivesčių rasta "<<hash500.size()<<" hash reikšmių, turinčių kolizijų"<<endl;

            poruKolizijos=0;
            koliziju_sk(1000, hash1000, poruKolizijos);
            cout<<"Iš 1000 baitų ilgio 100000 porų rasta kolizijų: "<<poruKolizijos<<endl;

            for (auto it = hash1000.begin(); it != hash1000.end(); )
            {
                if (it->second.size() == 1)
                    it = hash1000.erase(it);
                else ++it;
            }
            cout<<"Tarp visų 1000 baitų ivesčių rasta "<<hash1000.size()<<" hash reikšmių, turinčių kolizijų"<<endl;

            vector<string>ivestys={"abababab", "babababa", "aaaaaaa", "aaaaaab", "ABCDEFGHIJ", "JIHGFEDCBA", "BACDEFGHIJ", "ABCDEFGHJI"};
            for(int i=0; i<ivestys.size(); i++)
            {
                for(int j=i+1; j<ivestys.size(); j++)
                {
                    if(funkcija(ivestys[i])==funkcija(ivestys[j]) && ivestys[i]!=ivestys[j])
                        cout<<"Rasta kolizija! "<<ivestys[i]<<" "<<ivestys[j]<<endl;
                }
            }
            break;
        }
        case 7:
        {
            fr<<"6 eksperimentas"<<endl<<endl;
            vector<Statistika>rezultatai;
            vector<long long>histograma(5, 0);
            vector<int>ilgiai={10, 100, 500, 1000};
            Statistika bendri;
            for (int ilgis : ilgiai)
            {
                Statistika s = atlikti6Eksperimenta(ilgis, histograma);

                rezultatai.push_back(s);

                // Sukaupiame bendrus rezultatus
                bendri.bituMin = min(bendri.bituMin, s.bituMin);
                bendri.bituMax = max(bendri.bituMax, s.bituMax);
                bendri.bituSuma += s.bituSuma;

                bendri.hexMin = min(bendri.hexMin, s.hexMin);
                bendri.hexMax = max(bendri.hexMax, s.hexMax);
                bendri.hexSuma += s.hexSuma;

                bendri.kiekis += s.kiekis;
            }
            cout<<"Skiriasi 0-20% bitų: "<<histograma[0]<<endl;
            cout<<"Skiriasi 20-40% bitų: "<<histograma[1]<<endl;
            cout<<"Skiriasi 40-60% bitų: "<<histograma[2]<<endl;
            cout<<"Skiriasi 60-80% bitų: "<<histograma[3]<<endl;
            cout<<"Skiriasi 80-100% bitų: "<<histograma[4]<<endl<<endl;
            cout<<"====================="<<endl;
            cout<<"REZULTATAI PAGAL ILGI"<<endl;
            cout<<"====================="<<endl;
            for(int i=0; i<ilgiai.size(); i++)
            {
                cout<<"Ilgis: "<<ilgiai[i]<<endl;
                cout<<"Bitų skirtumas:"<<endl;
                cout<<"Min: "<<rezultatai[i].bituMin<<"%"<<endl;
                cout<<"Max: "<<rezultatai[i].bituMax<<"%"<<endl;
                cout<<"Vidurkis: "<<rezultatai[i].bituVidurkis()<<"%"<<endl;
                
                cout<<"Hex skirtumas:"<<endl;
                cout<<"Min: "<<rezultatai[i].hexMin<<"%"<<endl;
                cout<<"Max: "<<rezultatai[i].hexMax<<"%"<<endl;
                cout<<"Vidurkis: "<<rezultatai[i].hexVidurkis()<<"%"<<endl;
            }

            cout<<"================="<<endl;
            cout<<"BENDRI REZULTATAI"<<endl;
            cout<<"================="<<endl;

            cout<<"Poru skaicius: "<<bendri.kiekis<<endl;
            cout<<"Bitų skirtumas:"<<endl;
            cout<<"Min: "<<bendri.bituMin<<"%"<<endl;
            cout<<"Max: "<<bendri.bituMax<<"%"<<endl;
            cout<<"Vidurkis: "<<bendri.bituVidurkis()<<"%"<<endl;
            
            cout<<"Hex skirtumas:"<<endl;
            cout<<"Min: "<<bendri.hexMin<<"%"<<endl;
            cout<<"Max: "<<bendri.hexMax<<"%"<<endl;
            cout<<"Vidurkis: "<<bendri.hexVidurkis()<<"%"<<endl;
        }
        case 8:
        {
            vector<string> kandidatai;
            string tikslas="5837";
            for (int i=0; i<10000; i++)
            {
                string s=to_string(i);

                while (s.length()<4)
                    s="0"+s;
                kandidatai.push_back(s);
            }
            string tiksloHash=funkcija(tikslas);
            AtakosRezultatas r=beDruskos(tiksloHash, kandidatai);

            cout<<"---BE DRUSKOS---"<<endl;
            cout<<"----------------"<<endl;
            cout<<"Bandymų skaičius: "<<r.bandymai<<endl;
            cout<<"Bandymų laikas: "<<r.laikas<<" s"<<endl;
            cout<<"Sutapimai: "<<endl;

            if(!r.sutapimai.empty())
            {
                for(int i=0; i<r.sutapimai.size(); i++)
                    cout<<r.sutapimai[i]<<endl;
                cout<<endl;
            }

            string viesaDruska=generuotiSalt();
            tiksloHash=funkcija(tikslas+viesaDruska);
            AtakosRezultatas r1=SuViesaDruska(tiksloHash, viesaDruska, kandidatai);
            cout<<"---SU VIESA DRUSKA---"<<endl;
            cout<<"---------------------"<<endl;
            cout<<"Bandymų skaičius: "<<r1.bandymai<<endl;
            cout<<"Bandymų laikas: "<<r1.laikas<<" s"<<endl;
            cout<<"Sutapimai: "<<endl;

            if(!r1.sutapimai.empty())
            {
                for(int i=0; i<r1.sutapimai.size(); i++)
                    cout<<r1.sutapimai[i]<<endl;
                cout<<endl;
            }
            break;
        }
    }
    
    return 0;
}