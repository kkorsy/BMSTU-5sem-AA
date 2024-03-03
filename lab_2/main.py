import numpy
from algorythms import classic, vinograd, vinograd_optimised
import matplotlib.pyplot as plt
import random
from time import process_time_ns


def inputMatrix():
    n, m = input('Введите количество строк: '), input('Введите количество столбцов: ')
    try:
        n = int(n)
        m = int(m)
    except ValueError:
        print('Введено не целое число\n')
        return []
    if n < 1 or m < 1:
        print('Число должно быть > 0')
        return []

    matrix = []
    print('Введите матрицу построчно:')
    for i in range(n):
        row = input().split()
        if len(row) != m:
            print('В строке неверное количество элементов\n')
            return []

        try:
            row = list(map(int, row))
        except ValueError:
            print('Элементы матрицы должны быть целыми числами\n')
            return []
        matrix.append(row)

    return matrix


def printMatrix(m):
    p = 7
    print(('+' + '-' * p) * len(m[0]) + '+')
    for i in range(len(m)):
        for j in range(len(m[0])):
            print('|{0:^{1}}'.format(m[i][j], p), end='')
        print('|')
    print(('+' + '-' * p) * len(m[0]) + '+')


def menu():
    print("Меню:\n"
          "1) Классический алгоритм\n"
          "2) Алгоритм Винограда\n"
          "3) Алгоритм Винограда (оптимизированный)\n"
          "4) Построить графики\n"
          "\n"
          "0) Выйти\n")
    x = input("Выберите пункт меню: ")
    try:
        x = int(x)
    except ValueError:
        print("")
        return -1
    return x


def callFunc(func):
    a = inputMatrix()
    b = inputMatrix()
    if len(a) == 0 or len(b) == 0:
        exit(1)
    c = func(a, b)
    printMatrix(c)


def generateMatrix(n):
    a, b = list([0] * n for _ in range(n)), list([0] * n for _ in range(n))
    for i in range(n):
        for j in range(n):
            a[i][j] = random.randint(0, 50)
            b[i][j] = random.randint(0, 50)

    return a, b


def timeMeasure(func, iseven):
    length, t = [], []
    if iseven:
        start, stop = 10, 101
    else:
        start, stop = 11, 102

    for ln in range(start, stop, 10):
        temp = 0
        n = 1000 if ln == 10 or ln == 11 else 100
        for _ in range(n):
            a, b = generateMatrix(ln)
            t1 = process_time_ns()
            func(a, b)
            t2 = process_time_ns()
            temp += t2 - t1
        length.append(ln)
        if ln == 10 or ln == 11:
            t.append(temp / (1000 * 1000000))
        else:
            t.append(temp / (100 * 1000000))
        print(f"\rDone: {ln}", end='')

    return length, t


def drawGraph():
    length1, t1 = timeMeasure(vinograd, True)
    length2, t2 = timeMeasure(vinograd_optimised, True)
    length3, t3 = timeMeasure(vinograd, False)
    length4, t4 = timeMeasure(vinograd_optimised, False)
    length5, t5 = timeMeasure(classic, True)
    length6, t6 = timeMeasure(classic, False)

    fig, ax = plt.subplots()
    ax.set_xlabel("Размер матрицы")
    ax.set_ylabel("Время, мс")

    labels_list = [
        "Алгоритм Винограда",
        "Алгоритм Винограда (оптимизированный)",
        "Классический алгоритм"
    ]

    print("Четные, Классический")
    for i in range(len(t1)):
        print("|{0:<5}|{1:<30}|".format(length1[i], t1[i]))
        print('-' * 38)
    print("Четные, Виноград")
    for i in range(len(t2)):
        print("|{0:<5}|{1:<30}|".format(length2[i], t2[i]))
        print('-' * 38)
    print("Четные, Виноград опт")
    for i in range(len(t5)):
        print("|{0:<5}|{1:<30}|".format(length5[i], t5[i]))
        print('-' * 38)
    print("Нечетные, Классический")
    for i in range(len(t3)):
        print("|{0:<5}|{1:<30}|".format(length3[i], t3[i]))
        print('-' * 38)
    print("Нечетные, Виноград")
    for i in range(len(t4)):
        print("|{0:<5}|{1:<30}|".format(length4[i], t4[i]))
        print('-' * 38)
    print("Нечетные, Виноград опт")
    for i in range(len(t6)):
        print("|{0:<5}|{1:<30}|".format(length6[i], t6[i]))
        print('-' * 38)

    markers = ['.', 'x', 'v']

    ax.plot(length1, t1, label=labels_list[2], marker=markers[0])
    ax.plot(length2, t2, label=labels_list[0], marker=markers[1])
    ax.plot(length5, t5, label=labels_list[1], marker=markers[2])
    ax.legend()
    plt.show()

    fig, ax = plt.subplots()
    ax.set_xlabel("Размер матрицы")
    ax.set_ylabel("Время, мкс")
    ax.plot(length3, t3, label=labels_list[2], marker=markers[0])
    ax.plot(length4, t4, label=labels_list[0], marker=markers[1])
    ax.plot(length6, t6, label=labels_list[1], marker=markers[2])
    ax.legend()
    plt.show()


if __name__ == '__main__':
    task = 1
    while task != 0:
        task = menu()
        if task == 1:
            callFunc(classic)
        elif task == 2:
            callFunc(vinograd)
        elif task == 3:
            callFunc(vinograd_optimised)
        elif task == 4:
            drawGraph()


