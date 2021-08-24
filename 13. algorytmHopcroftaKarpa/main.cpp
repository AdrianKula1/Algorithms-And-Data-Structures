#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct node {
    int ID=0;
    vector<node *> neighbours;
    bool visited=false;
    bool isWhite=false;
    node *pair=nullptr;
    int distance=0;
    node *next = nullptr;
    bool partAugPath=false;
    node() = default;

    node(int ID) {
        this->ID = ID;
    }
};

struct field {
    int ID, pawn;
};


class graph {
private:
    int chessboardSize = 0;
    field **chessboard;
    node *head = nullptr;
    node *dummy=new node(-1);
public:
    graph() {//stworzenie szachownicy i przypisanie do niej podanych danych
        cin >> chessboardSize;
        chessboard = new field *[chessboardSize];
        int counter = 0;
        for (int i = 0; i < chessboardSize; i++) {
            chessboard[i] = new field[chessboardSize];
            for (int j = 0; j < chessboardSize; j++) {
                chessboard[i][j].ID = counter;
                cin >> chessboard[i][j].pawn;
                counter++;
            }
        }
        createGraph();//funkcja torząca graf na podstawie szachownicy
    }
    //funkcje sprawdzające czy indeksy nie wychodzą poza pola szachownicy
    bool exists1B(int i, int j) {
        if (j + 3 < chessboardSize || ((i + 1 < chessboardSize) && (j + 2 < chessboardSize)) || ((i - 1 >= 0) && (j + 2 < chessboardSize))) {
            return true;
        }
        return false;
    }

    bool exists2B(int i, int j) {
        if ((j + 1 < chessboardSize) || (i - 1>= 0) || (i + 1<chessboardSize)) {
            return true;
        }
        return false;
    }

    bool exists3B(int i, int j) {
        if ((j + 1 < chessboardSize) || ((i + 1 < chessboardSize) && (j + 2 < chessboardSize)) || ((i - 1 >= 0) && (j + 2 < chessboardSize))) {
            return true;
        }
        return false;
    }

    bool exists1W(int i, int j) {
        if ((j - 3 >= 0) || ((i + 1 < chessboardSize) && (j - 2 >= 0)) || ((i - 1 >= 0) && (j - 2 >= 0))) {
            return true;
        }
        return false;
    }

    bool exists2W(int i, int j) {
        if ((j - 1 >= 0) || (i - 1 >= 0) || (i + 1 < chessboardSize)) {
            return true;
        }
        return false;
    }

    bool exists3W(int i, int j) {
        if (j - 1 >= 0 || ((i + 1 < chessboardSize) && (j - 2 >= 0)) || ((i - 1 >= 0) && (j - 2 >= 0))) {
            return true;
        }
        return false;
    }

    node *addNode(int i, int j) {//dodanie elementu do listy sąsiedztwa. jeżeli element jeszcze nie istnieje to dodaję go na koniec listy wskaźnikowej. Jeżeli istnieje to zwracam wskaźnik na niego
        if(head==nullptr){
            head=new node(chessboard[i][j].ID);
            return head;
        }else{
            if(head->ID==chessboard[i][j].ID){
                return head;
            }
            node *tmp=head;
            while(tmp->next!=nullptr){
                if(tmp->next->ID==chessboard[i][j].ID){
                    return tmp->next;
                }
                tmp=tmp->next;
            }
            tmp->next = new node(chessboard[i][j].ID);
            return tmp->next;
        }
    }

    void createGraph() {//Stworzenie grafu
        bool isWhite = false;
        for (int i = 0; i < chessboardSize; i++) {
            for (int j = 0; j < chessboardSize; j++) {
                if (chessboard[i][j].pawn == 1) {///////Jeżeli na polu [i][j] stoi pion 1
                    if(!isWhite){//jeżeli pole jest czarne
                        if (exists1B(i, j)) {//jeżeli  istnieje pole które może dany pion zbić
                            if (j + 3 < chessboardSize) {//jeżeli pierwsze z pól istnieje...
                                if (chessboard[i][j + 3].pawn != 0) {//... i nie jest zerem(którego nie da się zbić)
                                    node *leftNode = addNode(i, j);//dodaję lewy wierzchołek do listy sąsiedztwa
                                    node *rightNode=addNode(i, j+3);//dodaję prawy wierzchołek do listy sąsiedztwa
                                    leftNode->neighbours.push_back(rightNode);//dodaję prawy wierzchołek jako sąsiad lewego
                                    leftNode->pair=dummy;//ustawiam dodatkowy wierzchołęk jako parę leweo i prawego wierzchołka
                                    rightNode->pair=dummy;
                                    leftNode->isWhite=false;//lewy wierzchołek jest czarny
                                    rightNode->isWhite=true;//prawy wierzchołek jest biały
                                }
                            }
                            if ((i + 1 < chessboardSize) && (j + 2 < chessboardSize)) {
                                if (chessboard[i + 1][j + 2].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i+1, j+2);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                            if ((i - 1 >= 0) && (j + 2 < chessboardSize)) {
                                if (chessboard[i - 1][j + 2].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i-1, j+2);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                        }
                    }else{
                        if(exists1W(i, j)){
                            if (j - 3 >= 0){
                                if (chessboard[i][j - 3].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i,j-3);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }

                            if ((i + 1 < chessboardSize) && (j - 2 >= 0)){
                                if (chessboard[i + 1][j - 2].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i+1,j-2);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                            if ((i - 1 >= 0) && (j - 2 >= 0)){
                                if (chessboard[i - 1][j - 2].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i-1,j-2);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                        }
                    }
                }else  if (chessboard[i][j].pawn == 2) {//////////////////////////////////////////////////////////////
                    if(!isWhite){
                        if (exists2B(i, j)) {
                            if (j + 1 < chessboardSize) {
                                if (chessboard[i][j + 1].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i, j+1);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                            if (i - 1>= 0) {
                                if (chessboard[i -1][j].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i-1, j);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                            if (i + 1<chessboardSize) {
                                if (chessboard[i+1][j].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i+1, j);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                        }
                    }else{
                        if(exists2W(i, j)){
                            if (j - 1 >= 0){
                                if (chessboard[i][j-1].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i,j-1);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                            if (i - 1 >= 0){
                                if (chessboard[i-1][j].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i-1, j);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                            if (i + 1 < chessboardSize){
                                if (chessboard[i+1][j].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i+1, j);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                        }
                    }
                } else if (chessboard[i][j].pawn == 3) {///////////////////////////////////////////////////////////////
                    if(!isWhite){
                        if (exists3B(i, j)) {
                            if (j + 1 < chessboardSize) {
                                if (chessboard[i][j + 1].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i, j+1);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                            if ((i + 1 < chessboardSize) && (j + 2 < chessboardSize)) {
                                if (chessboard[i + 1][j + 2].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i+1, j+2);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                            if ((i - 1 >= 0) && (j + 2 < chessboardSize)) {
                                if (chessboard[i - 1][j + 2].pawn != 0) {
                                    node *leftNode = addNode(i, j);
                                    node *rightNode=addNode(i-1, j+2);
                                    leftNode->neighbours.push_back(rightNode);
                                    leftNode->pair=dummy;
                                    rightNode->pair=dummy;
                                    leftNode->isWhite = false;
                                    rightNode->isWhite=true;
                                }
                            }
                        }
                    }else{
                        if(exists3W(i, j)){
                            if (j - 1 >= 0){
                                if (chessboard[i][j-1].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i,j-1);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                            if ((i + 1 < chessboardSize) && (j - 2 >= 0)){
                                if (chessboard[i + 1][j - 2].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i+1,j-2);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                            if ((i - 1 >= 0) && (j - 2 >= 0)){
                                if (chessboard[i - 1][j - 2].pawn != 0) {
                                    node *rightNode = addNode(i, j);
                                    node *leftNode=addNode(i-1,j-2);
                                    rightNode->neighbours.push_back(leftNode);
                                    rightNode->pair=dummy;
                                    leftNode->pair=dummy;
                                    rightNode->isWhite = true;
                                    leftNode->isWhite = false;
                                }
                            }
                        }
                    }
                }
                if(chessboardSize%2==1){
                    isWhite = !isWhite;
                }else{
                    if(j!=chessboardSize-1){
                        isWhite = !isWhite;
                    }

                }
            }
        }
    }

    bool bfs(){//Sprawdzam czy któryś z czarnych wierzchołków nie ma jeszcze pary i czy da sie stworzyć ścieżkę
        queue<node*>q1;
        node *tmp=head;
        while(tmp!=nullptr){//dodaję do kolejki wszystkie czarne wierzchołki które nie mają jeszcze pary
            if(!tmp->isWhite && tmp->pair==dummy){
                q1.push(tmp);
                tmp->distance=0;
            }else if(!tmp->isWhite && tmp->pair!=dummy) {//Jeżeli dany czarny wierzchołek ma już parę to ustawiam jego dystans na INT_MAX czyli ten wierzchołęk nie będzie brany pod uwagę podczas następnego obrotu pętli
                tmp->distance=INT_MAX;
            }
            tmp=tmp->next;
        }
        dummy->distance=INT_MAX;//Ustawiam dystans wierzchołka dodatkowego na INT_MAX
        while(!q1.empty()){//Dopóki kolejka nie jest pusta
            node *current=q1.front();
            q1.pop();
            if(current->distance < dummy->distance){//Jeżeli dystans aktualnego czarnego wierzchołka jest mniejszy od dystansu wierzchołka dodatkowego
                for(int i=0; i<current->neighbours.size(); i++){//Sprawdzam wszystkie sąsiednie białe wierzchołki
                    if(current->neighbours[i]->pair->distance==INT_MAX){//Jezeli dystans pary białego wierzchołka to INT_MAX czyli dodatkowy wierzchołek
                        current->neighbours[i]->pair->distance=current->distance+1;//to ustawiam dystans białego wierzchołka na dystans aktualnego wierzchołka +1
                        q1.push(current->neighbours[i]->pair);//dodaję do kolejki parę białego wierzchołka
                    }
                }
            }
        }
        return dummy->distance!=INT_MAX;//Jeżeli dystans dodatkowego wierzchołka to nie INT_MAX to zwróć true - czyli da się stworzyć ścieżkę
    }


    bool dfs(node *current){
        if(current != dummy){//Jeżeli aktualny wierzchołek nie jest dodatkowym
            for(int i=0; i<current->neighbours.size(); i++){//sprawdzam wszystkich sąsiadów danego wierzchołka
                if(current->neighbours[i]->pair->distance==current->distance+1){//Jeżeli dystans pary danego białego wierzchołka jest równy dystansowi aktualnego +1
                    if(dfs(current->neighbours[i]->pair)){// jeżeli dfs pary białego zwróci true
                        current->neighbours[i]->pair=current;//biały wierzchołek jako parę ma aktualny wierzchołek
                        current->pair=current->neighbours[i];//czarny wierzchołek ma jako aktualny i-ty biały wierzchołęk
                        return true;//da się połączyć więc zwracam true
                    }
                }
            }
            current->distance=INT_MAX;//ustalam dystans aktualnego wierzchołka na INT_MAX
            return false;//nie da sie stworzyć ścieżki i zwracam true;
        }
        return true;//Aktualny wierrzchołek jest dodatkowy więc zwracam true
    }

    int hopcroftKarp(){
        int result=0;
        while(bfs()){//Dopóki da się stworzyć ścieżkę
            node *tmp=head;
            while(tmp!=nullptr){//sprawdzam wszystkie czarne wierzchołki
                if(!tmp->isWhite && tmp->pair==dummy && dfs(tmp)){//Jeżeli wierzchołek jest czarny i DFS zwróci true
                    result++;//to zwiększam wynik o 1
                }
                tmp=tmp->next;
            }
        }
        return ((chessboardSize*chessboardSize)-result);
    }


};

int main() {
    graph G1;
    cout << G1.hopcroftKarp() << endl;
    return 0;
}