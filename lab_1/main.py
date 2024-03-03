from algorithms import non_recursive, recursive, recursive_with_cache
from graph import graph


def menu():
    print(
        "Расстояние Дамерау-Левенштейна:\n\t1) Нерекурсивно\n\t2) Рекурсивно\n\t3) Рекурсивно с кэшем\n"
        "Расстояние Левенштейна:\n\t4) Нерекурсивно\n\n\t5) Построить график\n\t0) Выйти\n"
    )

    num = int(input("Введите номер: "))
    if 5 < num or num < 0:
        print("Введена неверная команда")
        num = -1

    return num


def get_strings():
    s1 = input("Введите первую строку: ")
    s2 = input("Введите вторую строку: ")

    return s1, s2


def choose_type():
    print("\t1) Используя всю матрицу\n\t2) Используя 3 последних строки")
    return int(input("Введите номер: "))


if __name__ == '__main__':
    cmd = menu()

    while cmd:
        s1, s2 = "", ""

        if cmd == 1:
            t = choose_type()
            s1, s2 = get_strings()
            print("Полученное расстояние:", str(non_recursive(s1, s2, t)))
        elif cmd == 2:
            s1, s2 = get_strings()
            print("Полученное расстояние:", str(recursive(s1, s2)))
        elif cmd == 3:
            s1, s2 = get_strings()
            print("Полученное расстояние:", str(recursive_with_cache(s1, s2)))
        elif cmd == 4:
            t = choose_type()
            s1, s2 = get_strings()
            print("Полученное расстояние:", str(non_recursive(s1, s2, t, False)))
        elif cmd == 5:
            graph()

        cmd = menu()
