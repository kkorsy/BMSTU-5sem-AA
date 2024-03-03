import random
from time import process_time_ns
from sort import block_sort, smooth_sort, merge_sort
import matplotlib.pyplot as plt


def menu():
    print("Меню:\n"
          "1) Блочная сортировка\n"
          "2) Плавная сортировка\n"
          "3) Сортировка слиянием\n"
          "4) Построить графики\n\n"
          "0) Выход\n")
    t = input("Введите пункт меню: ")
    try:
        t = int(t)
    except ValueError:
        print()
        return 0
    return t


def input_array():
    a = input("Введите элементы массива через пробел\n")
    try:
        a = list(map(int, a.split()))
    except ValueError:
        print("Элементы массива должны быть целыми числами\n")
        return None
    return a


def generateArray(length, type):
    a = []
    if type == 1:
        for i in range(length):
            a.append(random.randint(0, 100))
    elif type == 2:
        for i in range(length):
            a.append(i)
    elif type == 3:
        for i in range(length, -1, -1):
            a.append(i)
    return a


def timeMeasure(func):
    length_rand, t_rand = [], []
    length_sort, t_sort = [], []
    length_rev, t_rev = [], []
    for ln in range(100, 501, 50):
        temp1, temp2, temp3 = 0, 0, 0
        for _ in range(5000):
            a = generateArray(ln, 1)
            t1 = process_time_ns()
            func(a)
            t2 = process_time_ns()
            temp1 += t2 - t1

            a = generateArray(ln, 2)
            t1 = process_time_ns()
            func(a)
            t2 = process_time_ns()
            temp2 += t2 - t1

            a = generateArray(ln, 3)
            t1 = process_time_ns()
            func(a)
            t2 = process_time_ns()
            temp3 += t2 - t1
        length_rand.append(ln)
        t_rand.append(temp1 / (5000 * 1000000))

        length_sort.append(ln)
        t_sort.append(temp2 / (5000 * 1000000))

        length_rev.append(ln)
        t_rev.append(temp3 / (5000 * 1000000))

        print(f"\rDone: {ln}", end='')

    return length_rand, t_rand, length_sort, t_sort, length_rev, t_rev


def drawGraph():
    length1_rand, t1_rand, length1_sort, t1_sort, length1_rev, t1_rev = timeMeasure(block_sort)
    length2_rand, t2_rand, length2_sort, t2_sort, length2_rev, t2_rev = timeMeasure(smooth_sort)
    length3_rand, t3_rand, length3_sort, t3_sort, length3_rev, t3_rev = timeMeasure(merge_sort)

    fig, ax = plt.subplots()
    ax.set_xlabel("Размер массива")
    ax.set_ylabel("Время, мс")

    labels_list = [
        "Блочная сортировка",
        "Плавная сортировка",
        "Сортировка слиянием"
    ]

    print("Блочная rand")
    for i in range(len(t1_rand)):
        print("|{0:<5}|{1:<30}|".format(length2_rand[i], t2_rand[i]))
        print('-' * 38)
    print("Плавная rand")
    for i in range(len(t2_rand)):
        print("|{0:<5}|{1:<30}|".format(length1_rand[i], t1_rand[i]))
        print('-' * 38)
    print("Слиянием rand")
    for i in range(len(t1_rand)):
        print("|{0:<5}|{1:<30}|".format(length3_rand[i], t3_rand[i]))
        print('-' * 38)

    print("Блочная sort")
    for i in range(len(t1_rand)):
        print("|{0:<5}|{1:<30}|".format(length3_sort[i], t3_sort[i]))
        print('-' * 38)
    print("Плавная sort")
    for i in range(len(t2_rand)):
        print("|{0:<5}|{1:<30}|".format(length1_sort[i], t1_sort[i]))
        print('-' * 38)
    print("Слиянием sort")
    for i in range(len(t1_rand)):
        print("|{0:<5}|{1:<30}|".format(length2_sort[i], t2_sort[i]))
        print('-' * 38)

    print("Блочная rev")
    for i in range(len(t1_rand)):
        print("|{0:<5}|{1:<30}|".format(length3_rev[i], t3_rev[i]))
        print('-' * 38)
    print("Плавная rev")
    for i in range(len(t2_rand)):
        print("|{0:<5}|{1:<30}|".format(length1_rev[i], t1_rev[i]))
        print('-' * 38)
    print("Слиянием rev")
    for i in range(len(t1_rand)):
        print("|{0:<5}|{1:<30}|".format(length2_rev[i], t2_rev[i]))
        print('-' * 38)

    markers = ['.', 'x', 's']

    ax.plot(length2_rand, t2_rand, label=labels_list[0], marker=markers[0])
    ax.plot(length1_rand, t1_rand, label=labels_list[1], marker=markers[1])
    ax.plot(length3_rand, t3_rand, label=labels_list[2], marker=markers[2])
    ax.legend()
    plt.show()

    fig, ax = plt.subplots()
    ax.set_xlabel("Размер массива")
    ax.set_ylabel("Время, мс")

    ax.plot(length3_sort, t3_sort, label=labels_list[0], marker=markers[0])
    ax.plot(length1_sort, t1_sort, label=labels_list[1], marker=markers[1])
    ax.plot(length2_sort, t2_sort, label=labels_list[2], marker=markers[2])
    ax.legend()
    plt.show()

    fig, ax = plt.subplots()
    ax.set_xlabel("Размер массива")
    ax.set_ylabel("Время, мс")

    ax.plot(length2_rev, t2_rev, label=labels_list[0], marker=markers[0])
    ax.plot(length1_rev, t1_rev, label=labels_list[1], marker=markers[1])
    ax.plot(length3_rev, t3_rev, label=labels_list[2], marker=markers[2])
    ax.legend()
    plt.show()


if __name__ == '__main__':
    task = 1
    sorts = [block_sort, smooth_sort, merge_sort]
    while task != 0:
        task = menu()

        if task == 1 or task == 2 or task == 3:
            arr = input_array()
            if arr is not None:
                res = sorts[task - 1](arr)
                print(res)
        elif task == 4:
            drawGraph()
