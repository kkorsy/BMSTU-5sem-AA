import matplotlib.pyplot as plt
import string
import random
from time import process_time_ns
from algorithms import full_matrix, optimised, recursive, recursive_with_cache


def graph():
    n = 50000
    fig, ax = plt.subplots()
    ax.set_xlabel("Длина строки")
    ax.set_ylabel("Время, мкс")

    x_list = []
    y_list = [[], [], [], [], []]

    labels_list = [
        "Матричный алгоритм нахождения расстояния Дамерау-Левенштейна",
        "Матричный алгоритм нахождения расстояния Левенштейна",
        "Оптимизированный матричный алгоритм нахождения расстояния Дамерау-Левенштейна",
        "Оптимизированный матричный алгоритм нахождения расстояния Левенштейна",
        "Рекурсивный алгоритм нахождения расстояния Дамерау-Левенштейна с использованием кэша в виде матрицы"
    ]

    for length in range(10, 411, 50):
        measure_list = measure(n, length)

        for i in range(len(measure_list)):
            y_list[i].append(measure_list[i])
        x_list.append(length)

        print(f"\rDone: {length}", end='')
        n = 100 if length > 100 else 1000

    for i in range(len(labels_list)):
        print("|{0:^10}|{1:^30}|".format("length", labels_list[i]))
        for j in range(len(y_list[i])):
            print("|{0:^10}|{1:^30}|".format(x_list[j], y_list[i][j]))
        print()

    markers = ['.', 'x', '+', 'v', 's']

    for i in range(len(y_list)):
        ax.plot(x_list, y_list[i], label=labels_list[i], marker=markers[i])
    ax.legend()
    plt.show()


def measure(n, length):
    t_sum_fm_dl, t_sum_fm_l, t_sum_opt_dl, t_sum_opt_l, t_sum_rec_ch_dl = 0, 0, 0, 0, 0
    for i in range(n):
        s1 = get_random_string(length)
        s2 = get_random_string(length)

        t_start = process_time_ns()
        full_matrix(s1, s2, True, False)
        t_end = process_time_ns()
        t_sum_fm_dl += (t_end - t_start)

        t_start = process_time_ns()
        full_matrix(s1, s2, False, False)
        t_end = process_time_ns()
        t_sum_fm_l += (t_end - t_start)

        t_start = process_time_ns()
        optimised(s1, s2, True)
        t_end = process_time_ns()
        t_sum_opt_dl += (t_end - t_start)

        t_start = process_time_ns()
        optimised(s1, s2, False)
        t_end = process_time_ns()
        t_sum_opt_l += (t_end - t_start)

        # t_start = process_time_ns()
        # recursive(s1, s2)
        # t_end = process_time_ns()
        # t_sum_rec_dl += (t_end - t_start)

        t_start = process_time_ns()
        recursive_with_cache(s1, s2, False)
        t_end = process_time_ns()
        t_sum_rec_ch_dl += (t_end - t_start)

    return [t_sum_fm_dl / n / (10 ** 6), t_sum_fm_l / n / (10 ** 6), t_sum_opt_dl / n / (10 ** 6),
            t_sum_opt_l / n / (10 ** 6), t_sum_rec_ch_dl / n / (10 ** 6)]


def get_random_string(length):
    letters = string.ascii_lowercase
    rand_string = ''.join(random.choice(letters) for i in range(length))

    return rand_string
