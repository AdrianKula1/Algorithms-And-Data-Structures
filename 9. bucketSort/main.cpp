#include <iostream>

using namespace std;

struct Node {//Struktura reprezentująca element w kubełku. Przechowuje wartość (Sumę butelek w pudle) i indeks pudła
    double Value;
    int Index;
    Node() = default;
    Node(double Value, int Index) {
        this->Value = Value;
        this->Index = Index;
    }
};

class List {//Każdy kubełek to lista struktur ^
private:
    int Size = 2;//Wielkość tablicy elementów
    int NoElements=0;//Liczba elementów w kubełku
    Node* Arr = new Node[Size];//Tablica elementów
public:

    void Push(double Value, int index) {//Dodaję element do kubełka
        if (NoElements==Size) {//Jeżeli chcę dodać element ale liczba elemmtów jest równa wielkości tablicy
            Size *= 2;//to zwiększam pojemność tablicy
            Node* NewArr=new Node[Size];//Tworzę nową tablicę
            for (int i = 0; i < NoElements; i++) {//Przepisuję element ze starej tablicy do nowej
                NewArr[i] = Arr[i];
            }
            delete Arr;//kasuję starą tablicę
            Arr = NewArr;//Powiększona tablica staje się tą właściwą

        }
        Arr[NoElements] = {Value, index};//Dodaję nowy elemnt do tablicy
        NoElements++;//Zwiększam liczbę elemntów w tablicy
    }


    void InsertionSort() {//Insertion sort. Sortuję elementy malejąco względem sum pudeł

        for (int i = 1; i < NoElements; i++) {//Dla każdego elementu oprócz tego o indeksie 0
            Node Key = Arr[i];//zapamiętuję go
            int j = i - 1;//Wyznaczam element o indeksie wcześniejszym o 1
            while (j >= 0 && Arr[j].Value < Key.Value) {//Dopóki indeks jest większy równy zero oraz wartośc zapamiętanego elementu jest większa od wartości elemntu o indeksie j
                Arr[j + 1] = Arr[j];//zamieniam elementy ze sobą
                j --;//i przeskakuję element w lewo
            }
            Arr[j + 1] = Key;//Na koniec w puste miejsce zamieniam zapamiętany element
        }
        return;
    }

    int GetNoElements() {//Zwracam liczbę elemntów  w tablicy
        return NoElements;
    }

    Node* GetArr() {//Zwracam tablicę struktur
        return Arr;
    }
};
//N				M
List* BucketSort(double *Sums, int NoBoxes, int NoBottles) {//Bucket sort. W argumentach mam tablicę sum pudeł, liuczbę pudeł, liczbę butelek i liczbę kubełków(do wyjaśnione niżej)
    double F_NoBoxes=(double)NoBoxes, F_NoBottles=(double)NoBottles;//Konwertuję liczbę pudeł i butelek z int na double
    List* Buckets = new List[NoBoxes];//Tworzę tablicę list kubełków

    double BucketCapacity= BucketCapacity = F_NoBottles / F_NoBoxes;//Pojemność kubełka. (Chodzi o przedziały według których będę umieszczał elementy do kubełków). Dziel;ę liczbę butelek w pudlę przez liczbę pudeł
    int Index = 0;
    for (int i = 0; i < NoBoxes; i++) {//Dla każdego elementu w tablicy sum (czyli dla każdej sumy pudła)
        Index = (int)(Sums[i] / BucketCapacity);//wyliczam indeks kubła do którego mam wsadzić daną wartość. Dzielę sumę przez pojemność kubła i liczba całkowita z tego (podłoga?) daje indeks
        //Wrzucam elementy do kubełków względem pojemności kubełków
        if (Index >= NoBoxes) {//Jeżeli indeks jest większy od ilości pudeł to wrzucam element do ostatniego kubełka
            Buckets[NoBoxes - 1].Push(Sums[i], i);
        }else {
            Buckets[Index].Push(Sums[i], i);//Dodaję element do kubełka. Każdy element ma sumę butelek w pudle oraz indeks pudła
        }
    }
    return Buckets;//Zwracam listę kubełków
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int NoBoxes, NoBottles, NoBoxesToShow;
    cin >> NoBoxes >> NoBottles >> NoBoxesToShow;

    double** Boxes = new double* [NoBoxes];
    double* Sums = new double[NoBoxes];
    double Sum = 0;
    for (int i = 0; i < NoBoxes; i++) {
        Boxes[i] = new double[NoBottles];
        Sum = 0;
        for (int j = 0; j < NoBottles; j++) {
            cin >> Boxes[i][j];
            Sum += Boxes[i][j];
        }
        Sums[i] = Sum;
    }
    //^^^^^^^^ Pobieranie danych

    //SORTOWANIE DO KUBEŁKÓW
    List* l1 = BucketSort(Sums, NoBoxes, NoBottles);//Tworzę tablicę list kubełków

    cout << endl << endl;
    for (int i = NoBoxes - 1; i >= 0; i--) {//Dla każdego kubełka od końca

        bool x = 0;
        if (l1[i].GetNoElements() > 1) {//Jeżeli kubełek ma więcej niż 1 element
            l1[i].InsertionSort();//to sortuję je za pomocą insertion sort'a
        }

        if (l1[i].GetNoElements()>0) {//Jeżeli kubełek ma jakieś elementy
            Node* Arr = l1[i].GetArr();//Pobierem tablicę elementów i-tego kubełka

            for (int a = 0; a < l1[i].GetNoElements(); a++) {//Dla każdego a-tego pudła
                for (int b = 0; b < NoBottles; b++) {//Wyświetlam b-tą butelkę
                    cout << Boxes[Arr[a].Index][b] << " ";
                    x = 1;
                }

                if (x) {
                    cout << endl;
                }

                NoBoxesToShow--;//Zmniejszam liczbę podełek do wyświetlenia
                if (NoBoxesToShow == 0) {//Jeżeli wyświetliłem wszystkie wymagane pudełka to kończę program
                    return 0;
                }
            }
        }
    }
}