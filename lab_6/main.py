from time import process_time
from random import randint
import matplotlib.pyplot as plt

import numpy as np

from algorythms import full_combinations, ant_algorythm

menu = "1. Полный перебор \n\
2. Муравьиный алгоритм \n\
3. Параметризация \n\
4. Замерить время \n\
5. Сгенерировать новую матрицу \n\
0. Выход \n\n\
\
Выберите команду: \
"

LATEX = 0
CSV = 1
NORMAL = 2


def add_matrix_file():
    n = int(input("Введите размер матрицы: "))
    rb, re = map(int, input("Введите минимальное и максимальное значения для длин дуг: ").split())
    matrix = generate_matrix(n, rb, re)
    filename = input("Введите имя файла: ")
    f = open(filename, 'w')
    for i in range(n):
        for j in range(n):
            f.write(str(matrix[i][j]) + ' ')
        f.write('\n')
    f.close()


def generate_matrix(n, rs, re):
    matrix = np.zeros((n, n), dtype=int)
    for i in range(n):
        for j in range(n):
            if i == j:
                num = 0
            else:
                num = randint(rs, re)
            matrix[i][j] = num
    return matrix


def read_matrix(filename):
    try:
        file = open(filename, "r")
    except FileNotFoundError:
        print("Файла с указанным именем не существует\n")
        return []

    size = len(file.readline().split())
    file.seek(0)

    matrix = np.zeros((size, size), dtype=int)
    i = 0
    for line in file.readlines():
        j = 0
        for num in line.split():
            matrix[i][j] = int(num)
            j += 1
        i += 1
    file.close()

    return matrix


def print_matrix(matrix):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            print("%4d" % (matrix[i][j]), end="")
        print()


def parse_full_combinations():
    matrix = read_matrix(input("Введите имя файла: "))
    size = len(matrix)
    result = full_combinations(matrix, size)
    print("\n\nМинимальная сумма пути = ", result[0],
          "\nПуть: ", result[1])


def read_koefs():
    alpha = float(input("\n\nВведите коэффициент alpha: "))
    beta = 1 - alpha
    k_evaporation = float(input("Введите коэффициент испарения: "))
    days = int(input("Введите кол-во дней: "))

    return alpha, beta, k_evaporation, days


def parse_ant_alg():
    matrix = read_matrix(input("Введите имя файла: "))
    size = len(matrix)
    alpha, beta, k_evaporation, days = read_koefs()
    result = ant_algorythm(matrix, size, alpha, beta, k_evaporation, days)
    print("\n\nМинимальная сумма пути = ", result[0],
          "\nПуть: ", result[1])


def graph():
    size_start = int(input("\n\nВведите начальный размер матрицы: "))
    size_end = int(input("Введите конечный размер матрицы: "))

    time_full_combs = []
    time_ant = []

    sizes_arr = [i for i in range(size_start, size_end + 1)]

    cnt = 0

    for size in sizes_arr:
        cnt += 1
        t1, t2 = 0, 0
        n = 5000 if size < 7 else 10
        for i in range(n):
            matrix = generate_matrix(size, 1, 2)

            # Full combinations
            start = process_time()
            full_combinations(matrix, size)
            end = process_time()

            t1 += end - start

            # Ant algorythm
            start = process_time()
            ant_algorythm(matrix, size, 0.5, 0.5, 0.5, 250)
            end = process_time()

            t2 += end - start

        if size < 7:
            print(t1 / 5000)
            time_full_combs.append(t1 / 5000)
            time_ant.append(t2 / 5000)
        else:
            time_full_combs.append(t1 / 10)
            time_ant.append(t2 / 10)
        print("\rProgress: %3d%s" % ((cnt / len(sizes_arr)) * 100, "%"), end='')

    f_latex = open("latex_table.txt", "w")
    for i in range(len(sizes_arr)):
        f_latex.write("%4d & %10.6f & %10.6f \\\\ \\hline\n" % (sizes_arr[i], time_full_combs[i], time_ant[i]))
    f_latex.close()

    # Graph
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot()
    ax.plot(sizes_arr, time_full_combs, label="Полный перебор")
    ax.plot(sizes_arr, time_ant, label="Муравьиный алгоритм")

    plt.legend()
    plt.grid()
    plt.title("Временные характеристики")
    plt.ylabel("Затраченное время (с)")
    plt.xlabel("Размер матрицы")

    plt.show()


def parametrization(t=CSV):
    alpha_arr = [num / 10 for num in range(0, 11, 2)]
    k_eva_arr = [num / 10 for num in range(0, 11, 2)]
    days_arr = [1, 10, 50, 100, 500]

    size = 10

    matrix1 = read_matrix("matrP1.txt")
    matrix2 = read_matrix("matrP2.txt")
    matrix3 = read_matrix("matrP3.txt")

    optimal1 = full_combinations(matrix1, size)
    optimal2 = full_combinations(matrix2, size)
    optimal3 = full_combinations(matrix3, size)

    file1 = open("p_class1.txt", "w")

    count = 0
    count_all = len(alpha_arr) * len(k_eva_arr) * len(days_arr)

    for alpha in alpha_arr:
        beta = 1 - alpha

        for k_eva in k_eva_arr:

            for days in days_arr:
                res1 = ant_algorythm(matrix1, size, alpha, beta, k_eva, days)
                res2 = ant_algorythm(matrix2, size, alpha, beta, k_eva, days)
                res3 = ant_algorythm(matrix3, size, alpha, beta, k_eva, days)

                if t == LATEX:
                    sep = " & "
                    ender = " \\\\"
                elif t == CSV:
                    sep = ", "
                    ender = ""
                else:
                    sep = " | "
                    ender = ""

                m = max(res1[0] - optimal1[0], res2[0] - optimal2[0], res3[0] - optimal3[0])
                s = "%4.1f%s%4.1f%s%4d%s%5d%s%5d%s%5d%s%5d%s\n" \
                    % (alpha, sep, k_eva, sep, days, sep, res1[0] - optimal1[0], sep, res2[0] - optimal2[0], sep,
                       res3[0] - optimal3[0], sep, m, ender)
                file1.write(s)

                count += 1
                print("\rProgress: %3d%s" % ((count / count_all) * 100, "%"), end='')
    print()
    file1.close()


if __name__ == "__main__":
    cmd = int(input(menu))
    while cmd != 0:
        if cmd == 1:
            parse_full_combinations()
        elif cmd == 2:
            parse_ant_alg()
        elif cmd == 3:
            parametrization(LATEX)
        elif cmd == 4:
            graph()
        elif cmd == 5:
            add_matrix_file()
        else:
            print("\nВведена неверная команда\n")
        cmd = int(input(menu))
