from algorithms import full_matrix, optimised, recursive, recursive_with_cache

params_dl = [
    ["", "", 0],
    ["кот", "скат", 2],
    ["абвгд", "абвгд", 0],
    ["а", "б", 1],
    ["", "ааа", 3],
    ["аааа", "", 4],
    ["аб", "ба", 1],
    ["абвгд", "багвд", 2],
    ]

params_l = [
    ["", "", 0],
    ["кот", "скат", 2],
    ["абвгд", "абвгд", 0],
    ["а", "б", 1],
    ["", "ааа", 3],
    ["аааа", "", 4],
    ["аб", "ба", 2],
    ["абвгд", "багвд", 3],
    ]


def test_dl_full_matrix():
    for t in params_dl:
        res = full_matrix(t[0], t[1], True, False)
        assert res == t[2]


def test_dl_optimised():
    for t in params_dl:
        res = optimised(t[0], t[1], True)
        assert res == t[2]


def test_l_full_matrix():
    for t in params_l:
        res = full_matrix(t[0], t[1], False, False)
        assert res == t[2]


def test_l_optimised():
    for t in params_l:
        res = optimised(t[0], t[1], False)
        assert res == t[2]


def test_dl_recursive():
    for t in params_dl:
        res = recursive(t[0], t[1])
        assert res == t[2]


def test_dl_recursive_with_cache():
    for t in params_dl:
        res = recursive_with_cache(t[0], t[1], False)
        assert res == t[2]
