#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

//ПУНКТ 1

void punkt1(){

    int N;
    int** matrix; //Указатель на указатель

    cout << "Введите N: ";
    cin >> N;

    while (!(N > 5 && N % 2 == 0)){ //N > 5 и чётное
        cout << "Попробуйте снова: ";
        cin >> N;
    }

    matrix = new int*[N]; //Массив указателей (строки)

    for (int i = 0; i < N; ++i){ //Выделяется память под массив указателей
        matrix[i] = new int[N];
    }


    cout << "Оригинальная матрица:" << endl;

    //Заполнение
    for (int i = 0; i < N; ++i){ //строки
        for (int j = 0; j < N; ++j){ //столбцы
            matrix[i][j] = -20 + rand() % 41;
            cout << setw(5) << matrix[i][j];
        }

        cout << endl;
    }


    int counter = 0; //всего замен
    int maxChange = 0;
    int numStolb = 0; //столбец с макс замен

    for (int j = 0; j < N; j++) {

        if ((j + 1) % 2 == 0) {

            int cur = 0;

            for (int i = 0; i < N; i++) {

                if (matrix[i][j] >= -10 && matrix[i][j] <= 10) { //диапазон [-10, 10]

                    matrix[i][j] = 0;

                    counter++;
                    cur++;
                }
            }

            if (cur > maxChange){

                maxChange = cur;
                numStolb = j + 1;
            }
        }
    }

    cout << "Модифицированная матрица:" << endl;

    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            cout << setw(5) << matrix[i][j];
        }

        cout << endl;
    }

    cout << "Всего заменено: " << counter << endl;
    cout << "Столб с наибольшим количеством изменений: " << numStolb << endl;

    for (int i = 0; i < N; ++i) { //очищение памяти
        delete[] matrix[i];
    }

    delete[] matrix;
}


//ПУНКТ 2

//  Сортировка в столбце (bubble) 
void sortirovka(int** matrix, int strok, int stolb) {

    for (int i = 0; i < strok - 1; i++) { 

        for (int j = 0; j < strok - i - 1; j++) {

            if (matrix[j][stolb] > matrix[j + 1][stolb]) { 

                int temp = matrix[j][stolb];

                matrix[j][stolb] = matrix[j + 1][stolb];

                matrix[j + 1][stolb] = temp;
            }
        }
    }
}


void punkt2() {

    int M, N;
    int** matrix; //указатель на указатель

    cout << "Введите M, N: ";
    cin >> M >> N;

    while (!(N > 5 && M > 5)){
        cout << "Повторите снова: ";
        cin >> M >> N;
    }
   
    matrix = new int*[M]; //массив указателей (строки)

    for (int i = 0; i < M; i++) { //для M строк
        matrix[i] = new int[N]; //N столбцов
    }

    cout << "Оригинальная матрица:" << endl;

    for (int i = 0; i < M; i++) { //строки
        for (int j = 0; j < N; j++) { //столбцы
            matrix[i][j] = -100 + rand() % 201;
            cout << setw(5) << matrix[i][j];
        }

        cout << endl;
    }

    int sortedStolbs = 0; //отсартированные столбцы
    int squared = 0; //столбцы с элементами ^ 2
    int replaceZero = 0; // столбцы с заменами на 0

    for (int j = 0; j < N; j++) {

        int plus = 0;
        int minus = 0;

        for (int i = 0; i < M; i++) {

            if (matrix[i][j] > 0)
                plus++;

            else if (matrix[i][j] < 0)
                minus++;
        }

        if (plus > minus) { 

            sortirovka(matrix, M, j);

            sortedStolbs++;
        }

        else if (minus > plus) {

            for (int i = 0; i < M; i++) {

                matrix[i][j] *= matrix[i][j];
            }

            squared++;
        }

        else {

            for (int i = 0; i < M; i++) {

                matrix[i][j] = 0;
            }

            replaceZero++;
        }
    }

    cout << "Модифицированная матрица:" << endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }

    cout << "Отсортировано стобцов: " << sortedStolbs << endl;
    cout << "Столбцы возведённые в квадрат: " << squared << endl;
    cout << "Заменено на нули: " << replaceZero << endl;

    for (int i = 0; i < M; i++) { //очищение памяти
        delete[] matrix[i];
    }

    delete[] matrix;
}


//Пункт 3
void punkt3() {
    int M, N;
    int** matrix;
    int** podmatrix;

    cout << "Введите M, N: ";
    cin >> M >> N;

    matrix = new int*[M]; //массив указателей (строки)
    podmatrix = new int*[M];

    for (int i = 0; i < M; i++) { //для M строк
        matrix[i] = new int[N]; //N столбцов
        podmatrix[i] = new int[N];
    }

/*
    cout << "Матрица:" << endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 2;;
            cout << matrix[i][j] << " ";
        }

        cout << endl;
    }
*/

    cout << "Введите элементы матрицы:" << endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> matrix[i][j];
        }
    } 


    int total = 0; //всего подматриц
    int maxSize = (M < N) ? M : N; //максимальная сторона
    int* count = new int[maxSize + 1]; //массив для квадратов


    for (int i = 0; i <= maxSize; i++) {
        count[i] = 0;
    }

    //Поиск квадратов
    for (int i = 0; i < M; i++) { //строка
        for (int j = 0; j < N; j++) { //столбец

            if (matrix[i][j] == 0) { //текущий 0 - подматрицы нет

                podmatrix[i][j] = 0;
            }

            else if (i == 0 || j == 0) { //если индекс = 0 (элемент на краю)

                podmatrix[i][j] = 1; //максимум 1 квадрат
            }

            else {

                int minimum = podmatrix[i - 1][j]; //сверху

                if (podmatrix[i][j - 1] < minimum) //слева
                    minimum = podmatrix[i][j - 1];

                if (podmatrix[i - 1][j - 1] < minimum) //диагональ
                    minimum = podmatrix[i - 1][j - 1];

                podmatrix[i][j] = minimum + 1; 
            }

            for (int k = 1; k <= podmatrix[i][j]; k++) {

                count[k]++; //индекс - сторона квадрата
                total++; //всего
            }
        }
    }

    cout << "Всего найдено " << total << " подматриц:" << endl;

    for (int i = 1; i <= maxSize; i++) {

        if (count[i] > 0) {

            cout << count[i] << " подматриц со стороной " << i << endl;
        }
    }

    for (int i = 0; i < M; i++) {

        delete[] matrix[i];
        delete[] podmatrix[i];
    }

    delete[] matrix;
    delete[] podmatrix;
    delete[] count;
}


int main(){
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    srand(time(nullptr));

    punkt1();
    cout << endl;
    punkt2();
    cout << endl;
    punkt3();

    return 0;
}