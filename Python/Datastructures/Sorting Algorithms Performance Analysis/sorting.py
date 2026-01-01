def partition(lst: list, start, end):
    x = lst[end]
    i = start - 1
    for j in range(start, end):
        if lst[j] <= x:
            i = i + 1
            lst[i], lst[j] = lst[j], lst[i]
    lst[i + 1], lst[end] = lst[end], lst[i + 1]
    return i + 1


def quicksort(lst: list) -> None:
    def _quicksort(start, end):
        if start < end:
            pivot = partition(lst, start, end)
            _quicksort(start, pivot - 1)
            _quicksort(pivot + 1, end)

    _quicksort(0, len(lst) - 1)


def _insertionsort(lst: list, start, end):
    for i in range(start + 1, end + 1):
        key = lst[i]
        j = i - 1
        while j >= start and key < lst[j]:
            lst[j + 1] = lst[j]
            j -= 1
        lst[j + 1] = key


def insertionsort(lst: list) -> None:
    _insertionsort(lst, 0, len(lst) - 1)


def merge(lst: list, start, end):
    i = j = k = 0

    while i < len(start) and j < len(end):
        if start[i] < end[j]:
            lst[k] = start[i]
            i += 1
        else:
            lst[k] = end[j]
            j += 1
        k += 1

    while i < len(start):
        lst[k] = start[i]
        i += 1
        k += 1

    while j < len(end):
        lst[k] = end[j]
        j += 1
        k += 1


def mergesort(lst: list) -> None:
    if len(lst) > 1:
        mid = len(lst) // 2
        start = lst[:mid]
        end = lst[mid:]
        mergesort(start)
        mergesort(end)
        merge(lst, start, end)


def merge_hybrid(lst: list, start, mid, end):
    i = j = k = 0
    left_length = mid - start + 1
    right_length = end - mid

    left = lst[start:start + left_length]
    right = lst[mid + 1:mid + 1 + right_length]

    while i < left_length and j < right_length:
        if left[i] < right[j]:
            lst[k + start] = left[i]
            i += 1
        else:
            lst[k + start] = right[j]
            j += 1
        k += 1

    while i < left_length:
        lst[k + start] = left[i]
        i += 1
        k += 1

    while j < right_length:
        lst[k + start] = right[j]
        j += 1
        k += 1


def _mergesort_hybrid(lst: list, start, end, breakingpoint):
    if end - start > breakingpoint:
        mid = (start + end) // 2
        _mergesort_hybrid(lst, start, mid, breakingpoint)
        _mergesort_hybrid(lst, mid + 1, end, breakingpoint)
        merge_hybrid(lst, start, mid, end)
    else:
        _insertionsort(lst, start, end)


def mergesort_hybrid(lst: list) -> None:
    _mergesort_hybrid(lst, 0, len(lst) - 1, 49)


def _quicksort_hybrid(lst: list, start, end, breakingpoint):
    if end - start > breakingpoint:
        pivot = partition(lst, start, end)
        _quicksort_hybrid(lst, start, pivot - 1, breakingpoint)
        _quicksort_hybrid(lst, pivot + 1, end, breakingpoint)
    else:
        _insertionsort(lst, start, end)


def quicksort_hybrid(lst: list) -> None:
    _quicksort_hybrid(lst, 0, len(lst) - 1, 22)
