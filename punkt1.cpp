#include "task1_1.h"

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>


using namespace std;

void punkt1(){
    srand(time(nullptr));

    int N, M;

    cout << "enter N: ";
    cin >> N;
    
     if (!(N > 5 && N % 2 == 0)){
        cout << "Try again: ";
        cin >> N;
    }

    int** matrix = new int*[N];

    for (int i = 0; i < N; ++i){
        matrix[i] = new int[N];
    }


    cout << "Original matrix:" << endl;
    for (int i = 0; i < N; ++i){ //строки
        for (int j = 0; j < N; ++j){ //столбцы
            matrix[i][j] = -20 + rand() % 41;
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;


    int counter = 0;
    int maxChange = 0;
    int numStolb = 0;

    for (int j = 0; j < N; j++) {

        if ((j + 1) % 2 == 0) { //чётные столбцы

            int cur = 0;

            for (int i = 0; i < N; i++) {

                if (matrix[i][j] >= -10 && matrix[i][j] <= 10) { //диапазон от [-10, 10]

                    matrix[i][j] = 0;
                    counter++;
                    cur++;
                }
            if (cur > maxChange){
                maxChange = cur;
                numStolb = j+1;
            }
            }
        }
    }

    cout << "Modification matrix:" << endl;

    for (int i = 0; i < N; ++i){ //строки
        for (int j = 0; j < N; ++j){ //столбцы
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
    cout << "Number of changes: " << counter << endl;
    cout << "Stolb of max changes: " << numStolb << endl;


}