package main

import (
	"fmt"
	"math"
)

const COUNT = 4
const EPS = 0.001

const M = 0.93
const N = 0.07
const P = -0.84

// ВЫВОД МАТРИЦЫ
func printMatrix(matrix [COUNT][COUNT + 1]float64) {

	for i := 0; i < COUNT; i++ {

		for j := 0; j <= COUNT; j++ {
			fmt.Printf("%12.6f ", matrix[i][j]) //%f - float, .6 - знаки после запятой, 12 - ширина поля
		}

		fmt.Println()
	}

	fmt.Println()
}

// ПЕРЕСТАНОВКА СТРОК
func swapStroks(matrix *[COUNT][COUNT + 1]float64, r1 int, r2 int) {

	for j := 0; j <= COUNT; j++ {

		cur := matrix[r1][j]
		matrix[r1][j] = matrix[r2][j]
		matrix[r2][j] = cur
	}
}

// МЕТОД ГАУССА
func gauss() {

	matrix := [COUNT][COUNT + 1]float64{
		{M, -0.04, 0.21, -1.16, -1.24},
		{0.25, -1.23, N, -0.09, P},
		{-0.21, N, 0.8, -0.13, 2.56},
		{0.15, -1.31, 0.06, P, M},
	}

	var x [COUNT]float64

	fmt.Println("\nМЕТОД ГАУССА")

	fmt.Println("\nИсходная расширенная матрица:")
	printMatrix(matrix)

	// Прямой ход
	for i := 0; i < COUNT; i++ {

		maxEl := i

		// поиск максимального элемента
		for k := i + 1; k < COUNT; k++ {

			if math.Abs(matrix[k][i]) > math.Abs(matrix[maxEl][i]) {
				maxEl = k
			}
		}

		if maxEl != i {
			swapStroks(&matrix, i, maxEl)
		}

		// зануление элементов
		for k := i + 1; k < COUNT; k++ {

			coef := matrix[k][i] / matrix[i][i]

			for j := i; j <= COUNT; j++ {

				matrix[k][j] -= coef * matrix[i][j]
			}
		}
	}

	fmt.Println("\nМатрица после прямого хода:")
	printMatrix(matrix)

	// Обратный ход
	for i := COUNT - 1; i >= 0; i-- {

		x[i] = matrix[i][COUNT]

		for j := i + 1; j < COUNT; j++ {

			x[i] -= matrix[i][j] * x[j]
		}

		x[i] /= matrix[i][i]
	}

	fmt.Println("\nКорни уравнения с помощью метода Гаусса:")

	for i := 0; i < COUNT; i++ {

		fmt.Printf("x%d = %.6f\n", i+1, x[i]) //%d - int
	}
}

// МЕТОД ЗЕЙДЕЛЯ
func zeidel() {

	matrix := [COUNT][COUNT + 1]float64{
		{M, -0.04, 0.21, -1.16, -1.24},
		{0.25, -1.23, N, -0.09, P},
		{-0.21, N, 0.8, -0.13, 2.56},
		{0.15, -1.31, 0.06, P, M},
	}

	var x [COUNT]float64
	var oldX [COUNT]float64

	eps := 0.0
	it := 0

	fmt.Println("\nМЕТОД ЗЕЙДЕЛЯ:")

	fmt.Printf("%5s%12s%12s%12s%12s%12s\n",
		"№", "X1", "X2", "X3", "X4", "En")

	for {

		// сохраняем старые значения
		for i := 0; i < COUNT; i++ {
			oldX[i] = x[i]
		}

		// вычисление новых x
		for i := 0; i < COUNT; i++ {

			slu := matrix[i][COUNT]

			for j := 0; j < COUNT; j++ {

				if j != i {

					slu -= matrix[i][j] * x[j]
				}
			}

			x[i] = slu / matrix[i][i]
		}

		// уточнение
		eps = math.Abs(x[0] - oldX[0])

		for i := 1; i < COUNT; i++ {

			if math.Abs(x[i]-oldX[i]) > eps {
				eps = math.Abs(x[i] - oldX[i])
			}
		}

		it++

		fmt.Printf("%5d%12.6f%12.6f%12.6f%12.6f%12.6f\n",
			it, x[0], x[1], x[2], x[3], eps)

		if eps <= EPS {
			break
		}
	}

	fmt.Println("\nКорни:")

	for i := 0; i < COUNT; i++ {

		fmt.Printf("x%d = %.6f\n", i+1, x[i])
	}
}

func main() {

	fmt.Println("Система уравнений:")

	fmt.Printf("%.2fx1 - 0.04x2 + 0.21x3 - 1.16x4 = -1.24\n", M)

	fmt.Printf("0.25x1 - 1.23x2 + %.2fx3 - 0.09x4 = %.2f\n",
		N, P)

	fmt.Printf("-0.21x1 + %.2fx2 + 0.8x3 - 0.13x4 = 2.56\n",
		N)

	fmt.Printf("0.15x1 - 1.31x2 + 0.06x3 + %.2fx4 = %.2f\n",
		P, M)

	gauss()

	fmt.Println()

	zeidel()
}
