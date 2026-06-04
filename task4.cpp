#include <iostream>
#include <climits>

using namespace std;


//ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ 

int N, M; //размер

int** matrix; //исходная доска 
int** board; //копия
int** target; //шаблон цели

// МАССИВЫ КООРДИНАТОВ ЭЛЕМЕНТОВ
int* row1;
int* col1;

int* row2;
int* col2;

//ПЕРЕМЕННЫЕ ДЛЯ ВЫЧИСЛЕНИЯ
int moveCount;
int bestAnswer = INT_MAX;


//СОЗДАНИЕ ШАБЛОНА
void goal(int up, int down){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){

            if(i < N / 2)
                target[i][j] = up;
            else
                target[i][j] = down;
        }
    }
}


//СОСЕДНИЕ ПАРЫ
void variation(){ 

    moveCount = 0;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){

            if(j + 1 < M){ //пара справа

                row1[moveCount] = i;
                col1[moveCount] = j;

                row2[moveCount] = i;
                col2[moveCount] = j + 1;

                moveCount++;
            }

            if(i + 1 < N){ //пара снизу

                row1[moveCount] = i;
                col1[moveCount] = j;

                row2[moveCount] = i + 1;
                col2[moveCount] = j;

                moveCount++;
            }
        }
    }
}


//ПРОВЕРКА ДОСТИЖЕНИЯ ЦЕЛИ
bool check(){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){

            if(board[i][j] != target[i][j])
                return false;
        }
    }
    return true;
}


//ПЕРЕВОРОТ ПАР
void makeMove(int id){
    board[row1[id]][col1[id]] ^= 1; 
    board[row2[id]][col2[id]] ^= 1;
}


//ВОССТАНОВЛЕНИЕ ИСХОДНОЙ ДОСКИ
void resetBoard(){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            board[i][j] = matrix[i][j];
        }
    }
}


//ПОЛНЫЙ ПЕРЕБОР
void solve(int currentMove, int usedMoves){

    if(usedMoves >= bestAnswer) //если больше
        return;

    if(check()){ //если цель достигнута
        bestAnswer = usedMoves; //перезаписать результат
        return;
    }

    if(currentMove == moveCount) //если рекурсия дошла до последнего шага
        return; 

    solve(currentMove + 1, usedMoves);  

    //использовать текущий ход
    makeMove(currentMove);

    solve(currentMove + 1, usedMoves + 1); //добавить использованный ход

    makeMove(currentMove); //использовать ход
}


int main(){

    //СОЗДАНИЕ ДОСКИ И ДОП ПЕРЕМЕННЫХ
    cout << "Введите N (чётное) и M: ";
    cin >> N >> M;

    while(N % 2 != 0){
        cout << "N должно быть чётным. Введите снова: ";
        cin >> N >> M;
    }

    matrix = new int*[N];
    board = new int*[N];
    target = new int*[N];

    for(int i = 0; i < N; i++){
        matrix[i] = new int[M];
        board[i] = new int[M];
        target[i] = new int[M];
    }

    int maxMoves = 2 * N * M; //сколько максимум пар

    row1 = new int[maxMoves];
    col1 = new int[maxMoves];
    row2 = new int[maxMoves];
    col2 = new int[maxMoves];

    cout << "\nВведите состояние доски:\n";

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cin >> matrix[i][j];
            board[i][j] = matrix[i][j];
        }
    }

    //ПЕРЕБОР

    variation(); 

    //цель - верхняя половина 0, нижняя 1
    goal(0, 1);
    solve(0, 0); //начальное состояние 0, 0

    //цель - верхняя половина 1, нижняя 0
    resetBoard();
    goal(1, 0);
    solve(0, 0);

    if(bestAnswer == INT_MAX){
        cout << "\nРешения нет\n";
    }
    
    else{
        cout << "\nРезультат: " << bestAnswer << endl;
    }


    //ОЧИЩЕНИЕ ПАМЯТИ
    for(int i = 0; i < N; i++){
        delete[] matrix[i];
        delete[] board[i];
        delete[] target[i];
    }

    delete[] matrix;
    delete[] board;
    delete[] target;

    delete[] row1;
    delete[] col1;
    delete[] row2;
    delete[] col2;

    return 0;
}