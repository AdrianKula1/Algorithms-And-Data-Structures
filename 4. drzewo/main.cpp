#include <iostream>
#include <string>

using namespace std;

struct Worker{//Struktura pracownika
    string Name;//Nazwa pracownika
    int MaxNoSubordinates, ActualNoSubordinates=0;//Maksymalna liczba podwładnych i aktualna liczba podwładnych
    Worker *FirstLSon, *RSibling, *LastLSon;//Wskaźniki na lewego syna, prawego brata i najdalszego lewego syna (żeby zaoszczędzić na czasie)
    Worker()=default;

    Worker(int MaxNoSubordinates, string Name){
        this->MaxNoSubordinates=MaxNoSubordinates;
        this->Name=Name;
        this->FirstLSon=NULL;
        this->RSibling=NULL;
        this->LastLSon=NULL;
    }
};

class Tree{
private:
    Worker **WorkersArr;//tablica wskaźników na pracowników (pomaga dodawać pracowników żeby wyszło drzewo zupełne)
    Worker *Root=NULL, *Current=NULL, *CurrentFather=NULL;//Wskaźniki na korzeń, aktualny wskaźnik i jego ojca (pomocne przy dodawaniu przypisaniu który wskaźnik jest najdalszym synem danego ojca)
    int NoWorkers, IdOfLatestWorker;//Liczba pracowników i ID ostatniego pracownika
    int LastFullIndex=0;//Żeby zaoszczędzić czas zapamiętyję indeks pracownika który jako ostatni nie ma wystarczającej liczby podwładnych
public:
    Tree(int NoWorkers, int MaxNoSubordinates, string Name){//Tworzę korzeń drzewa i pierwszego pracownika
        this->NoWorkers=NoWorkers;
        WorkersArr=new Worker*[NoWorkers];
        WorkersArr[0]=Root=Current=new Worker(MaxNoSubordinates, Name);
        IdOfLatestWorker=0;
        return;
    }

    void AddWorker(int MaxNoSubordinates, string Name){//Dodaję pracownika
        IdOfLatestWorker++;
        WorkersArr[IdOfLatestWorker]=new Worker(MaxNoSubordinates, Name);//tworzę pracownika w kolejnym miejscu w tablicy

        for(int i=LastFullIndex; i<NoWorkers; i++){//Przeszukuję listę pracowników w poszukiwaniu pierwszego pracownika który nie ma maksymalnej liczby podwładnych (ma mniej)
            Current=WorkersArr[i];
            if(Current->ActualNoSubordinates<Current->MaxNoSubordinates){//Jeżeli znajdę takiego pracownika
                if(Current->FirstLSon==NULL){//Jeżeli ten pracownik nie ma lewego syna...
                    Current->FirstLSon=WorkersArr[IdOfLatestWorker];//to go tworzę
                    Current->ActualNoSubordinates++;//Zwiększam liczbę podwładnych danego pracownika
                    Current->LastLSon=Current->FirstLSon;//Najdalszy lewy syn = pierwszy lewy syn danego pracownika
                }else{//Jeżeli dany pracownik ma już pierwszego lewego syna
                    CurrentFather=Current;//Zapamiętuję pracownika jako ojciec
                    CurrentFather->ActualNoSubordinates++;//Dany pracownik ma zwiększoną liczbę podwładnych o 1
                    Current=Current->LastLSon;//Przeskakuję do ostatniego syna pracownika
                    Current->RSibling=WorkersArr[IdOfLatestWorker];//Tworzę nowego
                    CurrentFather->LastLSon=Current->RSibling;//Nowy pracownik staje się ostatnim synem ojca
                }
                LastFullIndex=i;//Żeby zaoszczędzić czas zapamiętuję indeks pracownika na którym właśnie wykonywałem operacje (żeby nie sprawdzać znowu od zera)
                return;
            }
        }

    }

    //Najpierw potomkowie potem ojciec
    void PostOrder(Worker *worker){//Wypisuję pracowników w kolejności PostOrder
        if(worker==NULL){//Jeżeli przekazany pracownik to NULL to nic się nie dzieje
            return;
        }
        PostOrder(worker->FirstLSon);//Rekurencyjnie wchodzę do lewego syna
        cout << worker->Name << "\n";//Wyświetlam jego nazwę
        PostOrder(worker->RSibling);//Rekurencyjnie wchodzę do prawego brata
    }

    Worker *GetRoot(){//Pobieram korzeń drzewa
        return Root;
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);//Wymagane do zadania
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    string Name;
    int NoWorkers, MaxNoSubordinates;
    cin >> NoWorkers;
    cin >> MaxNoSubordinates >> Name;
    Tree T1(NoWorkers, MaxNoSubordinates, Name);//Tworzę korzeń drzewa

    for(int i=1; i<NoWorkers; i++){
        cin >> MaxNoSubordinates >> Name;
        T1.AddWorker(MaxNoSubordinates, Name);
    }
    T1.PostOrder(T1.GetRoot());
}