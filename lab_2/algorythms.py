def classic(a, b):
    m, q, n = len(a), len(a[0]), len(b[0])
    res = [[0] * n for _ in range(m)]

    for i in range(m):
        for j in range(n):
            for k in range(q):
                res[i][j] = res[i][j] + a[i][k] * b[k][j]
    return res


def vinograd(a, b):
    res = []
    if len(a[0]) != len(b):
        print("Умножение матриц с такими размерами невозможно")
        return res

    m, q, n = len(a), len(a[0]), len(b[0])
    for _ in range(m):
        res.append([0] * n)
    tmp_row, tmp_col = [0] * m, [0] * n

    for i in range(m):
        for j in range(0, q // 2):
            tmp_row[i] = tmp_row[i] + a[i][2 * j] * a[i][2 * j + 1]

    for i in range(n):
        for j in range(0, q // 2):
            tmp_col[i] = tmp_col[i] + b[2 * j][i] * b[2 * j + 1][i]

    for i in range(m):
        for j in range(n):
            res[i][j] = - tmp_row[i] - tmp_col[j]
            for k in range(0, q // 2):
                res[i][j] = res[i][j] + (a[i][2 * k] + b[2 * k + 1][j]) * (a[i][2 * k + 1] + b[2 * k][j])

    if q % 2 == 1:
        for i in range(m):
            for j in range(n):
                res[i][j] = res[i][j] + a[i][q - 1] * b[q - 1][j]

    return res


def vinograd_optimised(a, b):
    res = []
    if len(a[0]) != len(b):
        print("Умножение матриц с такими размерами невозможно")
        return res

    m, q, n = len(a), len(a[0]), len(b[0])
    for _ in range(m):
        res.append([0] * n)
    tmp_row, tmp_col = [0] * m, [0] * n
    q_half = q // 2

    for i in range(m):
        for j in range(0, q_half):
            tmp_row[i] += a[i][j << 1] * a[i][(j << 1) + 1]

    for i in range(n):
        for j in range(0, q_half):
            tmp_col[i] += b[j << 1][i] * b[(j << 1) + 1][i]

    for i in range(m):
        for j in range(n):
            res[i][j] = - tmp_row[i] - tmp_col[j]
            buf = 0
            for k in range(0, q_half):
                buf += (a[i][k << 1] + b[(k << 1) + 1][j]) * (a[i][(k << 1) + 1] + b[k << 1][j])
            res[i][j] += buf

    if q % 2 == 1:
        for i in range(m):
            for j in range(n):
                res[i][j] += a[i][q - 1] * b[q - 1][j]

    return res
