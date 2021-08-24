#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Node {//Struktura reprezentująca wierzchołek w grafie
    int ID, Weight;
};
//Lista sąsiedztwa:
//Wierzchołek 0: {Wierzchołek 1, Waga 3}; {Wierzchołek 2, Waga 10}
//czyli z wierzchołka 0 do wierzchołka 1 waga to 3; z wierzchołka 0 do wierzchołka 2 waga to 10


struct Point {//Struktura reprezentująca punkt na układzie współrzędnych
    int ID, x, y;
};


bool CompareX(Point& A, Point& B) {//Porównanie do sortowania względem Y
    return A.x < B.x;
}

bool CompareY(Point& A, Point& B) {//Porównanie do sortowania względem Y
    return A.y < B.y;
}

struct ComparePQ{//Komparator kolejki priorytetowej. Wierzchołek o najmniejszej wadze zostaje na górze drzewa
    bool operator()(const Node& V1, const Node& V2)
    {
        return V1.Weight > V2.Weight;
    }
};


class Graph {
private:
    int NoVerticles;//Liczba wierzchołków
    Point* Points;//Tablica punktów
    vector<Node>* AdjList;//Lista sąsiedztwa
    priority_queue<Node, vector<Node>, ComparePQ> PQ;//kolejka priorytetowa do alg. dijkstry
    int* Distances;//Tablica odległości od punktu startowego
    int Start, End;//Punkt startowy i punkt końcowy
public:
    Graph() {
        int x, y;
        cin >> NoVerticles;
        AdjList = new vector<Node>[NoVerticles];
        Points = new Point[NoVerticles];
        Distances = new int[NoVerticles];
        for (int i = 0; i < NoVerticles; i++) {
            cin >> x >> y;
            Points[i] = { i, x, y };//Pobieram punkty do tablicy
            Distances[i] = 2147483647;//Ustawiam odległości na max int
        }
        cin >> Start >> End;
        CreateGraph();//Tworzę graf
        DijkstraAlgorithm();//Przechodzę graf algorytmem dijkstry
    }

    //Sortuję punkty względem x-ów potem dodaję do listy sąsiedztwa najbliższe punkty po lewej i po prawej w tablicy
    //Tak samo z Y
    void CreateGraph() {
        /*{ {0, 0}, {3, 1}, {3, 0}, {5, 3}, {5, 6}, {5, 10}, {10, 20}, {10, 31}, {100, 0}, {120, 31}}
		  Jeżeli i=4			czyli tu^		 to:
		*/
        //cout << "-------------------------------------------- X -------------------------------------------- " << endl;
        sort(Points, Points + NoVerticles, CompareX);//Sortuję punkty względem X
        bool TheSame = 0;
        for (int i = 0; i < NoVerticles; i++) {//Dla każdego wierzchołka
            int j = i - 1;
            int X = Points[i].x;
            while (X == Points[j].x && j > -1) {//dopóki X jest taki sam jak w i-tym punkcie to idę w lewo i dodaję te punkty do kolejki
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));//Wyliczam wagę krawędsi = min(większy X - mniejszy X, większy Y - większy Y)
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {//Przechodzę sąsiadów danego wierzchołka żeby nie dodać takich samych krawędzi
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {//Jeżeli krawędź się nie powtarza to dodaję ją do listy sąsiedztwa
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j--;
            }

            X = Points[j].x;
            while (X == Points[j].x && j > -1) {//Dodaję wszystkie sąsiadujące wierzchołki o następnym X mniejszym od i-tego punktu
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j--;
            }

            j = i + 1;
            X = Points[i].x;
            while (X == Points[j].x && j < NoVerticles) {//dopóki X jest taki sam jak w i-tym punkcie to idę w prawo i dodaję punkty sąsiednie
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j++;
            }

            X = Points[j].x;
            while (X == Points[j].x && j < NoVerticles) {//Dodaję wszystkie wierzchołki o następnym X większym od i-tego punktu
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j++;
            }
        }
        //cout << "-------------------------------------------- Y -------------------------------------------- " << endl;

        sort(Points, Points + NoVerticles, CompareY);//Sortuję względem Y
        for (int i = 0; i < NoVerticles; i++) {
            int j = i - 1;
            int X = Points[i].y;
            while (X == Points[j].y && j > -1) {//dodaję punkty w lewo względem takiego samego Y
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j--;
            }

            X = Points[j].y;
            while (X == Points[j].y && j > -1) {//dodaję punkty w lewo względem mniejszego Y
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j--;
            }

            j = i + 1;
            X = Points[i].y;
            while (X == Points[j].y && j < NoVerticles) {//dodaję punkty w lewo względem takiego samego Y
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j++;
            }

            X = Points[j].y;
            while (X == Points[j].y && j < NoVerticles) {//dodaję punkty w lewo względem większego Y
                int Weight = min((max(Points[i].x, Points[j].x) - min(Points[i].x, Points[j].x)), (max(Points[i].y, Points[j].y) - min(Points[i].y, Points[j].y)));
                TheSame = 0;
                for (int a = 0; a < AdjList[Points[i].ID].size(); a++) {
                    if (AdjList[Points[i].ID][a].ID == Points[j].ID && AdjList[Points[i].ID][a].Weight == Weight) {
                        TheSame = 1;
                        break;
                    }
                }
                if (!TheSame) {
                    AdjList[Points[i].ID].push_back({ Points[j].ID, Weight });
                }
                j++;
            }
        }
    }

    void DijkstraAlgorithm() {
        Distances[Start] = 0;//Odległość wierzchołka początkowego do siebie to 0
        PQ.push({Start, Distances[Start]});//Dodaję do kolejki ID punktu startowego wraz z odległością

        while (!PQ.empty()) {//Dopóki kolejka nie będzie pusta
            Node Current = PQ.top();//pobieram wierzchołek z góry
            PQ.pop();//usuwam wierzchołęk z kolejki
            for (int i = 0; i < AdjList[Current.ID].size(); i++) {//Dla każdego wierzchołka sąsiadującego z danym wierzchołkiem
                int AdjVerticleID = AdjList[Current.ID][i].ID, WeightToAdjVerticle = AdjList[Current.ID][i].Weight;
                if (Distances[AdjVerticleID] > Distances[Current.ID] + WeightToAdjVerticle) {//Jeżeli wierzchołek sąsiadujący już ma większą odległość niż odległość aktualnego wierzchołka+odległość z aktualnego to tego wierzchołka
                    Distances[AdjVerticleID] = Distances[Current.ID] + WeightToAdjVerticle;//to zastępuje odległość tą mniejszą
                    PQ.push({AdjVerticleID, Distances[AdjVerticleID]});//i dodaję ten wierzchołek do kolejki
                }
            }
        }
    }


    void GetAnswer() {//Zwracam odległość wierzchołka końcowego od wierzchołka początkowego
        cout << Distances[End] << endl;
    }
};


int main() {
    Graph G1;
    G1.GetAnswer();
}