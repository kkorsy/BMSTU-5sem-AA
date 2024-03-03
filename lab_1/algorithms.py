def distance(sub_1, sub_2):
    if len(sub_1) == 0 and len(sub_2) == 0:
        return 0
    if len(sub_1) == 0 and len(sub_2) > 0:
        return len(sub_2)
    if len(sub_1) > 0 and len(sub_2) == 0:
        return len(sub_1)

    m = min(
        distance(sub_1, sub_2[:-1]) + 1,
        distance(sub_1[:-1], sub_2) + 1,
        distance(sub_1[:-1], sub_2[:-1]) + (0 if sub_1[-1] == sub_2[-1] else 1)
    )

    # transpose is possible
    if len(sub_1) > 1 and len(sub_2) > 1 and sub_1[-2] == sub_2[-1] and sub_1[-1] == sub_2[-2]:
        return min(m, distance(sub_1[:-2], sub_2[:-2]) + 1)

    return m


def recursive(s1, s2):
    return distance(s1, s2)


def get_matrix(l1, l2):
    matrix = [[float('inf')] * (l2 + 1) for _ in range(l1 + 1)]

    k = 0
    for j in range(l2 + 1):
        matrix[0][j] = k
        k += 1
    k = 0
    for i in range(l1 + 1):
        matrix[i][0] = k
        k += 1

    return matrix


def print_matrix(s1, s2, a):
    print("\nПолученная матрица:")
    print("0  0  " + "  ".join([c for c in s2]))

    for i in range(len(s1) + 1):
        print(s1[i - 1] if (i != 0) else "0", end="")
        for j in range(len(s2) + 1):
            print("  " + str(a[i][j]), end="")
        print()


def full_matrix(s1, s2, flag, output=True):
    l1, l2 = len(s1), len(s2)
    matrix = get_matrix(l1, l2)

    for i in range(1, l1 + 1):
        for j in range(1, l2 + 1):
            matrix[i][j] = min(
                matrix[i - 1][j] + 1,
                matrix[i][j - 1] + 1,
                matrix[i - 1][j - 1] + (0 if s1[i - 1] == s2[j - 1] else 1)
            )
            if flag and i > 1 and j > 1 and s1[i - 1] == s2[j - 2] and s1[i - 2] == s2[j - 1]:
                matrix[i][j] = min(matrix[i][j], matrix[i - 2][j - 2] + 1)

    if output:
        print_matrix(s1, s2, matrix)

    return matrix[-1][-1]


def optimised(s1, s2, flag):
    l1, l2 = len(s1), len(s2)
    # line_2 - current
    line_0, line_1, line_2 = [float('inf')] * (l2 + 1), [i for i in range(l2 + 1)], [float('inf')] * (l2 + 1)

    for i in range(1, l1 + 1):
        line_2[0] = i
        for j in range(1, l2 + 1):
            line_2[j] = min(
                line_1[j] + 1,
                line_2[j - 1] + 1,
                line_1[j - 1] + (0 if s1[i - 1] == s2[j - 1] else 1)
            )
            if flag and i > 1 and j > 1 and s1[i - 1] == s2[j - 2] and s1[i - 2] == s2[j - 1]:
                line_2[j] = min(line_2[j], line_0[j - 2] + 1)

        line_0, line_1, line_2 = line_1, line_2, [float('inf')] * (l2 + 1)

    return line_1[-1]


def non_recursive(s1, s2, t, flag=True):
    return full_matrix(s1, s2, flag) if t == 1 else optimised(s1, s2, flag)


def recursive_with_cache(s1, s2, output=True):
    def get_value(cache, sub1, sub2):
        i, j = len(sub1), len(sub2)
        if i == 0 and j == 0:
            cache[i][j] = 0
            return 0
        if i == 0 and j > 0:
            cache[i][j] = j
            return j
        if i > 0 and j == 0:
            cache[i][j] = i
            return i
        if cache[i][j] < float('inf'):
            return cache[i][j]

        cache[i][j] = min(
            get_value(cache, sub1[:i], sub2[:j - 1]) + 1,
            get_value(cache, sub1[:i - 1], sub2[:j]) + 1,
            get_value(cache, sub1[:i - 1], sub2[:j - 1]) + (0 if sub1[-1] == sub2[-1] else 1)
        )
        if i > 1 and j > 1 and s1[i - 1] == s2[j - 2] and s1[i - 2] == s2[j - 1]:
            cache[i][j] = min(cache[i][j], get_value(cache, sub1[:i - 2], sub2[:j - 2]) + 1)
        return cache[i][j]

    l1, l2 = len(s1), len(s2)
    cache = get_matrix(l1, l2)

    result = get_value(cache, s1, s2)
    if output:
        print_matrix(s1, s2, cache)

    return result
