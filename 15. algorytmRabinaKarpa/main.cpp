#include <iostream>
#include <vector>

using namespace std;

class RabinKarp{
private:
    vector<long long int> numbers, sequence;
    long long int sequenceHash = 0;
    vector<long long int> fibonacci;
    long long int d=101, q=47;// d jest zdefiniowane w zadaniu, q = jakaś liczba pierwsza
    long long int h=1;//

public:
    RabinKarp(){
        int noPlanets, sizeOfSubsequence;
        cin >> noPlanets;
        int number=0;
        for(int i=0; i< noPlanets; i++){// pobieram główną sekwencję
            cin >> number;
            numbers.push_back(number);
        }

        cin >> sizeOfSubsequence;
        for(int i=0; i<sizeOfSubsequence; i++){//pobieram subsekwencję do znalezienia
            cin >> number;
            sequence.push_back(number);
        }



        for(int i = 1; i <= sizeOfSubsequence-1; i++) {// H = d^(M-1) % 47
            h = h * d;
        }

        h%=q;


        calculateSequenceHash(0, 0);//Wyliczam hash subsekwencji
        findIndexesOfHash(calculateHash(0, 0), 0, sequence.size());//Wyliczam miejsca w których znajdę subsekwencję


        long long int value=0;
        int i=0;
        while(value<numbers.size()){//Wykorzystując fibonacciego, wyliczamindeks pierwszej sekwencji wychodzącej poza zakres
            value=fibonacci[i]+fibonacci[i+1];
            fibonacci.push_back(value);
            i++;
        }
        cout << (value - numbers.size())<< endl;
    }

    //sequenceHash = (d*PreviousResult+liczbaw danym miejscu w subsekwencji) % q
    //wyliczam hash z jedną liczbą podem z dwiema, z trzema itd....
    void calculateSequenceHash(long long int previousResult, long long int iteration){
        if(iteration >= sequence.size()){
            this->sequenceHash = previousResult;
            return;
        }else{
            long long int result = (d*previousResult+sequence[iteration])%q;
            calculateSequenceHash(result, (iteration+1));
        }
    }

    //Korzystając ze wzorów, wyliczam hasha dla kolejnych liczb
    //Zamiast wyliczać po kolei dla wszystkich liczb
    //po prostu odejmuję hasha liczby o indeksie lewym i dodaję hasha liczby o indeksie prawym
    void findIndexesOfHash(long long int currentResult, unsigned int leftIndex, unsigned int rightIndex){
        currentResult = (d*(currentResult-(numbers[leftIndex]*h))+numbers[rightIndex])%47;

        if(currentResult<0){
            currentResult+=q;
        }

        if(currentResult==sequenceHash){
            if(checkConflict(leftIndex+1, rightIndex)){//dodaje indeks znalezionego sekwencji
                fibonacci.push_back(leftIndex+1);
                if(fibonacci.size()==2){//jeżeli znalazłem dwa indeksy to kończę tę funkcję
                    return;
                }
            }
        }
        if(rightIndex+1 < numbers.size()) {
            findIndexesOfHash(currentResult, leftIndex + 1, rightIndex + 1);
        }

    }

    //wyliczam hash dla pierwszych cyfr w sekwencji
    long long int calculateHash(long long int previousResult, int iteration){
        if(iteration >= sequence.size()){
            if(previousResult==sequenceHash && checkConflict(0, sequence.size())){//jeżeli hash i liczby części sekwencji zgadza sie z hashem subsekwencji i  to dodaję indeks 0 do fibonacciego
                fibonacci.push_back(0);
            }
            return previousResult;
        }else{
            long long int result = (d*previousResult+numbers[iteration])%q;
            return calculateHash(result, ++iteration);
        }
    }

    bool checkConflict(unsigned int firstIndex, unsigned int lastIndex){//sprawdzam konflikt - iteruje po każdej liczbie znalezionej sekwencji i subsekwencji. Jeżeli liczby sie zgadzają zwracam true
        for(unsigned int i=firstIndex, j=0; i<lastIndex; i++, j++){
            if(numbers[i]!=sequence[j]){
                return false;
            }
        }
        return true;
    }
};

int main() {
    RabinKarp alg1;
    return 0;
}