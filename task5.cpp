#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Point { //точка рисунка
    int row;
    int col;
};

vector<Point> graph; //массив точек

int rowNow = 0;
int colNow = 0;

int it = 0; 

//смещения координат для 6 направлений
int rowMove[6] = {0, -1, -1, 0, 1, 1}; //вертикаль
int colMove[6] = {1, 1, -1, -1, -1, 1}; //горизонталь


//СМЕЩЕНИЕ
void step(){
    if (rowMove[it] == 0) //прямо
    {
        colNow += colMove[it]; 
        graph.push_back({rowNow, colNow});

    }
    else
    {
        rowNow += rowMove[it];
        colNow += colMove[it];

        graph.push_back({rowNow, colNow});
    }
}


//КОХА
void Koxa(int n)
{
    if (n == 0)
    {
        step();
        return;
    }

    Koxa(n - 1);

    //Направление в цикле 6 поворотов
    it = (it + 1) % 6;
    Koxa(n - 1);

    it = (it + 4) % 6;
    Koxa(n - 1);

    it = (it + 1) % 6;
    Koxa(n - 1);
}



int main()
{
    int n;

    cout << "N: "; //Циклы повтора
    cin >> n;

    graph.push_back({rowNow, colNow});

    Koxa(n);

    //НАЧАЛЬНЫЕ ЗНАЧЕНИЯ
    int minRow = graph[0].row;
    int maxRow = graph[0].row;

    int minCol = graph[0].col;
    int maxCol = graph[0].col;

    for (Point p : graph){
        if (p.row < minRow) minRow = p.row;
        if (p.row > maxRow) maxRow = p.row;

        if (p.col < minCol) minCol = p.col;
        if (p.col > maxCol) maxCol = p.col;
    }


    //ЗАПОЛНЕНИЕ И ВЫВОД
    vector<string> printGraph(
        maxRow - minRow + 1,
        string(maxCol - minCol + 1, ' ')
    );

    for (Point p : graph){
        printGraph[p.row - minRow][p.col - minCol] = '`';
    }

    for (const string& line : printGraph){
        cout << line << '\n';
    }

    return 0;
}