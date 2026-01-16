#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

class Prostokat{
    private:
        int width;
        int height;

    public:
        Prostokat(int w, int h) : width(w), height(h){}

        int pole() const{
            return width*height;
        }

        int obwod() const{
            return width*2+height*2;
        }
};
void prostokot(){
    Prostokat p1(25, 5);
    std::cout<<p1.pole()<<' '<<p1.obwod();
}

class Licznik{
    private:
        int count;
    public:
        Licznik(int start=0) : count(start) {}

        void zwieksz(){
            count++;
        }
        
        void zmniejsz(){
            if(count>0) count--;
        }

        void reset(){
            count=0;
        }

        int pokaz() const{
            return count;
        }
};
void licznik(){
    Licznik l(23);
    l.zwieksz();
    l.zwieksz();
    l.zwieksz();
    l.zmniejsz();
    std::cout<<"Warto˜†: "<<l.pokaz()<<'\n';
    l.reset();
    std::cout<<"Warto˜† po resecie: "<<l.pokaz()<<'\n';
}

class Bankomat{
    private:
        double saldo;
    public:
        Bankomat() : saldo(0) {}

        void wplac(double kwota){
            if(kwota>0) saldo+=kwota;
            else std::cout<<"Za maˆo kasy!\n";
        }

        bool wyplac(double kwota){
            if(kwota<=saldo){
                saldo-=kwota;
                return true;
            }else return false;
        }

        double pokazSaldo() const{
            return saldo;
        }
};
void bankomat(){
    Bankomat b;
    b.wplac(250);
    b.wyplac(50);
    b.wyplac(300);
    std::cout<<b.pokazSaldo()<<'\n';
    b.wplac(-25);
    b.wplac(2137.69);
    std::cout<<b.pokazSaldo()<<'\n';
}

class Zwierze{
    protected:
        std::string imie;
    public:
        Zwierze(const std::string& i): imie(i){}
        virtual void dajGlos() const{
                std::cout<<"???";
            }
        virtual ~Zwierze() {}
};
class Pies: public Zwierze{
    public:
        Pies(const std::string& i): Zwierze(i) {}
        void dajGlos() const override{
            std::cout <<"Hau! Jestem "<<imie<<"\n";
        }
};
class Kot: public Zwierze{
    public:
        Kot(const std::string& i): Zwierze(i) {}
        void dajGlos() const override{
            std::cout<<"Miau! Jestem "<<imie<<'\n';
        }
};
void zoo(){
    Zwierze* zwierzaki[2];
    zwierzaki[0] = new Pies("Burek");
    zwierzaki[1] = new Kot("Mruczek");

    for(int i=0; i<2; i++) zwierzaki[i]->dajGlos();
    for(int i=0; i<2; i++) delete zwierzaki[i];
}

class Czas{
    private:
        int godzina;
        int minuta;
    public:
        Czas(int g, int m): godzina(g), minuta(m) {}
        
        void pokaz() const{
            if(godzina<10) std::cout<<'0';
            std::cout<<godzina<<':';
            if(minuta<10) std::cout<<'0';
            std::cout<<minuta<<'\n';
        }

        void dodajMinuty(int x){
            int total = godzina*60+minuta+x;
            godzina = (total/60)%24;
            minuta = total % 60;
        }

        bool wczesniejNiz(const Czas& inny) const{
            int now = godzina*60+minuta;
            int diffrent = inny.godzina*60+inny.minuta;

            return now<diffrent;
        }

        bool rowny(const Czas& inny) const{
            int now = godzina*60+minuta;
            int diffrent = inny.godzina*60+inny.minuta;

            return now==diffrent;
        }

        // wczesniej niz - alternative
        bool operator<(const Czas& inny) const{
            return(godzina*60 + minuta)<(inny.godzina*60 + inny.minuta);
        }

        // pozniej niz
        bool operator>(const Czas& inny) const{
            return(godzina*60 + minuta)>(inny.godzina*60 + inny.minuta);
        }

        // rowny() - alternative
        bool operator==(const Czas& inny) const{
            return(godzina*60 + minuta)==(inny.godzina*60 + inny.minuta);
        }

};
class Zegar{
    private:
        std::string nazwa;
        Czas czas;
    public:
        Zegar(std::string n, int g, int m): nazwa(n), czas(g, m) {}
        void pokaz() const{
            std::cout<<nazwa<<' ';
            czas.pokaz();
        }

        void dodajMinuty(int x){
            czas.dodajMinuty(x);
        }

        bool wczesniejNiz(const Zegar& inny) const{
            return czas.wczesniejNiz(inny.czas);
        }

        bool operator<(const Zegar& inny) const{
            return czas<inny.czas;
        }

        bool operator>(const Zegar& inny) const{
            return czas>inny.czas;
        }

        bool operator==(const Zegar& inny) const{
            return czas==inny.czas;
        }


};
void time(){
    Zegar budzik("Budzik", 6, 45);
    Zegar piekarnik("Piekarnik", 13, 20);
    Zegar zegar("Zegar testowy", 23, 50);

    Zegar d("Z1", 5, 30);
    Zegar e("Z2", 6, 15);

    Czas a(10, 30);
    Czas b(12, 15);
    Czas c(10, 30);

    budzik.pokaz();
    piekarnik.pokaz();
    
    zegar.pokaz();
    zegar.dodajMinuty(20);
    zegar.pokaz();

    std::cout<<"U¾ywaj¥c napisanych funkcji:\n";
    std::cout<<a.wczesniejNiz(b)<<'\n';
    std::cout<<b.wczesniejNiz(a)<<'\n';
    std::cout<<a.rowny(c)<<'\n';

    std::cout<<"U¾ywaj¥c przeci¥¾enia operatora:\n";
    std::cout<<(a<b)<<'\n';
    std::cout<<(b<a)<<'\n';
    std::cout<<(a==b)<<'\n';

    std::cout<<d.wczesniejNiz(e)<<'\n';

    std::vector<Zegar> zegary;

    zegary.push_back(Zegar("Budzik", 6, 45));
    zegary.push_back(Zegar("Zegar testowy", 23, 50));
    zegary.push_back(Zegar("Piekarnik", 13, 20));

    std::sort(zegary.begin(), zegary.end());

    for(const auto& z: zegary) z.pokaz();
}

class Film{
    protected:
        std::string title;
        std::string autor;
        int duration;
    public:
        Film(std::string t, std::string a, int d): title(t), autor(a), duration(d) {}

        void pokaz() const{
            std::cout<<"Tytuˆ: "<<title<<", Re¾yser: "<<autor<<", Czas: "<<duration<<" min\n";
        }

        bool operator<(const Film& inny) const{
            return duration<inny.duration;
        }

};
class BibliotekaFilmow{
    private:
        std::vector<Film> film;
    public:

        void dodajFilm(std::string title, std::string autor, int duration){
            film.push_back(Film(title, autor, duration));
        }

        void pokazWszystko() const{
            for(const auto& f : film) f.pokaz();
        }

        void posortujPoCzasie(){
            std::sort(film.begin(), film.end());
        }

};
void movie(){
    BibliotekaFilmow bf;

    bf.dodajFilm("Harry Potter", "JK Rowling", 236);
    bf.dodajFilm("Kamienie na szaniec", "Rudy", -1);
    bf.dodajFilm("Historia upadku ChataGPT", "Gemini", 12);

    bf.pokazWszystko();
    bf.posortujPoCzasie();
    std::cout<<"\nPo posortowaniu:\n\n";
    bf.pokazWszystko();
}

class Ksiazka{
    protected:
        std::string tytul;
        std::string autor;
        int liczbaStron;
    public:
        Ksiazka(std::string t, std::string a, int lS): tytul(t), autor(a), liczbaStron(lS) {}

        void pokaz() const{
            std::cout<<"Tytuˆ: "<<tytul<<" - Autor: "<<autor<<" - liczba stron: "<<liczbaStron<<'\n';
        }

        std::string zwrocAutor() const{
            return autor;
        }

        bool operator<(const Ksiazka& inny) const{
            return liczbaStron<inny.liczbaStron;
        }
        
        bool operator==(const std::string inny) const{
            return tytul==inny;
        }

        friend std::ostream& operator<<(std::ostream& os, const Ksiazka& k){
            os<<"Tytuˆ: "<<k.tytul<<" - Autor: "<<k.autor<<" - liczba stron: "<<k.liczbaStron;
            return os;
        }

};
class Biblioteka{
    private:
        std::vector<Ksiazka> zbior;
    public:
        void dodajKsiazke(std::string tytul, std::string autor, int liczbaStron){
            zbior.push_back(Ksiazka(tytul, autor, liczbaStron));    
        }

        void pokazWszystko() const{
            for(const auto& z: zbior) z.pokaz();
        }

        void posortujPoStronach(){
            std::sort(zbior.begin(), zbior.end());
        }

        void usunKsiazke(const std::string& tytul){
            zbior.erase(
                std::remove_if(zbior.begin(), zbior.end(), 
                    [&tytul](const Ksiazka& k){return k == tytul;}), zbior.end()
            );
        }

        void znajdzPoAutorze(const std::string& autor) const{
            for(const auto& z: zbior)
                if (z.zwrocAutor() == autor)
                    z.pokaz();
        }


};
void bookTest(){
    Biblioteka library;

    library.dodajKsiazke("The estate developer", "Lloyd Frontiera", 230);
    library.dodajKsiazke("Solo leveling", "Baka Baka", 201);
    library.dodajKsiazke("Miˆo˜† w Zakopanem", "Zenek Martyniuk", 59);
    library.dodajKsiazke("Maryla Kapan", "Zenek Martyniuk", 23);
    library.dodajKsiazke("Dlaczego nie warto ˜wi©towa† przed wcze˜nie.", "Rafaˆ Trzaskowski", 2);
    library.dodajKsiazke("Jedzymy razem Price Polo.", "Rafaˆ Trzaskowski", 35);
    library.dodajKsiazke("To pobyˆam dam¥ ile? P¢ˆ godziny.", "½ona Rafaˆa Trzaskowski", 59);

    library.pokazWszystko();
    library.posortujPoStronach();
    std::cout<<"\nPosortowane wzgl©dem stron:\n\n";
    library.pokazWszystko();

    std::cout<<"\n Usuni©cie rekordu:\n\n";
    library.usunKsiazke("Solo leveling");
    library.pokazWszystko();

    std::cout<<"\nSzukamy Rafaˆa:\n\n";

    library.znajdzPoAutorze("Rafaˆ Trzaskowski");

}

class DzieloSztuki{
    protected:
        std::string tytul;
        std::string autor;
    public:
        DzieloSztuki(std::string t, std::string a) : tytul(t), autor(a) {}
        virtual void pokaz() const = 0;
        virtual ~DzieloSztuki() {}

        std::string zwrocAutor() const{
            return autor;
        }
};
class Obraz: public DzieloSztuki{
    protected:
        std::string technika;
    public:
        Obraz(std::string t, std::string a, std::string th): DzieloSztuki(t, a), technika(th) {}

        void pokaz() const override{
            std::cout<<"Obraz: \""<<tytul<<"\" autor: "<<autor<<", technika: "<<technika<<'\n';
        }

};
class Rzezba: public DzieloSztuki{
    protected:
        std::string material;
    public:
        Rzezba(std::string t, std::string a, std::string m): DzieloSztuki(t, a), material(m) {}

        void pokaz() const override{
            std::cout<<"Rze«ba: \""<<tytul<<"\" autor: "<<autor<<", amteriaˆ: "<<material<<'\n';
        }

};
class Instalacja: public DzieloSztuki{
    protected:
        bool czyWideo;
    public:
        Instalacja(std::string t, std::string a, bool c): DzieloSztuki(t, a), czyWideo(c) {}

        void pokaz() const override{
            std::cout<<"Instalacja: \""<<tytul<<"\" autor: "<<autor<<", zawiera wideo: "<<(czyWideo ? "TAK" : "NIE")<<'\n';
        }

};
class Galeria{
    private:
        std::vector<std::shared_ptr<DzieloSztuki>> kolekcja;
    public:
        void dodaj(std::shared_ptr<DzieloSztuki> dzielo){
            kolekcja.push_back(dzielo);
        }

        void pokazWszystko() const{
            for(const auto& k:kolekcja) k->pokaz();
        }

        void znajdPoAtuorze(const std::string& a) const{
            for(const auto& k: kolekcja)
                if (k->zwrocAutor() == a)
                    k->pokaz();
        }

};
void virtualGalery(){
    Galeria galeria;

    galeria.dodaj(std::make_shared<Obraz>("Mona Lisa", "Leonardo", "olej"));
    galeria.dodaj(std::make_shared<Rzezba>("Dawid", "Michelangelo", "marmur"));
    galeria.dodaj(std::make_shared<Instalacja>("TV Noise", "Nam June Paik", true));

    galeria.pokazWszystko();

    std::cout << "\nDzieˆa Leonarda:\n";
    galeria.znajdPoAtuorze("Leonardo");
}

int main(){
    //prostokot();
    //bankomat();
    //zoo();
    //time();
    //movie();
    //bookTest();
    virtualGalery();

    return 0;
}