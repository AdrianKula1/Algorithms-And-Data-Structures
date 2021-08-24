#include <iostream>

using namespace std;

struct Patient {
    int ID=0, Priority=0, Index=0;
    Patient() = default;
    Patient(int ID, int Priority, int Index) {
        this->ID = ID;
        this->Priority = Priority;
        this->Index = Index;
    }
};

class Heap {
private:
    int Size = 2, NoElements = 0, Counter = 0;
    Patient* Patients = new Patient[Size];
public:
    void AddPatient(int ID, int Priority) {
        if (NoElements == Size) {
            Size *= 2;
            Patient* Tmp = new Patient[Size];
            for (int i = 0; i < NoElements; i++) {
                Tmp[i] = Patients[i];
            }
            delete Patients;
            Patients = Tmp;
        }
        Patients[NoElements] = { ID, Priority, Counter};
        Upheap(NoElements);
        NoElements++;
        Counter++;
    }

    void Swap(Patient& P1, Patient& P2) {//zamieniam dwa elementy tablicy
        Patient tmp = P1;
        P1 = P2;
        P2 = tmp;
    }


    bool FatherExists(int IndexOfElement) {//Sprawdzam czy ojciec elementu istnieje
        return ((IndexOfElement-1) / 2) >= 0;
    }

    void Upheap(int IndexOfSon) {//Przesiewanie kopca w górę
        int IndexOfFather = (IndexOfSon - 1) / 2;
        while (FatherExists(IndexOfSon)) {//Dopóki ojciec istnieje
            if (Patients[IndexOfSon].Priority < Patients[IndexOfFather].Priority) {//Jeżeli priorytet syna jest mniejszy od priorytetu ojca
                Swap(Patients[IndexOfFather], Patients[IndexOfSon]);//Zamieniam te dwa elementy
                IndexOfSon = IndexOfFather;//ojciec staje się synem
                IndexOfFather = (IndexOfFather - 1) / 2;//Wyliczam nowego ojca
            }else {
                return;
            }
        }
    }

    void Downheap() {//Przesiewanie kopca w dół
        int IndexOfFather = 0, IndexOfSon = 1;
        while (LSonExists(IndexOfFather)) {//Dopóki lewy syn ojca istnieje
            if (RSonExists(IndexOfFather)) {//Jeżeli prawy syn istnieje
                if (Patients[IndexOfSon + 1].Priority < Patients[IndexOfSon].Priority) {//Jeżeli prawy syn ma mniejszy priorytet niż lewy to biorę jego
                    IndexOfSon++;
                }else if (Patients[IndexOfSon + 1].Priority == Patients[IndexOfSon].Priority) {//Jeżeli synowie mają takie same priorytety i...
                    if (Patients[IndexOfSon + 1].Index < Patients[IndexOfSon].Index) {//...Jeżeli prawy syn ma mniejszy indeks od lewego syna to biorę jego
                        IndexOfSon++;
                    }
                }
            }

            if (Patients[IndexOfFather].Priority < Patients[IndexOfSon].Priority) {//Jeżeli ojciec ma mniejszy priorytet od syna to konczę pętlę
                return;
            }
            if (Patients[IndexOfFather].Priority == Patients[IndexOfSon].Priority) {//Jeżeli ojciec ma taki sam priorytet jak syn i...
                if (Patients[IndexOfFather].Index < Patients[IndexOfSon].Index) {//...Jeżeli ojciec ma mniejszy indeks niż syn top niuc się nie dzieje
                    return;
                }
            }
            Swap(Patients[IndexOfFather], Patients[IndexOfSon]);//Zamieniam ojca i syna
            IndexOfFather = IndexOfSon;//Ojciec staj się synem
            IndexOfSon = (IndexOfSon * 2) + 1;//Wyliczam nowy indeks syna
        }
    }

    bool LSonExists(int IndexOfElement) {//Jeżeli lewy syn istnieje
        return ((IndexOfElement * 2) + 1) < NoElements;
    }

    bool RSonExists(int IndexOfElement) {//Jeżeli prawy syn istnieje
        return ((IndexOfElement * 2) + 2) < NoElements;
    }

    void DisplayQueue(int NoVaccines) {//Wyświetlam elementy kolejki+
        if (NoElements < NoVaccines) {//Jeżeli elementów w kopcu jest mniej niż elementów do wyświetlenia
            NoVaccines = NoElements;//to wyświetlam wszyskie elementy kopca
        }

        for (int i = 0; i < NoVaccines; i++) {//Dla każdego elementu w kopcu...
            cout << Patients[0].ID << " ";//...Wyświetl go...
            DeleteRoot();//...I usuń z kopca
        }
        cout << endl;
        return;
    }

    void DeleteRoot() {//Usuwanie elementu z kopca
        NoElements--;//Zmniejszam liczbę elementów w kopcu
        Swap(Patients[0], Patients[NoElements]);//zamieniam ostatni element z zerowym
        Downheap();//Przesiewam kopiec w dół
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int NoVaccinesPerDay, NoDays, NoPepoleToVaccine, ID, Priority;
    Heap H1;

    cin >> NoVaccinesPerDay >> NoDays;

    for (int i = 0; i < NoDays; i++) {
        cin >> NoPepoleToVaccine;
        for (int j = 0; j < NoPepoleToVaccine; j++) {
            cin >> ID >> Priority;
            H1.AddPatient(ID, Priority);
        }
        H1.DisplayQueue(NoVaccinesPerDay);
    }

}