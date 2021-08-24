#include <iostream>

using namespace std;

bool solve(){
    int Number, NoAmino, *Arr;
    int CounterSekw=0, CounterZb=0; //Jeżeli po pobraniu danych countery nie są równe to jest to anagram
    cin >> NoAmino;
    Arr=new int[NoAmino];
    //Tworzę tablicę o wielkości rówej liczbie aminokwasów. i tablica  pokazuje ile razy dana liczba znajduje się w sekwencji
    for(int i=0; i<NoAmino; i++){
        Arr[i]=0;
    }

    //Pobieram sekwencję i tablica  pokazuje ile razy dana liczba znajduje się w sekwencji
    while(1){
        cin >> Number;
        if(Number==-1){
            break;
        }
        CounterSekw++;
        Arr[Number]++;
    }

    //Pobieram zbiór przy czym każda liczba odejmuje 1 w tablicy
    while(1){
        cin >> Number;
        if(Number==-1){
            break;
        }
        CounterZb++;

        Arr[Number]--;

    }

    //Jeżeli zbiór lub sekwencja ma więcej cyfr to nie jest to anagram
    if(CounterSekw!=CounterZb){
        return false;
    }
    //Sprawdzam czy wszystkie elementy w tablicy (zliczającej cyfry) są równe 0. Jeżeli tak to jest to anagram jeżeli nie - to nie ponieważ np. może być więcej 2 w zbiorze niż sekwencji
    for(int i=0; i<NoAmino; i++){
        if(Arr[i]!=0){
            return false;
        }
    }

    delete Arr;
    //Podane ciągi to anagram
    return true;
}
int main(){
    ios_base::sync_with_stdio(false);

    int Sets;
    cin >> Sets;
    //Zestawy danych
    for(int i=0; i<Sets; i++){
        if(solve()){
            cout << "TAK" << endl;
        }else{
            cout << "NIE" << endl;
        }


    }
}
