def Merge(arr, left, middle1, middle2, right):
    temp = [0] * len(arr)
    i = left
    j = middle1 + 1
    k = middle2 + 1
    p = left

    while i <= middle1 and j <= middle2 and k <= right:
        if arr[i] <= arr[j] and arr[i] <= arr[k]:
            temp[p] = arr[i]
            i += 1
        elif arr[j] <= arr[i] and arr[j] <= arr[k]:
            temp[p] = arr[j]
            j += 1
        else:
            temp[p] = arr[k]
            k += 1
        p += 1

    while i <= middle1 and j <= middle2:
        if arr[i] <= arr[j]:
            temp[p] = arr[i]
            i += 1
        else:
            temp[p] = arr[j]
            j += 1
        p += 1

    while i <= middle1 and k <= right:
        if arr[i] <= arr[k]:
            temp[p] = arr[i]
            i += 1
        else:
            temp[p] = arr[k]
            k += 1
        p += 1

    while j <= middle2 and k <= right:
        if arr[j] <= arr[k]:
            temp[p] = arr[j]
            j += 1
        else:
            temp[p] = arr[k]
            k += 1
        p += 1

    while i <= middle1:
        temp[p] = arr[i]
        i += 1
        p += 1

    while j <= middle2:
        temp[p] = arr[j]
        j += 1
        p += 1

    while k <= right:
        temp[p] = arr[k]
        k += 1
        p += 1

    for i in range(left, right + 1):
        arr[i] = temp[i]


def MergeSort3Way(arr, left, right):
    if left < right:
        if right - left >= 1:
            third = (right - left) // 3
            middle1 = left + third
            middle2 = left + 2 * third

            MergeSort3Way(arr, left, middle1)
            MergeSort3Way(arr, middle1 + 1, middle2)
            MergeSort3Way(arr, middle2 + 1, right)

            Merge(arr, left, middle1, middle2, right)


def MergeSort3Way2(arr):
    MergeSort3Way(arr, 0, len(arr) - 1)


