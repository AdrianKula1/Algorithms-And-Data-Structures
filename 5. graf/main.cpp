#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node{//Struktura reprezentująca wierzchołek krafu
    bool Visited=0;//Znacznik czy wierzchołek jest odwiedzony
    int Level=0;//Warstwa na której znajduje się dany wierzchołek względem startowego
    //Startowy wierzchołek ma poziom 0. Sąsiadujące z nim mają poziom 1. Sąsiedzi sąsiadów mają poziomy 2 itd...
};

class graph{
private:
    int NoNodes, NoEdges, StartingNode, MaxLevel;
    vector<int>*AdjList;//Lista sąsiedztwa
    Node *Nodes;
    queue<int>q1;//Kolejka do BFS
public:
    graph(){
        cin >> NoNodes >> NoEdges >> StartingNode >> MaxLevel;//Pobieranie danych
        MaxLevel/=2;//Chcemy żeby wóz wrócił do początkowego miejsca czyli dzielimy drogę na 2 żeby mógł wrócić
        AdjList=new vector<int>[NoNodes];
        Nodes=new Node[NoNodes];

        for(int i=0; i<NoNodes; i++){//Każdy wioerzchołek jest nieodwiedzony
            Nodes[i].Visited=false;
        }

        int x, y;
        for(int i=0; i<NoEdges; i++){//Tworzę listę sąsiedztwa
            cin >> x >> y;
            AdjList[x].push_back(y);
            AdjList[y].push_back(x);
        }
    }

    void BFS(){
        int FacilitiesWithinMaxDistance=0, DistanceFromFirstNode=0;
        q1.push(StartingNode);//Wstawiam pierwszy wierzchołek do kolejki...
        Nodes[StartingNode].Visited=true;//...Oznaczam go jako odwiedzony...
        Nodes[StartingNode].Level=0;//...i poziom tego wierzchołka to 0 bo od niego zaczynam.

        int CurrentNode;
        while(!q1.empty()){//Dopóki kolejka nie będzie pusta
            CurrentNode=q1.front();
            q1.pop();//Zdejmij dany wierzchołek z kolejki
            if(CurrentNode!=StartingNode && Nodes[CurrentNode].Level<=MaxLevel){//Jeżeli samochód może dojechać do tego wierzchołka i z niego wrócić (i nie jest to wierzchołek startowy)...
                FacilitiesWithinMaxDistance++;//...to zwiększam liczbę placówek do której można dojechać (i wrócić z niej) o 1
            }
            for(int i=0; i<AdjList[CurrentNode].size(); i++){//Dodanie sąsiadów wierzchołka
                if(!Nodes[AdjList[CurrentNode][i]].Visited){//Jeżeli sąsiad jest jeszcze nieodwiedzony...
                    Nodes[AdjList[CurrentNode][i]].Visited=true;//...to go odwiedzam...
                    q1.push(AdjList[CurrentNode][i]);//...Dodaję do kolejki...
                    DistanceFromFirstNode=Nodes[CurrentNode].Level+1;//... i zwiększam poziom o 1 (im większy poziom tym dalej samochód musi dojechać)
                    Nodes[AdjList[CurrentNode][i]].Level=DistanceFromFirstNode;
                }
            }
        }
        cout <<  DistanceFromFirstNode << " " << FacilitiesWithinMaxDistance  << endl;
        //Wyświetlam ile godzin trwałaby podróż do najdalszej placówki (czyli najwyższy poziom wierzchołka) oraz  ile placówek znajduje się dostatecznie blisko, by samochód wrócił na czas.
    }
};

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    graph G1;
    G1.BFS();
    return 0;
}