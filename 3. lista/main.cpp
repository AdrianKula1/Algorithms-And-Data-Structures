#include <iostream>
#include <string>

using namespace std;

struct Worker{
    unsigned int ID;//ID pracownika
    string Name;//Mazwa pracownika
    struct Worker *Previous=NULL, *Next=NULL;
    struct Department *DepName;//Wskaźnik na wydział w którym pracuje pracownik
    Worker()=default;
    Worker(string Name, Department *DepName){
        this->Name=Name;
        this->DepName=DepName;
    }
};

class WorkersAccessor{//Tablicowa lista pracowników
private:
    unsigned int Size=100, NoElements=0;
    Worker **WorkersArr=new Worker*[Size];//Tabvlica wskaźników na pracowników
public:

    void AddWorkerToArr(string Name, Department *DepName){//Dodaję pracownika do listy pracowników
        if(NoElements==Size){//Jeżeli tablica jest pełna to tworzę nową z dwukrotną ilością miejsc i przepisuję wszystkie elementy ze starej do nowej
            Size*=2;
            Worker **Tmp=new Worker*[Size];
            for(unsigned int i=0; i<NoElements; i++){
                Tmp[i]=WorkersArr[i];
            }
            delete WorkersArr;
            WorkersArr=Tmp;
        }
        WorkersArr[NoElements]=new Worker(Name, DepName);//Dodaję nowego pracownika
        WorkersArr[NoElements]->ID=NoElements;
        NoElements++;
        return;
    }

    Worker* GetWorker(unsigned int ID){//Zwracam wskaźnik na pracownika
        return WorkersArr[ID];
    }

    unsigned int GetIdOfLatestWorker(){//Ostatni pracownik ma ID (LiczbaElementówTablicy-1) bo zerowy element tablicy jest pracownikiem nr 1
        return WorkersArr[NoElements-1]->ID;
    }

    void RenameWorker(unsigned int ID, string Name){//Zmieniam nazwę pracownika o danym ID
        WorkersArr[ID]->Name=Name;
        return;
    }

    void ChgangeDepartment(unsigned int ID, Department *DepName){//Zmieniam nawze wydziału w któryum pracuje dany pracownik
        WorkersArr[ID]->DepName=DepName;
        return;
    }
};

WorkersAccessor Accessor;

class WorkersList{//Lista pracowników w danym wydziale
private:
    unsigned int NoWorkers=0;//Liczba pracowników w danym wydziale
    Worker *Head=NULL, *Tail=NULL;

public:
    void AddWorker(string Name, Department *DepName){//Dodaję NOWEGO pracownika do listy pracowników w danym wydziale
        Accessor.AddWorkerToArr(Name, DepName);//Dodaję pracownika do OGÓLNEJ LISTY PRACOWNIKÓW
        if(Head==NULL){//Jeżeli wydział nie ma pracownika to go tworzę
            Head=Tail=Accessor.GetWorker(Accessor.GetIdOfLatestWorker());//Pobieram wskaźnik na pierwszego pracownika  i przypisuję go do głowy tej listy
            Head->Next=Head->Previous=Head;
        }else{//Dodaję pracownika na koniec listy i pamiętam że to lista dwukierunkowa cykliczna wskaźnikowa
            Worker *tmp=Accessor.GetWorker(Accessor.GetIdOfLatestWorker());//Pobieram wskaźnik na kolejnego pracownika i przypisuję go do ogona
            tmp->Next=Head;
            tmp->Previous=Tail;
            Tail->Next=tmp;
            Head->Previous=tmp;
            Tail=tmp;

        }
        NoWorkers++;
        return;
    }

    void AddWorker(unsigned int ID){//Dodaję ISTNIEJĄCEGO pracownika do listy pracowników w danym wydziale
        if(Head==NULL){
            Head=Tail=Accessor.GetWorker(ID);//Istniejący pracownik o danym ID staje się głową
            Head->Next=Head->Previous=Head;
        }else{
            Worker *tmp=Accessor.GetWorker(ID);//Pobieram wskaźnik na kolejnego pracownika i przypisuję go do ogona
            tmp->Next=Head;
            tmp->Previous=Tail;
            Tail->Next=tmp;
            Head->Previous=tmp;
            Tail=tmp;
        }
        NoWorkers++;
        return;
    }

    void DeleteWorker(unsigned int ID){//Usuwam pracownika z listy pracowników danego wydziału
        if(NoWorkers==0){
            return;
        }

        Worker *tmp=Accessor.GetWorker(ID); //Pobieram pracownika


        if(NoWorkers==1){//Jeżeli jest jeden pracownik to
            NoWorkers--;//zmiejszam liczbę pracowników w danym wydziale
            tmp->DepName=NULL;//Czyszczę nazwę wydziału w którym dany pracownik pracuje
            Head=NULL;//Usuwam głowę listy
            return;
        }

        if(tmp==Head){//Jeżeli element do usunięcia to głowa to głową staje się kolejny element
            Head=Head->Next;
            Head->Previous=Tail;
            Tail->Next=Head;
        }else if(tmp==Tail){//Jeżeli element do usunięcia to ogon to ogonem staje się poprzedni element
            Tail=Tail->Previous;
            Tail->Next=Head;
            Head->Previous=Tail;
        }else{//Jeżeli element do sunięcia jst gdzieś po środku to przepinam odpowiednio wskaźniki
            tmp->Previous->Next=tmp->Next;
            tmp->Next->Previous=tmp->Previous;
        }
        tmp->DepName=NULL;//Usuwam nawę wydziału u danego pracownika
        NoWorkers--;//Liczba pracownikó w danym wydziale zmiejsza się o 1
        return;
    }

    void MoveShift(unsigned int NoShifts, unsigned int MinNoWorkers){
        unsigned int Result=(NoShifts*MinNoWorkers);//Wynik to liczba zmian * minimalna liczba pracowników w wydziale

        if(Result>=NoWorkers){//Jeżeli wynik jest większy od ilości pracowników to robię dzielenie modulo
            Result%=NoWorkers;
        }

        if(Result==0){//Jeżeli nie muszę przesuwać zmian to nic nie robię
            return;
        }

        Worker *tmp=Head;


        if(Result<=NoWorkers/2){//Bliżej jest iść do przodu
            for(unsigned int i=0; i<Result; i++){
                tmp=tmp->Next;
            }
        }else{//Bliżej jest iść do tyłu
            Result=NoWorkers-Result;//????
            for(unsigned int i=0; i<Result; i++){
                tmp=tmp->Previous;
            }
        }

        Head=tmp;
        Tail=Head->Previous;
    }

    bool Empty(){//Jeżeli lista nie ma elementów to zwracam true w przeciwnym przypadku zwracam false
        if(Head==NULL){
            return true;
        }else{
            return false;
        }
    }

    Worker *getHead(){//Zwracam wskaźnk na głowę listy
        return Head;
    }

};

struct Department{
    string Name;//Nazwa wydziału
    unsigned int MinNoWorkers, ActualNoWorkers;//Minimalna wymagana ilość pracowników i aktualna ilość pracowników
    struct Department *Next=NULL;//Wskaźnik na kolejny wydział
    class WorkersList WorkList;//Każdy wydział ma listę pracowników
    Department()=default;
    Department(string Name, unsigned int MinNoWorkers){
        this->Name=Name;
        this->MinNoWorkers=MinNoWorkers;
    }

    Department(string Name, unsigned int MinNoWorkers, Department *Next){
        this->Name=Name;
        this->MinNoWorkers=MinNoWorkers;
        this->Next=Next;
    }
};

class DepartmentList{
private:
    Department *Head=NULL, *Tail=NULL;

public:
    void AddDepartment(string DepartmentName, unsigned int MinNoWorkers){//Dodaję wydział do jednokierunkowej wskaźnikowej listy wydziałów
        if(Head==NULL){//Jeżeli głowa listy to NULL to tworzę głowę
            Head=Tail=new Department(DepartmentName, MinNoWorkers);
            Head->Next=Tail;
            Tail->Next=NULL;
        }else{//Dodaję element na koniec listy i wskaźnik na następny element to NULL
            Tail->Next=new Department(DepartmentName, MinNoWorkers, NULL);
            Tail=Tail->Next;
        }
        return;
    }

    void AddWorker(string Name, string DepName){//Dodaję pracownika do wydziału
        Department *tmp=Head;
        while(tmp!=NULL && tmp->Name!=DepName){//Wyszukuję wydział w którym mam umieścić nowego pracownika
            tmp=tmp->Next;
        }
        tmp->WorkList.AddWorker(Name, tmp);//Dodaję pracownika do listy pracowników w tym wydziale
        tmp->ActualNoWorkers++;//Zwiększam liczbę pracowinków w tym wydziale o 1
        return;
    }

    void DeleteDepartment(string Name){//Usuwam wydział
        if(Head==NULL){
            return;
        }

        Department *tmp=Head;
        if(tmp->Name==Name){//Jeżeli wydział jest głową listy to usuwam głowę i głową staje się kolejny element(jeżeli istnieje. Jak nie to NULL)
            if(tmp->Next!=NULL){
                Head=Head->Next;
            }else{
                Head=NULL;
            }
            return;
        }

        while(tmp!=NULL && tmp->Next->Name!=Name){//Wyszukuję wydziału który mam usunąć -1 bo nie mogę się cofać w liście jednokierunkowej
            tmp=tmp->Next;
        }

        //Przepinam wskaźniki aby usunąć dany element
        if(tmp->Next->Next!=NULL){//Jeżeli następny element istnieje to po prostu wskaźnik na następny element (czyli ten który mam usunąć) zastępuję kolejnym
            tmp->Next=tmp->Next->Next;//Omijam element który mam usunąć
        }else{
            tmp->Next=NULL;//Następnyy element to NULL
            Tail=tmp;//Ogonem staje się wcześniejszy element
        }

        return;
    }

    void DeleteWorker(unsigned int ID){//Usuwam pracownika z wydziału
        Department *tmp=Accessor.GetWorker(ID)->DepName;//Pobieram wsklaźnik na wydział w którym pracuje dany pracownik
        tmp->WorkList.DeleteWorker(ID);//Usuwam pracownika z listy pracowików danego wydziału
        tmp->ActualNoWorkers--;//Zmniejszam liczbę pracowników w danym wydziale
        return;
    }

    void TransferWorker(unsigned int ID, string DepName){//Przenoszę pracownika z wydziału (jeżeli już w jakimś jest) do wydziału
        Department *tmp=Head, *Dep1=NULL, *Dep2=NULL;
        if(Accessor.GetWorker(ID)->DepName!=NULL){//Jeżeli pracownik pracuje już w jakimś wydziale to pobieram wskaźnik na ten wydział
            Dep1=Accessor.GetWorker(ID)->DepName;
        }


        while(tmp!=NULL){//Wyszukuję wydział do którego mam przenieść pracownika
            if(tmp->Name==DepName){
                Dep2=tmp;
                break;
            }
            tmp=tmp->Next;
        }


        if(Dep1!=NULL && Dep2!=NULL){//Jeżeli mam oba wyższe wydziały
            Dep1->WorkList.DeleteWorker(ID);//Usuwam pracownika z wydziału w którym pracuje
            Dep1->ActualNoWorkers--;//Zmiejszam liczbę pracowników wydziału o 1

            Dep2->WorkList.AddWorker(ID);//Dodaję pracownika do wydziału
            Dep2->ActualNoWorkers++;//Zwiększam liczbę pracowników w wydziale
        }else{//Jeżeli mam tylko wydział DO KTÓREGO MAM PRZENIEŚĆ PRACOWNIKA
            Dep2->WorkList.AddWorker(ID);
            Dep2->ActualNoWorkers++;
        }
        Accessor.ChgangeDepartment(ID, Dep2);//Zmieniam wskaźnik na wydział w którym pracuje dany pracownik
    }

    void RenameDepartment(string OldName, string NewName, unsigned int NewMinNoWorkers){//Zmieniam nazwę i min ilość pracowników w wydziale

        Department *tmp=Head;
        while(tmp!=NULL){//Wyszukuję wydziału którego nazwę mam zmienić
            if(tmp->Name==OldName){
                break;
            }
            tmp=tmp->Next;
        }

        tmp->Name=NewName;//Zmieniam nazwę
        tmp->MinNoWorkers=NewMinNoWorkers;//Zmieniam minimalną liczbę pracowników w danym wydziale
        return;
    }

    void Display(unsigned int NoChanges){//Zmieniam zmiany w każdym z wydziałów
        if(Head==NULL){//Jeżeli nie mam wydziałów to nic się nie dzieje
            cout << "-" << endl;
            return;
        }

        Department *Dept=Head;

        do{//Wyświetlam wydziały i każdego z pracowników z danego wydziału
            if(!Dept->WorkList.Empty()){
                if(NoChanges!=0){//Wprowadzam zmiany w każdym z wydziałów
                    Dept->WorkList.MoveShift(NoChanges, Dept->MinNoWorkers);
                }

                if(Dept->ActualNoWorkers<Dept->MinNoWorkers){//JEżeli liczba pracowników jest liczba od minimalnej wymaganej liczby pracowników to wyświetlam komunikat
                    cout << "(niedobor pracownikow)";
                }
                cout << Dept->Name << ":";//Wyświetlam nazwę wydziału

                Worker *WorkerBegin=Dept->WorkList.getHead();//Pobieram pierwszego pracownika z listy pracowników danego wydziału

                if(Dept->ActualNoWorkers<Dept->MinNoWorkers){//Jeżeli liczba pracowników jest mniejsza niż wymagana to wypisuję wszystkich
                    do{//Wyświetlam każdego pracownika wydziału
                        if(WorkerBegin->Next!=Dept->WorkList.getHead()){
                            cout << WorkerBegin->Name << ",";
                        }else{
                            cout << WorkerBegin->Name << ".";
                        }
                        WorkerBegin=WorkerBegin->Next;

                    }while(WorkerBegin!=Dept->WorkList.getHead());//Idę po pracownikach dopóki nie wrócę do początku listy
                }else{//Jeżeli liczba pracowników jest równa lub większa niż wymagana to wypisuję tylko tyle pracowników jaką wartość ma liczba racowników NA ZMIANIE
                    for(unsigned int i=0; i<Dept->MinNoWorkers; i++){
                        if(i==Dept->MinNoWorkers-1){
                            cout << WorkerBegin->Name <<  ".";
                            break;
                        }
                        cout << WorkerBegin->Name << ",";
                        WorkerBegin=WorkerBegin->Next;
                    }
                }
                cout << endl;
            }else{//Jeżeli wydział nie ma pracowników to wyświetlam tylko W1:.  - i idę do następnego
                if(Dept->ActualNoWorkers<Dept->MinNoWorkers){
                    cout << "(niedobor pracownikow)";
                }
                cout << Dept->Name << ":" << "." << endl;
            }
            Dept=Dept->Next;

        }while(Dept!=NULL);//Dopóki nie natrafię na NULLa czyli dopóki wydziały istnieją
        cout << "-" << endl;//Na koniec daję kreskę

        return;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    unsigned int NoOperations;
    char Operation;
    string WorkerName, DepartmentName, OldDepartmentName;
    unsigned int MinNoWorkers, ID, Number;
    cin >> NoOperations;
    DepartmentList DepList;
    for(unsigned int i=0; i<NoOperations; i++){
        cin >> Operation;
        switch (Operation){
            case 'a':
                cin >> Operation;
                switch (Operation){
                    case 'w':
                        cin >> DepartmentName >> MinNoWorkers;
                        DepList.AddDepartment(DepartmentName, MinNoWorkers);//Dodaję wydział (element do listy wydziałów)
                        break;

                        case 'e':
                            cin >> WorkerName >> DepartmentName;
                            DepList.AddWorker(WorkerName, DepartmentName);//Dodaję pracownika W WYDZIALE (element do listy prascowników w wydziale)
                            break;
                }
                break;
                case 'd':
                    cin >> Operation;
                    switch (Operation){
                        case 'w':
                            cin >> DepartmentName;
                            DepList.DeleteDepartment(DepartmentName);//Usuwam WYDZIAŁ
                            break;

                            case 'e':
                                cin >> ID;
                                DepList.DeleteWorker(ID);//Usuwam pracownika Z WYDZIAŁU
                                break;
                    }
                    break;
                    case 'm':
                        cin >> ID >> DepartmentName;
                        DepList.TransferWorker(ID, DepartmentName);//Przenoszę pracownika Z WYDZIAŁU (jeżeli już w jakimś jest) DO WYDZIAŁU

                        break;
                        case 'e':
                            cin >> Operation;
                            switch (Operation){
                                case 'w':
                                    cin >> OldDepartmentName >> DepartmentName >> MinNoWorkers;
                                    DepList.RenameDepartment(OldDepartmentName, DepartmentName, MinNoWorkers);//Zmieniam nazwę i minimalną liczbę pracowników W WYDZIALE
                                    break;

                                    case 'e':
                                        cin >> ID >> WorkerName;
                                        Accessor.RenameWorker(ID, WorkerName);//Zmieniam nazwę PRACOWNIKA
                                        break;
                            }
                            break;
                            case 's':
                                cin >> Number;
                                DepList.Display(Number);//Wykonuję zmiany i wyświetlam pracowników
                                break;

                                default:
                                    cout << "ERROR";
                                    break;

        }
    }
}