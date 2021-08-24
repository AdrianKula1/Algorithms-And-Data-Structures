#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Patient {
    int ID = 0;
    string Name, Surname;
    Patient() = default;
    Patient(int ID, string Name, string Surname) {
        this->ID = ID;
        this->Name = Name;
        this->Surname = Surname;
    }
};

bool Compare(int P1, int P2) {
    return P1 < P2;
}


int MedianOfMedians(vector<int> Patients) {//Szukam najlepszego elementu do użycia jako oś w algorytmie Horae'a
    if (Patients.size() <= 10) {//Jeżeli wektor ma 10 lub mniej elementów to po prostu go sortuję i wybieram środkowy element
        sort(Patients.begin(), Patients.begin() + Patients.size(), Compare);
        return Patients[Patients.size() / 2];
    }


    vector<int> Medians;
    int Rest = Patients.size() % 5;


    for (int i = 0; i < Patients.size(); i += 5) {//Co każde 5 elementów wwektorze
        if (i + 5 > Patients.size()) {//Jeżeli elementów jest mniej niż 5
            sort(Patients.begin() + i, Patients.begin() + i + Rest, Compare);//sortuję je
            Medians.push_back(Patients[(i + i + Rest) / 2]);//dodaję środkowy element do vektora median
            break;//Kończę pętlę
        }
        sort(Patients.begin() + i, Patients.begin() + i + 5, Compare);//Sortuję każde 5 elementów
        Medians.push_back(Patients[(i + i + 5) / 2]);//dodaję środkowy element do wektora median

    }
    return MedianOfMedians(Medians);//Wywołuję rekurencyjnie funkcję dla median wektora
}

int Hoare(vector<int> Patients, int KthPatient) {
    if (Patients.size() == 1) {//Jeżeli vector ma jeden element to po prostu go zwracam
        return Patients[0];
    }
    int Axis = MedianOfMedians(Patients);//Wyszukuję medianę median - optymalny element względem którego będę układał elementy wektora
    vector<int> Less, Equal, More;//Tworzę 3 wektory
    for (int i = 0; i < Patients.size(); i++) {//Dla każdego elementu w podanym wektorze
        if (Patients[i] < Axis) {//jeżeli ID i-tego pacjenta jest mniejsze niż ID osi to daję go do wektora przechowywującego ID mniejsze itd...
            Less.push_back(Patients[i]);
        }
        else if (Patients[i] == Axis) {
            Equal.push_back(Patients[i]);
        }
        else {
            More.push_back(Patients[i]);
        }
    }

    if (Less.size() > KthPatient) {//Jeżeli szukany element jest mniejszy mniejszemu vektorowi(less) to analizuję less w taki sam sposób
        vector<int>().swap(Equal);
        vector<int>().swap(More);
        return Hoare(Less, KthPatient);
    }
    else if ((Less.size() <= KthPatient) && (Less.size() + Equal.size()) > KthPatient) {//Jeżeli szukany element jest większy równy od less i mniejszy równy less+equal to zwracam zerowy element equal
        vector<int>().swap(Less);
        vector<int>().swap(More);
        int Result = Equal[0];
        vector<int>().swap(Equal);
        return Result;
    }
    else if (Less.size() + Equal.size() <= KthPatient) {//Jeżeli szukany element jest większy od less+equal to analizuję większy vektor(more) w taki sam sopsób ZMIEJSZAJĄC element szukany o less+equal
        int newNumber = (KthPatient - (Less.size() + Equal.size()));
        vector<int>().swap(Less);
        vector<int>().swap(Equal);
        return Hoare(More, newNumber);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int NoPatients, NoChanges;
    cin >> NoPatients >> NoChanges;

    Patient* Patients = new Patient[NoPatients];//Tablica pacjentów przechowywująca wszystkie dane
    vector<int> tmp;//vektor przechowujący ID pracowników
    int ID = 0, NewID = 0;
    string Name, Surname;
    for (int i = 0; i < NoPatients; i++) {//Pobieranie danych
        cin >> ID >> Name >> Surname;
        Patients[i] = { ID, Name, Surname};
        tmp.push_back(ID);
    }

    for (int i = 0; i < NoChanges; i++) {//Dla każdej zmiany ID pacjenta...
        cin >> ID >> NewID >> Name >> Surname;//pobierz nowe dane
        int IdOfKthPerson = Hoare(tmp, ID);//wyszukaj ID wymaganej osoby
        int Index = -1;
        for (int i = 0; i < NoPatients; i++) {//odnajdź jej miejsce w tablicy
            if (Patients[i].ID == IdOfKthPerson) {
                Index = i;
                break;
            }
        }

        //Wypisanie dwóch osób które są w tablicy przed tą znalezioną, wypisanie znalezionej osoby i wypisanie dwóch osób które są w tablicy po tej znalezionej (jeżeli istnieją)
        if (Index - 2 >= 0) {
            cout << Patients[Index - 2].ID << " " << Patients[Index - 2].Name << " " << Patients[Index - 2].Surname << endl;
        }

        if (Index - 1 >= 0) {
            cout << Patients[Index - 1].ID << " " << Patients[Index - 1].Name << " " << Patients[Index - 1].Surname << endl;
        }

        cout << Patients[Index].ID << " " << Patients[Index].Name << " " << Patients[Index].Surname << endl;

        if (Index + 1 < NoPatients) {
            cout << Patients[Index + 1].ID << " " << Patients[Index + 1].Name << " " << Patients[Index + 1].Surname << endl;
        }

        if (Index + 2 < NoPatients) {
            cout << Patients[Index + 2].ID << " " << Patients[Index + 2].Name << " " << Patients[Index + 2].Surname << endl;
        }

        Patients[Index] = { NewID, Name, Surname};//Zmiana danych pacjenta
        tmp[Index] = NewID;
    }


}