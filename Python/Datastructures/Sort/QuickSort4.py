import random


def partition(arr, low, high):
    pivot_index = random.randint(low, high)
    arr[high], arr[pivot_index] = arr[pivot_index], arr[high]
    pivot = arr[high]
    i = low

    for j in range(low, high):
        if arr[j] < pivot:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1

    arr[i], arr[high] = arr[high], arr[i]
    return i


def quickselect(arr, low, high, i):
    if low == high:
        return arr[low]

    pivot_index = partition(arr, low, high)

    if i == pivot_index:
        return arr[i]
    elif i < pivot_index:
        return quickselect(arr, low, pivot_index - 1, i)
    else:
        return quickselect(arr, pivot_index + 1, high, i)


def find_ith_smallest(arr, i):
    if i < 0 or i >= len(arr):
        return None
    return quickselect(arr, 0, len(arr) - 1, i)

