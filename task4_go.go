package main

import (
	"fmt"
	"math"
)

var ( //Глобальные переменные
	N, M int

	matrix [][]int
	board  [][]int
	target [][]int

	row1 []int
	col1 []int

	row2 []int
	col2 []int

	moveCount  int
	bestAnswer = math.MaxInt32
)

// создание шаблона
func goal(up, down int) {
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			if i < N/2 {
				target[i][j] = up
			} else {
				target[i][j] = down
			}
		}
	}
}

// соседние пар
func variation() {
	moveCount = 0

	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {

			// справа
			if j+1 < M {
				row1[moveCount] = i
				col1[moveCount] = j

				row2[moveCount] = i
				col2[moveCount] = j + 1

				moveCount++
			}

			// снизу
			if i+1 < N {
				row1[moveCount] = i
				col1[moveCount] = j

				row2[moveCount] = i + 1
				col2[moveCount] = j

				moveCount++
			}
		}
	}
}

// проверка достижения цели
func check() bool {
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			if board[i][j] != target[i][j] {
				return false
			}
		}
	}
	return true
}

// переворот пары
func makeMove(id int) {
	board[row1[id]][col1[id]] ^= 1
	board[row2[id]][col2[id]] ^= 1
}

// восстановление исходной доски
func resetBoard() {
	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			board[i][j] = matrix[i][j]
		}
	}
}

// полный перебор
func solve(currentMove, usedMoves int) {

	if usedMoves >= bestAnswer {
		return
	}

	if check() {
		bestAnswer = usedMoves
		return
	}

	if currentMove == moveCount {
		return
	}

	solve(currentMove+1, usedMoves)

	makeMove(currentMove)

	solve(currentMove+1, usedMoves+1)

	makeMove(currentMove)
}

func main() {

	fmt.Print("Введите N (чётное) и M: ")
	fmt.Scan(&N, &M)

	for N%2 != 0 {
		fmt.Print("N должно быть чётным. Введите снова: ")
		fmt.Scan(&N, &M)
	}

	matrix = make([][]int, N)
	board = make([][]int, N)
	target = make([][]int, N)

	for i := 0; i < N; i++ {
		matrix[i] = make([]int, M)
		board[i] = make([]int, M)
		target[i] = make([]int, M)
	}

	maxMoves := 2 * N * M

	row1 = make([]int, maxMoves)
	col1 = make([]int, maxMoves)

	row2 = make([]int, maxMoves)
	col2 = make([]int, maxMoves)

	fmt.Println("\nВведите состояние доски:")

	for i := 0; i < N; i++ {
		for j := 0; j < M; j++ {
			fmt.Scan(&matrix[i][j])
			board[i][j] = matrix[i][j]
		}
	}

	variation()

	// верх 0, низ 1
	goal(0, 1)
	solve(0, 0)

	// верх 1, низ 0
	resetBoard()
	goal(1, 0)
	solve(0, 0)

	if bestAnswer == math.MaxInt32 {
		fmt.Println("\nРешения нет")
	} else {
		fmt.Println("\nРезультат:", bestAnswer)
	}
}
