#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct element{
    string Password;
    string Explanation;

    element()=default;
    element(string Password, string Explanation) {
        this->Password = Password;
        this->Explanation = Explanation;
    }
};

struct treeElement{
    vector<element *>passwords;
    vector<treeElement *>childs;
    bool leaf=false;
    int maxSize;

    treeElement(int maxSize, bool Leaf){
        this->maxSize=maxSize;
        this->leaf=Leaf;
    }

    void splitChild(treeElement *toSplit){
        treeElement *rightHalf = new treeElement(toSplit->maxSize, toSplit->leaf);//tworze vector zawierajacy prawa czesc elementu do podzielenia

        //przenosze prawa czesc dzieci z oSplit do rightHalf
        rightHalf->passwords.insert(rightHalf->passwords.end(), make_move_iterator(toSplit->passwords.begin() + toSplit->passwords.size()/2+1), make_move_iterator(toSplit->passwords.end()));
        toSplit->passwords.erase(toSplit->passwords.begin() + toSplit->passwords.size()/2+1, toSplit->passwords.end());


        if(!toSplit->leaf){//jezeli toSplit nie jest lisciem to przenosze takze jego dzieci do rightHalf
            rightHalf->childs.insert(rightHalf->childs.end(), make_move_iterator(toSplit->childs.begin() + toSplit->childs.size()/2), make_move_iterator(toSplit->childs.end()));
            toSplit->childs.erase(toSplit->childs.begin() + toSplit->childs.size()/2, toSplit->childs.end());
        }

        int i=0;
        while(i<passwords.size() && (toSplit->passwords.back())->Password > passwords[i]->Password){//szukam indeksu na ktory mam wstawic srodkowy element toSplit w jego ojcu
            i++;
        }

        passwords.insert(passwords.begin()+i, toSplit->passwords.back());//wstawiam srodkowy elemnt toSplit w jego ojcu
        toSplit->passwords.erase(toSplit->passwords.begin() + toSplit->passwords.size()-1);

        if(childs.empty()){//jezeli dopiero tworze ojca to przepisuje dzieci po lewej i prawej stronie nowego elementu
            childs.insert(childs.begin()+i, toSplit);
            childs.insert(childs.begin()+i+1, rightHalf);
        }else{//jezeli ojciec juz istnieje to dopisuje wskaznik na dziecko w podanym indeksie
            childs.insert(childs.begin()+i+1, rightHalf);
        }
    }



    void insertElement(string password, string explanation){
        if(passwords.empty()){//jezeli lista elementow jest pusta to wsadzam element i koncze funkcje
            passwords.push_back(new element(password, explanation));
            return;
        }
        int i = passwords.size()-1;

        while(i>0 && (passwords[i]->Password > password)){//szukamindeksu pod ktorym mam umiescic haslo (alfabetycznie)
            i--;
        }

        if(passwords[i]->Password == password){//jezeli podane haslo juz istnieje to podmieniam definicje
            passwords[i]->Explanation = explanation;
            return;
        }

        if(password > passwords[i]->Password){//jezel podane haslo jest mniejsze od znalezionego to zwiekszam indeks o 1 zeby wiedziec na ktore miejsce wsadzic element lub do ktorego dziecka sie odwolac
            i++;
        }

        if(leaf){//jezeli element drzewa jest lisciem to po prostu wsadzam haslo i definicje na indeks i
            passwords.insert(passwords.begin()+i, new element(password, explanation));
        }else{//jezeli element drzewa nie jest lisciem to
            childs[i]->insertElement(password, explanation);//rekurencyjnie wsadzam element do znalezionego dziecka
            if(childs[i]->passwords.size() == maxSize){//jezeli dziecko o indeksie i osiagnelo maksymalny rozmiar to dziele je funkcja
                splitChild(childs[i]);
            }
        }
    }

    element *search(string password){//wyszukuje elementu
        if(passwords.empty()){//jezeli nie ma hasel to zwracam nullptr
            return nullptr;
        }

        int i=0;

        while(i < passwords.size() - 1 && password > passwords[i]->Password){//szukam indeksu pod ktorym jest podane haslo
            i++;
        }

        if(passwords[i]->Password==password){//jezeli haslo sie zgadza to zwracam element z tym haslem
            return passwords[i];
        }

        if(leaf){//jezeli element drzewa jest liściem to zwracam nullptr bo nie da sie wejsc glebiej
            return  nullptr;
        }

        if(password > passwords[i]->Password){//jezeli podane haslo jest mniejsze od tego o znalezionym indeksie to musze szukac w prawym wskazniku na dzieci
            i++;
        }

        return childs[i]->search(password); //rekurencyjnie wchodze do dziecka o indeksie i
    }

};

class tree{
private:
    int maxSize=11;//maxsymalna liczba haseł w elemencie drzewa to 11
    treeElement *root= nullptr;
public:
    tree(){
        int n;
        cin >> n;
        cin >> ws;
        string password, explanation;
        for(int i=0; i<n; i++){//pobieranie danych
            string phrase;
            getline(cin, phrase);
            if(phrase.find(':')!=std::string::npos){
                stringstream ss(phrase);
                std::getline(ss, password, ':');
                std::getline(ss, explanation, ':');
                explanation.erase(explanation.begin());
                insertElement(password, explanation);
            }else{
                showExplanation(phrase);
            }
        }
    }

    void insertElement(string password, string explanation){//wstawiam element do drzewa
        if(root==nullptr){//jezeli root nie istnieje to go tworze i wstawiam
            root=new treeElement(maxSize, true);
            root->insertElement(password, explanation);
        }else{//jezeli root istnieje to wstawiam hasło na odpowiednie miejsce w roocie
            root->insertElement(password, explanation);
            if(root->passwords.size() == maxSize) {//jeZeli root ma maxsymalna liczbe hasel
                treeElement *newRoot = new treeElement(maxSize, false);//tworze nowy root
                newRoot->splitChild(root);//wypycham srodkowy elemnt do gory tworzac nowy root a lewa i prawa strona starego roota stają się dziećmi nowego
                root = newRoot;//ustalam nowy root
            }
        }
    }

    void showExplanation(string password){
        element *result=root->search(password);
        if(result == nullptr){
            cout << "BRAK" << endl;
        }else{
            cout << result->Explanation << endl;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    tree T1;
    return 0;
}