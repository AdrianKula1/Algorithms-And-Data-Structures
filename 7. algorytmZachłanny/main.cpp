#include <iostream>
#include <algorithm>

using namespace std;

int Compare(pair<int, int> a, pair<int, int> b) {
    return a.second < b.second;
}

bool CanPlaceCameras() {
    int RoomSize;//Rozmiar pokoju
    pair<int, int>* PossibleCameraAreas;//Tablica obszarów w których można umieśić kamery (Jedna para = w i-tej kolumnie można umieścić kamery w przędzie od first do second)
    bool* RowTaken;//Tablica sprawdzająca czy dany rząd ma już kamerę

    cin >> RoomSize;//Pobieram rozmiar pokoju

    RowTaken = new bool[RoomSize];
    for (int i = 0; i < RoomSize; i++) {
        RowTaken[i] = false;
    }
    PossibleCameraAreas = new pair<int, int>[RoomSize];//Tablica obszarów których mogę umieścić kamery - kolumny

    for (int i = 0; i < RoomSize; i++) {//Pobieram dane do tablicy - w których częściach konkretnych kolumn można zamontować kamerę
        cin >> PossibleCameraAreas[i].first >> PossibleCameraAreas[i].second;
    }
    //Ostatni rząd
    sort(PossibleCameraAreas, PossibleCameraAreas + RoomSize, Compare);//Sortuję kolumny rosnąco względem ostatniego rzędu - czyli second. {1, 1}, {1, 9}...

    bool CanBePlacedInThisColumn = false;
    for (int i = 0; i < RoomSize; i++) {//Dla każdej kolumny sprawdzam czy można w niej umieścić kamerę
        for (int x = PossibleCameraAreas[i].first; x <= PossibleCameraAreas[i].second; x++){//Jeżeli w obszarze w którym możliwe jest umieszczenie kamery...
            if (RowTaken[x] == false) {//..i w danym wierszu nie ma jeszcze kamery
                RowTaken[x] = true;//to umieszczam tam kamerę
                break;//i kończę pętlę
            }
        }
    }

    for (int i = 0; i < RoomSize; i++) {//Sprawdzam czy w każdym wierszu (i kolumnie) są kamery
        if (!RowTaken[i]) {//Jeżeli w którymś nie ma to zwracam false
            return false;
        }
    }
    return true;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int NoRooms;//Pobieram liczbę pokoi
    cin >> NoRooms;

    for (int i = 0; i < NoRooms; i++) {

        if (CanPlaceCameras()){//Jeżeli znajdę odpowiednie ustawienie kamer w danym pokoju to wypisuję "TAK", jak nie to "NIE"
            cout << "TAK" << endl;
        }else {
            cout << "NIE" << endl;
        }
    }

    return 0;
}