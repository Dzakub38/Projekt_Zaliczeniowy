//4.Program prezentujący klasę Ułamek. Operacje na liczbach całkowitych. (Wykonać operacje pokazujące poprawność działań.)
//program został rozszerzony o różne typ Liczba, pozwala na wykonywanie działań takich jak +,-,*,/ na ułamkach i liczbach zmiennoprzecinkowych (obliczone analitycznie i sprawdzenie)
// oraz na potęgowanie i pierwiastkowanie liczb oraz ułamków, ograniczenie do wyników rzeczywistych


#include <iostream>
#include <string>
#include <cmath>
#include <variant>
#include <memory>


class Skladnik {
public:
//wirtualny destruktor, zabezpiecznie przed wyciekem pamięci 
  virtual ~Skladnik() {}
//wirtualne abstrakcyjne metody
  virtual void wyswietl() const = 0;

  virtual double getWartosc() const = 0;
};


//klasa pochodna Ułamek
class Ulamek : public Skladnik {
protected:

//uwagam, zezwalam na typ double ze względu na pierwiastkowanie ułamka, nie chcę by program starcił na fukcjonalności (np. 5/7 pod pierwiastkiem 2 stopnia)
//przy wprowadzaniu licznika i mianownika ułamka przez uzytkownika, sprawdzam czy wprowadzone liczby są int, założenia autora
  double licznik;
  double mianownik;
  double wartosc;

public:
//konstruktor parametryczny 
  Ulamek(double l, double m) : licznik(l), mianownik(m) {
    if (mianownik == 0) {
      std::cerr << "Blad: Mianownik nie moze byc rowny 0." << std::endl;
      exit(1);
    }
    //rzutowanie by otrzymać liczbę zmiennoprzecinkową
    wartosc = static_cast<double>(licznik) / mianownik;
  }

  virtual ~Ulamek() {}

  //nadpisanie metody
  virtual void wyswietl() const override {
    std::cout << "Ulamek: " << licznik << "/" << mianownik << " = " << wartosc << std::endl;
  }
  
  //metody/gettery pól
    virtual double getWartosc() const override {
        return wartosc;
    }
  
    double getLicznik() const {
        return licznik;
    }

    double getMianownik() const {
        return mianownik;
    }
  
};


//klasa pochodna Liczba 
class Liczba : public Skladnik {
protected:
  double wartosc;

public:
  Liczba(double w) : wartosc(w) {}

  virtual ~Liczba() {}

  virtual void wyswietl() const override {
    std::cout << "Liczba to : " << wartosc << std::endl;
  }

  virtual double getWartosc() const override {
    return wartosc;
  }
};


//klasa Działania
class Dzialania {
public:
//wirtualny destruktor, zabezpiecznie przed wyciekem pamięci
  virtual ~Dzialania() {}
//wirtualna abstrakcyjna metoda
  virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const = 0;
};


//klasa Dodawanie, dziedziczy po Działania
class Dodawanie : public Dzialania {
public:
    virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const override {
        
//sprawdzam czy składniki to ułamki, różne działania dla wariancji        
        const Ulamek* ulamek1 = dynamic_cast<const Ulamek*>(&arg1);
        const Ulamek* ulamek2 = dynamic_cast<const Ulamek*>(&arg2);
        // Dodawanie ułamków, jeżeli ułamek1 oraz ulamek2 należą do klasy Ulamek, to rzutowanie wskaźnika się powiedzie
        if (ulamek1 && ulamek2) {
             //dodawanie ułamków wg zasad matematyki a/b + c/d = (ad+cb)/bd
            int nowyLicznik = ulamek1->getLicznik() * ulamek2->getMianownik() + ulamek2->getLicznik() * ulamek1->getMianownik();
            int nowyMianownik = ulamek1->getMianownik() * ulamek2->getMianownik();
            return new Ulamek(nowyLicznik, nowyMianownik);
        } 
        else {
            // Dodawanie ułamka i liczby a/b + c = (a + cb) / b
            if (ulamek1) {
                return new Ulamek(ulamek1->getLicznik() + arg2.getWartosc() * ulamek1->getMianownik(), ulamek1->getMianownik());
            } 
            // Dodawanie liczby  i ułamka (przemienna kolejność podanych składników)
            else if (ulamek2) {
                return new Ulamek(arg1.getWartosc() * ulamek2->getMianownik() + ulamek2->getLicznik(), ulamek2->getMianownik());
            } 
            else {
                // Dodawanie dwóch liczb, sprawdzanie czy rzutowanie wskaźnika na klasę Liczba sie powiedzie (można pominąć)
                return new Liczba(dynamic_cast<const Liczba*>(&arg1)->getWartosc() + dynamic_cast<const Liczba*>(&arg2)->getWartosc());
            }
        }
    }
};


//klasa Odejmwoanie, calkowicie analogiczna jak Dodawanie

class Odejmowanie : public Dzialania {
public:
    virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const override {
        const Ulamek* ulamek1 = dynamic_cast<const Ulamek*>(&arg1);
        const Ulamek* ulamek2 = dynamic_cast<const Ulamek*>(&arg2);

        if (ulamek1 && ulamek2) {
            // Odejmowanie ułamków
            int nowyLicznik = ulamek1->getLicznik() * ulamek2->getMianownik() - ulamek2->getLicznik() * ulamek1->getMianownik();
            int nowyMianownik = ulamek1->getMianownik() * ulamek2->getMianownik();
            return new Ulamek(nowyLicznik, nowyMianownik);
        } else{
            //Odejmowanie ułamka i liczby 
            if (ulamek1) {
                return new Ulamek(ulamek1->getLicznik() - arg2.getWartosc() * ulamek1->getMianownik(), ulamek1->getMianownik());
            }
            //Odejmowanie liczby  i ułamka 
            else if (ulamek2) {
                return new Ulamek(arg1.getWartosc() * ulamek2->getMianownik() - ulamek2->getLicznik(), ulamek2->getMianownik());
            }
            
            else {
                // Odejmowanie dwóch liczb 
                return new Liczba(dynamic_cast<const Liczba*>(&arg1)->getWartosc() - dynamic_cast<const Liczba*>(&arg2)->getWartosc());
            }
        }
    }
};


//klasa Mnożenie
class Mnozenie : public Dzialania {
public:
    virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const override {
        const Ulamek* ulamek1 = dynamic_cast<const Ulamek*>(&arg1);
        const Ulamek* ulamek2 = dynamic_cast<const Ulamek*>(&arg2);

        if (ulamek1 && ulamek2) {
            // Mnożenie ułamków, a/b * c/d = ac/bd
            int nowyLicznik = ulamek1->getLicznik() * ulamek2->getLicznik();
            int nowyMianownik = ulamek1->getMianownik() * ulamek2->getMianownik();
            return new Ulamek(nowyLicznik, nowyMianownik);
        } else {
            //Mnożenie ułamka i liczby 
            if (ulamek1) {
                return new Ulamek(ulamek1->getLicznik() * arg2.getWartosc(), ulamek1->getMianownik());
            } 
            //Mnożenie liczby i ułamka 
            else if (ulamek2) {
                return new Ulamek(arg1.getWartosc() * ulamek2->getLicznik(), ulamek2->getMianownik());
            } 
            else {
                // Mnożenie dwóch liczb 
                return new Liczba(dynamic_cast<const Liczba*>(&arg1)->getWartosc() * dynamic_cast<const Liczba*>(&arg2)->getWartosc());
            }
        }
    }
};


//klasa Dzielenie ułamków 

class Dzielenie : public Dzialania {
public:
    virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const override {
        const Ulamek* ulamek1 = dynamic_cast<const Ulamek*>(&arg1);
        const Ulamek* ulamek2 = dynamic_cast<const Ulamek*>(&arg2);

        if (ulamek1 && ulamek2) {
            // Dzielenie ułamków
            int nowyLicznik = ulamek1->getLicznik() * ulamek2->getMianownik();
            int nowyMianownik = ulamek1->getMianownik() * ulamek2->getLicznik();
            
            if (nowyMianownik == 0) {
                std::cerr << "Blad: Dzielenie przez zero." << std::endl;
                exit(1);
            }

            return new Ulamek(nowyLicznik, nowyMianownik);
        } else {
            // Dzielenie ułamka i liczby 
            if (ulamek1) {
                int nowyLicznik = ulamek1->getLicznik();
                int nowyMianownik = ulamek1->getMianownik() * arg2.getWartosc();
                
                if (nowyMianownik == 0) {
                    std::cerr << "Blad: Dzielenie przez zero." << std::endl;
                    exit(1);
                }

                return new Ulamek(nowyLicznik, nowyMianownik);
            } else if (ulamek2) 
            {
                int nowyLicznik = arg1.getWartosc() * ulamek2->getMianownik();
                int nowyMianownik = ulamek2->getLicznik();
                
                if (nowyMianownik == 0) {
                    std::cerr << "Blad: Dzielenie przez zero." << std::endl;
                    exit(1);
                }

                return new Ulamek(nowyLicznik, nowyMianownik);
            } else 
            {
                // Dzielenie dwóch liczb 
                if (dynamic_cast<const Liczba*>(&arg2)->getWartosc() == 0) {
                    std::cerr << "Blad: Dzielenie przez zero." << std::endl;
                    exit(1);
                }
                
                return new Liczba(dynamic_cast<const Liczba*>(&arg1)->getWartosc() / dynamic_cast<const Liczba*>(&arg2)->getWartosc());
            }
        }
    }
};

class Potegowanie : public Dzialania {
public:
    virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const override {
        const Ulamek* ulamek1 = dynamic_cast<const Ulamek*>(&arg1);

        if (ulamek1) {
            // Ułamek podniesiony do potęgi
            double potega;
            std::cout << "Podaj potegę (liczba całkowita, nieujemna): ";
            std::cin >> potega;
            
            while(std::cin.fail())
            {
                std::cout << "Wprowadziles bledne dane" << std::endl;
                 
                std::cin.clear();
                std::cin.ignore(1000,'\n');
                 
                std::cout << "Wprowadz dane ponownie:";
                std::cin >> potega;
            }
            
            int a=potega;
            //sprawdzam czy potęga jest dodatnia oraz całkwita, jeżeli double(a)-int(a)=0 to liczba jest intem
            if ( potega < 0 || potega-a!=0) {std::cerr << "Blad: Ujemny wykladnik lub niecałkowity." << std::endl;
                    exit(1);}
            int nowyLicznik = static_cast<int>(std::pow(ulamek1->getLicznik(), potega));
            int nowyMianownik = static_cast<int>(std::pow(ulamek1->getMianownik(), potega));
            return new Ulamek(nowyLicznik, nowyMianownik);
        } else {
            // Liczba podniesiona do potęgi
            double potega;
            std::cout << "Podaj potegę (liczba całkowita, nieujemna): ";
            std::cin >> potega;
            
            while(std::cin.fail())
            {
                std::cout << "Wprowadziles bledne dane" << std::endl;
                 
                std::cin.clear();
                std::cin.ignore(1000,'\n');
                 
                std::cout << "Wprowadz dane ponownie:";
                std::cin >> potega;
            }
            
            int a=potega;
            if ( potega < 0 || potega-a!=0) {std::cerr << "Blad: Ujemny wykladnik lub niecałkowity" << std::endl;
                    exit(1);}
            return new Liczba(static_cast<double>(std::pow(dynamic_cast<const Liczba*>(&arg1)->getWartosc(), potega)));
        }
    }
};



class Pierwiastkowanie : public Dzialania {
public:
    virtual Skladnik* operacja(const Skladnik& arg1, const Skladnik& arg2) const override {
        const Ulamek* ulamek1 = dynamic_cast<const Ulamek*>(&arg1);

        if (ulamek1) {
            // Pierwiastek z ułamka
            double stopien;
            std::cout << "Podaj stopien pierwiastka (liczba całkowita, dodatnia): ";
            std::cin >> stopien;
            
            while(std::cin.fail())
            {
                std::cout << "Wprowadziles bledne dane" << std::endl;
                 
                std::cin.clear();
                std::cin.ignore(1000,'\n');
                 
                std::cout << "Wprowadz dane ponownie:";
                std::cin >> stopien;
            }
            
            int a = stopien;
            //ułamek a/b jest ujemny, gdy a*b < 0
            // jeżeli źle wproadzony został stopień pierwiastka -> error
            if (stopien - a !=0 || stopien <= 0 ) {std::cerr << "Blad: Nieprawidłowy stopień." << std::endl;
                    exit(1);}
            //jeżeli ułamek jest ujemny, a stopień pierwaistka parzysty -> error        
            else if (ulamek1->getLicznik()*ulamek1->getMianownik() < 0 && a%2==0 && stopien > 0)
            {
                std::cerr << "Blad: Pierwiastkowanie liczby ujemnej." << std::endl;
                    exit(1);
            }
            //jeżeli ułamek jest ujemny, a stopień pierwiastka nieparzysty, wyliczam
            else if (ulamek1->getLicznik()*ulamek1->getMianownik() < 0 && a%2==1 && stopien > 0)
            {
                double nowyLicznik = - static_cast<double>(std::pow(abs(ulamek1->getLicznik()), 1.0 / stopien));
                double nowyMianownik = static_cast<double>(std::pow(abs(ulamek1->getMianownik()), 1.0 / stopien));
                return new Ulamek(nowyLicznik, nowyMianownik);
                
            } 
            //jeżeli pierwiastek jest dodatni, wyliczam
            else {
            double nowyLicznik = static_cast<double>(std::pow(abs(ulamek1->getLicznik()), 1.0 / stopien));
            double nowyMianownik = static_cast<double>(std::pow(abs(ulamek1->getMianownik()), 1.0 / stopien));
            return new Ulamek(nowyLicznik, nowyMianownik);
            }
        } else {
            // Pierwiastek z liczby 
            double stopien;
            std::cout << "Podaj stopien pierwiastka (liczba całkowita, dodatnia): ";
            std::cin >> stopien;
            
            while(std::cin.fail())
            {
                std::cout << "Wprowadziles bledne dane" << std::endl;
                 
                std::cin.clear();
                std::cin.ignore(1000,'\n');
                 
                std::cout << "Wprowadz dane ponownie:";
                std::cin >> stopien;
            }
            
            int b = stopien;
            //sprawdzam czy stopien pierwiastka jest całkowity, dodatni
            if ( stopien <= 0 || stopien-b!=0) 
            {
                std::cerr << "Blad: Nieprawidłowy stopień." << std::endl;
                exit(1);
            }
            //jeżeli liczba jest ujemna, a stopień pierwiastka nieparzysty, wyliczam        
            if (dynamic_cast<const Liczba*>(&arg1)->getWartosc() < 0 && b%2==1)
            {
                return new Liczba(-static_cast<double>(std::pow(abs(dynamic_cast<const Liczba*>(&arg1)->getWartosc()), 1.0 / stopien)));
            }
            //jeżeli liczba jest dodatnia, a stopień całkowity, dodatni, wyliczam
            else 
            {
                return new Liczba(static_cast<double>(std::pow(dynamic_cast<const Liczba*>(&arg1)->getWartosc(), 1.0 / stopien)));
            }
        }
    }
};

//unique_ptr zarzadzaniem pamięcią, pozbycie się delete, zabezpieczenie wycieku pamięci
std::unique_ptr<Skladnik> pobierzLiczbe() {
  std::cout << "Wybierz typ liczby (u - ulamek, l - liczba ): ";
  char wybor;
  std::cin >> wybor;

  if (wybor != 'u' && wybor != 'l') {
    std::cerr << "Błąd: Nieprawidłowy wybór typu liczby." << std::endl;
    exit(1);
  }

  if (wybor == 'u') {
    double licznik, mianownik;
    std::cout << "Podaj licznik ułamka (liczba całkowita): ";
    std::cin >> licznik;
    
    while(std::cin.fail())
    {
        std::cout << "Wprowadziles bledne dane" << std::endl;
         
        std::cin.clear();
        std::cin.ignore(1000,'\n');
         
        std::cout << "Wprowadz dane ponownie:";
        std::cin >> licznik;
    }
    
    int a=licznik;
    //sprawdzam czy licznyik jest całkowity
    if ( licznik-a!=0) {
        std::cerr << "Blad: To nie jest liczba całkowita." << std::endl;
        exit(1);
        
    }
    std::cout << "Podaj mianownik ułamka (nie moze być równy 0, liczba całkowita): ";
    std::cin >> mianownik;
    
    while(std::cin.fail())
    {
        std::cout << "Wprowadziles bledne dane" << std::endl;
         
        std::cin.clear();
        std::cin.ignore(1000,'\n');
         
        std::cout << "Wprowadz dane ponownie:";
        std::cin >> mianownik;
    }
    
    int b = mianownik;
    //sprawdzam czy mianownik jest całkowity
    if ( mianownik-b!=0) {std::cerr << "Blad: To nie jest liczba całkowita." << std::endl;
                    exit(1);}
    std::cout << "Podany ułamek to: " << licznik << "/" << mianownik << std::endl;
    return std::make_unique<Ulamek>(licznik, mianownik);
    } 
    
    else {
    double wartosc;
    std::cout << "Podaj wartość liczby: ";
    std::cin >> wartosc;
    while(std::cin.fail())
    {
        std::cout << "Wprowadziles bledne dane" << std::endl;
         
        std::cin.clear();
        std::cin.ignore(1000,'\n');
         
        std::cout << "Wprowadz dane ponownie:";
        std::cin >> wartosc;
    }
    std::cout << "Podana liczba to: " << wartosc << std::endl;
    return std::make_unique<Liczba>(wartosc);
  }
}


//variant przetrzymujący rodzaje działań, urozmaicenie kodu w STL
using rodzaj_dzialania = std::variant<Dodawanie, Odejmowanie, Mnozenie, Dzielenie, Potegowanie, Pierwiastkowanie>;




int main() {
    
  rodzaj_dzialania dzialanie;

  // Wybierz operację
  std::cout << "Wybierz operację (+, -, *, /, ^, p): ";
  char operacja;
  std::cin >> operacja;

//wybór operacji przez użytkownika i stworzenie odpowiedniej klasy  
  switch (operacja) {
  case '+':
    dzialanie = Dodawanie();
    break;
  case '-':
    dzialanie = Odejmowanie();
    break;
  case '*':
    dzialanie = Mnozenie();
    break;
  case '/':
    dzialanie = Dzielenie();
    break;
  case '^':
    dzialanie = Potegowanie();
    break;
  case 'p':
    dzialanie = Pierwiastkowanie();
    break;
  default:
    std::cerr << "Błąd: Nieznana operacja." << std::endl;
    return 1;
  }
   //utworzenie pierwszego składnika wybranego przez użytkownika typu/klasy (Ulamek, Liczba)  
   std::unique_ptr<Skladnik> operand1 = pobierzLiczbe();

   std::unique_ptr<Skladnik> operand2 = nullptr;
//pobranie drugiej liczby w wypadku gdy operacja wybrana przez użytkownika tego wymaga, w przeciwnym razie, pobranie potęgi/stopnia   
   if (operacja == '^' || operacja == 'p') {
        // Potęgowanie lub pierwiastkowanie - pobierz tylko jedną liczbę
        ;
    } else {
        operand2 = pobierzLiczbe();
        operand2->wyswietl();
    }
  //użycie wizytatora na funkcji lambda, zmienne z zakresu funkcji zewnętrznej są dostępne w funkcji lambda [&]
  Skladnik* wynik = std::visit([&](const auto& arg) { return arg.operacja(*operand1, *operand2); }, dzialanie);

  // Wyświetl wynik
  std::cout << "Wynik działania to: " ;
  wynik->wyswietl();
  
  //urozmaicenie kodu o x.index() i skorzystanie z variantów, wyliczenie "ręcznie" wartości operacji dla sprawdzenia poprawności działań +,-,*,/
  switch (dzialanie.index()) {
  case 0:
    std::cout << "Wynik dodawania, sprawdzenie: " << operand1->getWartosc()+operand2->getWartosc() <<  std::endl; 
    break;
  case 1:
    std::cout << "Wynik odejmowania, sprawdzenie: " << operand1->getWartosc()-operand2->getWartosc() <<  std::endl;
    break;
  case 2:
    std::cout << "Wynik mnożenia, sprawdzenie: " << operand1->getWartosc()*operand2->getWartosc() <<  std::endl;
    break;
  case 3:
    std::cout << "Wynik dzielenia, sprawdzenie: " << operand1->getWartosc()/operand2->getWartosc() <<  std::endl;
    break;
  
  }
//zarządzanie pamięcią stowrzonego wcześniej obiektu  
  delete wynik;
  
}
