#include <iostream>

using namespace std;

void Sort(int *Arr, int Min, int Mid, int Max, long long int &Result){//Przekazuję tablicę, indeksy początkowy, środkowy i ostatni danej części tablicy oraz licznik wy,magany w  zadaniu
    int LArrSize=Mid-Min+1, RArrSize=Max-Mid;//Wyliczam wielkości tymczasowych tablic
    int LArr[LArrSize], RArr[RArrSize];//Tworzę dwie tymczasowe tablice

    //Kopiuję dane z przekazanej tablicy do tablic tymczasowych (od min do mid) i (od mid+1 do max)
    for(int i=0; i<LArrSize; i++){
        LArr[i]=Arr[Min+i];
    }

    for(int i=0; i<RArrSize; i++){
        RArr[i]=Arr[Mid+1+i];
    }

    int LArrIndex=0, RArrIndex=0, SortedArrIndex=Min;
    while(LArrIndex<LArrSize && RArrIndex<RArrSize){//Sprawdzam obie tablice i mniejszy element przerzucam do tablicy przekazanej w argumencie
        if(LArr[LArrIndex]<=RArr[RArrIndex]){//Jeżeli mniejszy element jest w lewej tablicy to przerzucam go i zwiększam ideks lewej tablicy
            Arr[SortedArrIndex]=LArr[LArrIndex];
            LArrIndex++;
        }else{//Jeżeli element po prawej jest mniejszy od elementu po lewej to przepisuję ten element do posortowanej tablicy i zwiększam indeksy

            Result+=(LArrSize-LArrIndex);//Zwiększam wynik o liczbę elementów które są w lewej tablicy

            Arr[SortedArrIndex]=RArr[RArrIndex];
            RArrIndex++;
        }
        SortedArrIndex++;//zwiększam index przekazanej tablicy
    }

    //Jeżeli jedna z tablic się skończyła to przepisuję po prostu wszytskie elementy z drugiej do przekazanej
    while(LArrIndex<LArrSize){
        Arr[SortedArrIndex]=LArr[LArrIndex];
        SortedArrIndex++;
        LArrIndex++;
    }
    while(RArrIndex<RArrSize){
        Arr[SortedArrIndex]=RArr[RArrIndex];
        SortedArrIndex++;
        RArrIndex++;
    }
}

void MergeSort(int *Arr, int Min, int Max, long long int &Result){//Rozbijam tablicę na mniejsze czyli schodzę w dół (MergeSort())- a na koniec je scalam czyli idę w górę (Sort())
    if(Min>=Max){//Jeżeli minimalny indeks jest większy od maksymalnego to kończę funkcję żeby nie wchodzić w nieskończoność
        return;
    }
    int Mid=(Min+Max)/2;//Wyliczam środek tablicy
    MergeSort(Arr, Min, Mid, Result);//Od lewej strony do połowy
    MergeSort(Arr, Mid+1, Max, Result);//Od połowy do prawej strony
    Sort(Arr, Min, Mid, Max, Result);//Wracając do góry sortuję części tablicy
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int Size;
    cin >> Size;
    int Arr[Size];
    for(int i=0; i<Size; i++){//Pobierm dane
        cin >> Arr[i];
    }

    long long int Result=0;
    MergeSort(Arr, 0, Size-1, Result);

    cout << Result << endl;

}