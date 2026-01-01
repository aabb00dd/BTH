def MedianOfThree(arr, start, end):
    mid = (start + end) // 2
    if arr[start] > arr[mid]:
        arr[start], arr[mid] = arr[mid], arr[start]
    if arr[start] > arr[end]:
        arr[start], arr[end] = arr[end], arr[start]
    if arr[mid] > arr[end]:
        arr[mid], arr[end] = arr[end], arr[mid]
    return mid


def Partition(arr, start, end):
    pivot_index = MedianOfThree(arr, start, end)
    arr[pivot_index], arr[start] = arr[start], arr[pivot_index]

    pivot = arr[start]
    left = start + 1
    right = end

    done = False
    while not done:
        while left <= right and arr[left] <= pivot:
            left += 1
        while arr[right] >= pivot and right >= left:
            right -= 1
        if right < left:
            done = True
        else:
            arr[left], arr[right] = arr[right], arr[left]

    arr[start], arr[right] = arr[right], arr[start]

    return right


def QuickSort(arr, start, end):
    if start < end:
        pivot = Partition(arr, start, end)
        QuickSort(arr, start, pivot - 1)
        QuickSort(arr, pivot + 1, end)

