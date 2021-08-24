#include <iostream>
#include <climits>

using namespace std;

class Backtracking {
private:
    int RoomSize, NoCameras;//Wielkość pokoju i liczba kamer - to samo
    int** Room, *SolutionArr;//Pokój przedstawiuony jako macierz i tablica wynikowa
    int CurrentSum=0, MinimumSum=INT_MAX;//Aktualna suma części pokoju i minimalna suma
    bool* RowTaken;//Tablica zajętych wierszy
public:
    Backtracking() {//Pobieram dane
        cin >> RoomSize;
        NoCameras = RoomSize;
        Room = new int* [RoomSize];
        RowTaken = new bool[RoomSize];
        for (int i = 0; i < RoomSize; i++) {
            Room[i] = new int[RoomSize];
            RowTaken[i] = false;
        }

        for (int i = 0; i < RoomSize; i++) {
            for (int j = 0; j < RoomSize; j++) {
                cin >> Room[i][j];
            }
        }

        SolutionArr = new int[NoCameras];

    }

    void CopyArr(int *CurrentArr) {//Kopiuję CurrentArr do SolutionArr (obie tablice są wskaźnikami więc jak dam SolutionArr=CurrentArr to obie będą się zmieniać tak samo)
        int* Arr1 = CurrentArr, *Arr2 = SolutionArr;
        for (int i = 0; i < NoCameras; i++) {
            *(Arr2 + i) = *(Arr1 + i);
        }

    }

    void Solution(int CurrentColumn, int *CurrentArr) {//W argumentach mam kolumnę w której jestem aktualnie (zaczynając od zera) i tablicę zawierającą numery wierszy w których mam umieszczone kamery
        if (CurrentColumn >= RoomSize) {//Jeżeli kolumna wychodzi poza zakres to wracam
            return;
        }
        for (int i = 0; i < RoomSize; i++) {//Przechodzę po wszystkich wierszach pokoju
            if (RowTaken[i] == false) {//Jeżeli w danym wierszu nie ma jeszcze kamery
                RowTaken[i] = true;//to ją umieszczam
                CurrentArr[CurrentColumn] = i;//zapisuję numer wiersza do tablicy przechowującej aktualne (półpełne) dane
                CurrentSum += Room[i][CurrentColumn];//Aktualna suma to CurrentSum+=wartość danej kratki w pokoju[wiersz][kolumna]
                if (CurrentSum > MinimumSum) {//Jeżeli aktualna suma jest większa niż minimalna suma to nie ma co dalej szukać rozwiązania bo lepsze nie będzie
                    RowTaken[i] = false;//Zdejmuję kamerę z danego wersza
                    CurrentSum -= Room[i][CurrentColumn];//Od sumy odejmuję wartość kratki w pokoju
                    CurrentArr[CurrentColumn] = -1;//Zeruję wiersz w którym zdjąłem kamerę
                    continue;//kontynuję pętlę czyli sprawdzam kolejny wiersz
                }
                if (CurrentColumn == RoomSize - 1 && CurrentSum < MinimumSum) {//Jeżeli jestem w ostatniej kolumnie i suma którą uzyskałem jest mniejsza od minimalnej
                    CopyArr(CurrentArr);//to CurrentArray staje się rozwiązaniem (SolutionArray)
                    MinimumSum = CurrentSum;//i obecna suma staje się minimalną
                }

                //Jeżeli zająłem rząd w kolumnie to przechodzę do kolejnej kolumny
                Solution(CurrentColumn + 1, CurrentArr);//Wchodzę rekurencyjnie do kolejnej kolumny przekazując numer tej kolumny oraz półpełne rozwiązanie w postaci obecnej tablicy


                //Cofam się jeżeli nie znalazłem wyniku (?) (jak jestem w wostatniej kolumnie?)
                RowTaken[i] = false;//Zdejmuję kamerę z danego wiersza
                CurrentArr[CurrentColumn] = -1; //Zeruję wiersz w którym zdjąłem kamerę
                CurrentSum -= Room[i][CurrentColumn]; //Od sumy odejmuję wartość kratki w pokoju

            }

        }

        return;
    }

    void PrintArr() {//Wypisuję rozwiązanie (SolutionArray)
        for (int i = 0; i < NoCameras; i++) {
            cout << this->SolutionArr[i] << " ";
        }
    }

    int GetNoCameras() {//Zwracam wymaganą liczbę kamer w pokoju
        return NoCameras;
    }

};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    Backtracking b1;

    int* CurrentArr = new int[b1.GetNoCameras()];//Tworzę tablicę która będzie wykorzystwana do przechowywania półpełnych rozwiązań
    b1.Solution(0, CurrentArr);
    b1.PrintArr();
}