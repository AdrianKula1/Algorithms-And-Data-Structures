#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main() {
    double x=0, y=0;
    double  h=0, result=0, average=0;
    cin >> x >> y;
    srand(time(NULL));
    double depth=0;
    int i=0;
    for(i=0; i<10000000; i++){
        double randX=fRand(0, x), randY=fRand(0, y);//dwa losowe punkty w obszarze x, y
        depth = d(randX, randY);
        if(depth>0){
            average+=depth;
        }
    }
    h = average/i;
    result = x*y*h;
    cout.precision(0);
    cout << fixed << result << endl;
    return 0;
}