#include <iostream>
#include <string>

using namespace std;

/*Zaczynając od początku tablicy przechodzę do prawej strony w poszukiwaniu największego elementu. Gdy go znajdę zamieniam ten element z pierwszym.
Potem znowu szukam drugiego największego elementu i zamieniam go z drugim elementem
itd..



---
Podczas porządkowania względem oznaczeń w przypadku tych samych oznaczeń o kolejnosci decyduje wiek próbki. -- Wcześniejsze leksykograficznie jest wypisywane wcześniej
Podczas porządkowania względem wieku próbki w przypadku tych samych czasów pobrania o kolejnosci decyduje oznaczenie. -- Większa liczba jest wypisywana wcześniej
Dla tych samych oznaczeń i czasu pobrania algorytm ma zachować się stabilnie.
---
*/

struct Sample{//Struktura reprezentująca próbkę. Poziada nazwę, czas pobrania i ID oznaczające oryginalne miejsce w tablicy
    string Name;
    int SamplingTime;
    int SampleID;
    Sample()=default;
};

//Sortowanie przez wybieranie
void SelectionSortNames(Sample* Samples, int size){
    int MaxIndex;
    int LowestID=2147483647;
    bool First=true;

    for(int i=0; i<size-1; i++){//Przeglądam całą tablicę
        MaxIndex=i;//Zakładam że pierwszy element tablicy (tutaj względem nazwy) jest największy  - zapamiętuję indeks
        for(int j=i+1; j<size; j++){//Przechodzę całą tablicę od i-tego+1 miejsca
            //Sortuję względem nazwy(rosnąco), czasu(malejąco) i względem ID(rosnąco)
            //Jeżeli znajdę nazwę próbki która jest wcześniej leksykograficznie LUB znajdę próbkę o tej samej nazwie ale z mniejszym czasem pobrania LUB znajdę próbkę o tej samej nazwie  i czasie pobrania ale ta próbka będzie wcześniej w oryginalnej tablicy (ma mniejsze ID=wcześniej w tablicy)
            if(Samples[j].Name<Samples[MaxIndex].Name || (Samples[j].Name==Samples[MaxIndex].Name && Samples[j].SamplingTime>Samples[MaxIndex].SamplingTime) || (Samples[j].Name==Samples[MaxIndex].Name && Samples[j].SamplingTime==Samples[MaxIndex].SamplingTime && Samples[j].SampleID<LowestID)){

                //to ta probówka staje się "największą"
                MaxIndex=j;

                //Zapamiętuję ID próbki (z oryginalnej tablicy)
                LowestID=Samples[j].SampleID;

            }

            //Powtarzam
        }
        //Zamieniam znalezioną próbkę z kolejno pierwszym, drigim itd.. elementem
        swap(Samples[MaxIndex], Samples[i]);
    }
}

void SelectionSortDigits(Sample* Samples, int size){
    int MaxIndex;
    int LowestID=2147483647;
    bool First=true;
    for(int i=0; i<size-1; i++){
        MaxIndex=i;
        LowestID=2147483647;
        for(int j=i+1; j<size; j++){
            //Sortuję względem czasu(malejąco), nazwy(rosnąco) i względem ID(rosnąco)
            //Jeżeli znajdę większy czas pobrania próbki  LUB znajdę próbkę o tym samym czasie pobrania próbki ale z nzawą wcześniej leksykograficznie LUB znajdę próbkę o tej samej nazwie  i czasie pobrania ale ta próbka będzie wcześniej w oryginalnej tablicy (ma mniejsze ID=wcześniej w tablicy)
            if((Samples[j].SamplingTime>Samples[MaxIndex].SamplingTime) || (Samples[j].SamplingTime==Samples[MaxIndex].SamplingTime && Samples[j].Name<Samples[MaxIndex].Name) || (Samples[j].SamplingTime==Samples[MaxIndex].SamplingTime && Samples[j].Name==Samples[MaxIndex].Name && Samples[j].SampleID<LowestID)){
                MaxIndex=j;
                LowestID=Samples[j].SampleID;
            }


        }
        swap(Samples[MaxIndex], Samples[i]);
    }
}



void Solve(){

    int NoSamples;
    cin >> NoSamples;

    Sample *Samples=new Sample[NoSamples];

    string Name;
    int SamplingTime;
    for(int i=0; i<NoSamples; i++){//Pobieram tablicę próbek
        cin >> Name >> SamplingTime;
        Samples[i].Name=Name;
        Samples[i].SamplingTime=SamplingTime;
        Samples[i].SampleID=i;
    }

    bool HowToSort;
    cin >> HowToSort;

    if(HowToSort==0){
        SelectionSortNames(Samples, NoSamples);//Sortowanie względem imion
    }else{
        SelectionSortDigits(Samples, NoSamples);//Sortowanie względem czasów pobrania
    }

    for(int i=0; i<NoSamples; i++){//Wypisuję tablicę
        cout << Samples[i].Name << "-" << Samples[i].SamplingTime << " ";
    }
}



int main(){
    ios_base::sync_with_stdio(false);

    int NoSets;
    cin >> NoSets;//Pobieram liczbę zestawów
    for(int i=0; i<NoSets; i++){
        Solve();
    }
}