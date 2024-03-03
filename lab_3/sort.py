import heapq


def block_sort(a):
    n = len(a)
    size = max(a) / n
    blocks_list = []
    for i in range(n):
        blocks_list.append([])

    for i in range(n):
        j = int(a[i] / size)
        if j != n:
            blocks_list[j].append(a[i])
        else:
            blocks_list[-1].append(a[i])

    for i in range(n):
        blocks_list[i] = sorted(blocks_list[i])

    res = []
    for i in range(n):
        res += blocks_list[i]
    return res


# Ввод
#     i - индекс, потомки которого ищутся
#     indexes - список индексов
# Вывод
#     -
def count_indexes(i, indexes):
    indexes.append(2 * indexes[i] + 1)
    indexes.append(2 * indexes[i] + 2)


# Ввод
#     index_list - массив индексов
#     heap - исходная куча
# Вывод
#     список, созданный из списка индексов и исходной кучи
def get_list(index_list, heap):
    return [heap[i] for i in index_list if i < len(heap)]


# Ввод
#     heap - куча для разделения
# Вывод
#     левая и правая куча
def heap_division(heap):
    index = 0
    indexes_left = [1]
    indexes_right = [2]
    while indexes_left[-1] < len(heap):
        count_indexes(index, indexes_left)
        count_indexes(index, indexes_right)
        index += 1
    heap_left = get_list(indexes_left, heap)
    heap_right = get_list(indexes_right, heap)
    return heap_left, heap_right


# Ввод
#     arr - массив для сортировки
# Вывод
#     -
def smooth_sort(a):
    leonardo_numbers = [1, 1]
    next_number = leonardo_numbers[-1] + leonardo_numbers[-2] + 1
    while len(a) > next_number:
        leonardo_numbers.append(next_number)
        next_number = leonardo_numbers[-1] + leonardo_numbers[-2] + 1
    leonardo_numbers.reverse()

    list_heaps = []
    j = 0
    for i in leonardo_numbers:
        if len(a) - j >= i:
            list_heaps.append(a[j:j + i])
            j += i

    for heap in list_heaps:
        heapq.heapify(heap)
    list_heaps.reverse()

    a.clear()
    while len(list_heaps):
        flag = False

        min_index = list_heaps.index(min(list_heaps))
        current_root = list_heaps[0][0]
        current_min = list_heaps[min_index][0]

        heapq.heapreplace(list_heaps[0], current_min)
        heapq.heapreplace(list_heaps[min_index], current_root)

        if len(list_heaps[0]) > 1:
            heap_left, heap_right = heap_division(list_heaps[0])
            flag = True

        minimum = heapq.heappop(list_heaps[0])
        a.append(minimum)

        list_heaps.pop(0)
        if flag:
            list_heaps.insert(0, heap_left)
            list_heaps.insert(0, heap_right)
    return a


def merge_sort(a):
    if len(a) > 1:
        mid = len(a) // 2
        left = a[:mid]
        right = a[mid:]
        merge_sort(left)
        merge_sort(right)
        i = j = k = 0
        while i < len(left) and j < len(right):
            if left[i] < right[j]:
                a[k] = left[i]
                i += 1
            else:
                a[k] = right[j]
                j += 1
            k += 1

        while i < len(left):
            a[k] = left[i]
            i += 1
            k += 1
        while j < len(right):
            a[k] = right[j]
            j += 1
            k += 1

    return a
