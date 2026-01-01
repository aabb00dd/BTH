def Partition3Way(arr, low, high):
    pivot = arr[low]
    left = low
    right = high
    i = low

    while i <= right:
        if arr[i] < pivot:
            arr[i], arr[left] = arr[left], arr[i]
            left += 1
            i += 1
        elif arr[i] > pivot:
            arr[i], arr[right] = arr[right], arr[i]
            right -= 1
        else:
            i += 1

    return left, right


def QuickSort3Way(arr, low, high):
    if low < high:
        left, right = Partition3Way(arr, low, high)
        QuickSort3Way(arr, low, left - 1)
        QuickSort3Way(arr, right + 1, high)


def QuickSort3Way2(arr):
    QuickSort3Way(arr, 0, len(arr) - 1)
