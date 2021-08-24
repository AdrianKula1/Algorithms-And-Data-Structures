#include <iostream>
#include <list>
#include <cmath>
#include <algorithm>
#include <stack>

using namespace std;

class algorithm{
private:
    struct point{//struktura reprezentująca punkt. Ma wspolrzedne x, y oraz kąt jaki tworzy z punktem P
        int x, y;
        double angle;
        point(int x, int y, double angle){
            this->x=x;
            this->y=y;
            this->angle=angle;
        }
        point()=default;
    };
    list<point> pointsList;
    point P = {2147483647, 2147483647, 0};//punkt P - najmniejszy Y i najmniejszy X
    stack<point> S1; //stos punktów wykorzystywany do zrobienia otoczki

    struct comparator{//komparator sortujący punkty względem kątów lub - jeżeli te są równe - względem x-ów i y-ów
        bool operator ()(const point & P1, const point & P2){
            if(P1.angle == P2.angle){
                if(P1.y==P2.y){
                    return P1.x>P2.x;
                }
                return P1.y > P2.y;

            }else{
                return P1.angle > P2.angle;
            }
        }
    };

public:
    algorithm(){
        int noPoints, x, y;
        cin >> noPoints;
        for(int i=0; i<noPoints; i++){//pobieram punkty szukając odrazu punktu P
            cin >> x >> y;
            if((y<P.y) || (y==P.y && x<P.x)){
                P={x, y, 0};
            }
            pointsList.emplace_back(x, y, 0);
        }


        auto pred = [this](const point & P1) {
            return ((P1.x == P.x) && (P1.y==P.y));
        };
        auto it = std::find_if(std::begin(pointsList), std::end(pointsList), pred);//Szukam iteratora na punkt P w liście..
        pointsList.erase(it);//...i usuwam go z listy

        polarSort();//sortuję polarnie punkty
        createShell();//Tworzę otoczke z odpowiednich punktów
        calculateSurface();//Obliczam pole powstałej figury
    }

    void calculateSurface(){
        point P1, P2;
        P2 = S1.top();
        S1.pop();
        P1 = S1.top();
        S1.pop();
        //zbiertam pierwsze dwa punkty ze stosu
        double result = 0;
        bool x = true;
        while(!S1.empty()){
            result += surface(P, P1, P2);//wyliczam pole każdej figury (trójkąta) z dwóch punktów z listy i punktu P

            if(x){
                P2=S1.top();
            }else{
                P1=S1.top();
            }
            x=!x;
            S1.pop();
        }
        cout.precision(3);
        cout << fixed << result << endl;
    }

    double surface(point P1, point P2, point P3){//za pomocą wzoru wyliczam i zwracam pole trójkąta z trzech punktów
        double side1 = calculateLength(P1, P2);
        double side2 = calculateLength(P2, P3);
        double side3 = calculateLength(P1, P3);
        return (sqrt(((side1+side2+side3)*(side1+side2-side3)*(side1-side2+side3)*(((-1)*side1)+side2+side3)))/4);
    }

    void createShell(){//tworze otoczke z punktów
        S1.push(P);//wpycham na stos punkt P
        auto it = pointsList.begin();
        S1.push(*it);//wpycham na stos pierwszy z punktów
        point P1, P2;
        auto end=pointsList.end();
        end--;
        while(1){
            if(it==end){
                break;
            }
            P2=S1.top();
            S1.pop();
            P1=S1.top();
            S1.pop();
            S1.push(P1);
            S1.push(P2);

            it++;


            if(det(P1, P2, (*it)) >= 0){//jeżeli wyznacznik macierzy z danych trzech punktów jest >=0 to zapamiętuje ten punkt
                S1.push((*it));
            }else{//Jeżeli wyznacznik jest <0- to popuje ten punkt i przechodze do następnego
                it--;
                S1.pop();
            }

        }
    }

    double det(point P1, point P2, point P3){//Wyliczam wyznacznik macierzy
        double result = (P1.x*P2.y*1 + P1.y*1*P3.x + P2.x*P3.y*1)-
                (1*P2.y*P3.x + P1.y*P2.x*1 + P1.x*1*P3.y);
        return result;
    }

    double calculateLength(point P1, point P2){//wyliczam odległość między dwoma punktami
        return sqrt(pow(P2.x - P1.x, 2) + pow(P2.y - P1.y, 2) * 1.0);
    }


    void calculateDegrees(){//wyliczam kąt między punktem P a każdym innym
        for (auto it = pointsList.begin(); it != pointsList.end(); ++it){
            it->angle = atan2(it->x-P.x, it->y-P.y);
        }
    }

    void polarSort(){
        calculateDegrees();//wyliczam kąty każdego punktu względem punktu P
        pointsList.sort(comparator());//sortuję punkty względem kątów lub - jeżeli kąty są równe - względem x lub y
        pointsList.unique([](point P1, point P2){//zostawiam pierwsze punkty o różnych kątach
            return P1.angle==P2.angle;
        });
    }



};

int main() {
    algorithm a1;
    return 0;
}