#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

const int N = 4; //кол-во уравнений и решений
const double EPS = 0.001; //точность

//ВЫВОД МАТРИЦЫ
void printMatrix(double matrix[N][N + 1]) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= N; j++) {
            cout << setw(12) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


//Перестановка строк
void swapStrok(double matrix[N][N + 1], int r1, int r2) { //матрица, строки которые нужно поменять

    for (int j = 0; j <= N; j++) {
        double cur = matrix[r1][j];
        matrix[r1][j] = matrix[r2][j];
        matrix[r2][j] = cur;
    }
}


//Метод Гаусса с выбором главного элемента
void gauss() {

    double matrix[N][N + 1] = { //Расширенная матрица
        {0.93, -0.04, 0.21, -1.16, -1.24}, // (M)0.93x1 - 0.04x2 + 0.21x3 - 1.16x4 = -1.24
        {0.25, -1.23, 0.07, -0.09, -0.84}, // 0.25x1 - 1.23x2 + (N)0.07x3 - 0.09x4 = -0.84(P)
        {-0.21, 0.07, 0.8, -0.13, 2.56}, // -0.21x1 + (N)0.07x2 + 0.8x3 - 0.13x4 = 2.56
        {0.15, -1.31, 0.06, -0.84, 0.93} // 0.15x1 - 1.31x2 + 0.06x3 - (P)0.84x4 = 0.93(M)
    };

    double x[N]; //массив корней

    cout << "\nМЕТОД ГАУССА\n";

    cout << "\nИсходная расширенная матрица:" << endl;
    printMatrix(matrix);

    //Прямой ход (зануляет по диагонали)
    for (int i = 0; i < N; i++) { 

        int maxEl = i; //макс эл в столбце

        //поиск макс эл
        for (int k = i + 1; k < N; k++) {
            if (fabs(matrix[k][i]) > fabs(matrix[maxEl][i])) {
                maxEl = k;
            }
        }

        if (maxEl != i) { //если максимальный ниже текущего
            swapStrok(matrix, i, maxEl); //поменять строки местами
        }

        //зануляем элементы ниже главной диагонали
        for (int k = i + 1; k < N; k++) {

            double coef = matrix[k][i] / matrix[i][i]; // Aik/Akk

            //вычитание строк
            for (int j = i; j <= N; j++) {

                matrix[k][j] -= coef * matrix[i][j]; //Bk = Bk - coef * Bi
            }
        }
    }

    cout << "\nМатрица после прямого хода: " << endl;
        printMatrix(matrix);


    // Обратный ход
    for (int i = N - 1; i >= 0; i--) { //ход с конца

        x[i] = matrix[i][N]; //свободный элемент

        for (int j = i + 1; j < N; j++) { //для предыдущих строк

            x[i] -= matrix[i][j] * x[j]; 
        }

        x[i] /= matrix[i][i]; //корени
    }

    cout << "\nКорни уравнения с помощью метода Гаусса:" << endl;

    for (int i = 0; i < N; i++) {

        cout << "x" << i + 1 << " = " << x[i] << endl;
    }
}


//Метод Зейделя
void zeidel() {

    double matrix[N][N + 1] = {
        {0.93, -0.04, 0.21, -1.16, -1.24},
        {0.25, -1.23, 0.07, -0.09, -0.84},
        {-0.21, 0.07, 0.8, -0.13, 2.56},
        {0.15, -1.31, 0.06, -0.84, 0.93}
    };

    double x[N] = {0}; //массив корней
    double oldX[N]; //старые корни до уточнения

    double eps;

    int it = 0; //номер итерации 

    cout << "МЕТОД ЗЕЙДЕЛЯ:" << endl;

    //Таблицы
    cout << setw(5)  << "N"
         << setw(12) << "X1"
         << setw(12) << "X2"
         << setw(12) << "X3"
         << setw(12) << "X4"
         << setw(12) << "En"
         << endl;

    do {

        //сохраняем старые значения
        for (int i = 0; i < N; i++) {
            oldX[i] = x[i];
        }

        //вычисление новых x
        for (int i = 0; i < N; i++) {

            double slu = matrix[i][N];
            
            for (int j = 0; j < N; j++) {

                if (j != i) {

                    slu -= matrix[i][j] * x[j]; //выражение x
                }
            }
            x[i] = slu / matrix[i][i]; //выражение x
        }
        
        //Уточнение
        eps = fabs(x[0] - oldX[0]); //новый x - старый x

        for (int i = 1; i < N; i++) {

            if (fabs(x[i] - oldX[i]) > eps) {
                eps = fabs(x[i] - oldX[i]);
            }
        }

        it++;

        //Вывод корней
        cout << setw(5)  << it 
            << setw(12) << x[0] 
            << setw(12) << x[1] 
            << setw(12) << x[2] 
            << setw(12) << x[3]
            << setw(12) << eps
            << endl;


    } while (eps > EPS); //уточнение до момента, пока текущий больше заданного

    cout << "\nКорни:" << endl;

    for (int i = 0; i < N; i++) {

        cout << "x" << i + 1 << " = " << x[i] << endl;
    }
}


int main() {

    cout << fixed << setprecision(6);


    cout << "Система уравнений:" << endl;

    cout << "0.93x1 - 0.04x2 + 0.21x3 - 1.16x4 = -1.24" << endl;
    cout << "0.25x1 - 1.23x2 + 0.07x3 - 0.09x4 = -0.84" << endl;
    cout << "-0.21x1 + 0.07x2 + 0.8x3 - 0.13x4 = 2.56" << endl;
    cout << "0.15x1 - 1.31x2 + 0.06x3 - 0.84x4 = 0.93" << endl;

    gauss();
    cout << endl;
    zeidel();

    return 0;
}