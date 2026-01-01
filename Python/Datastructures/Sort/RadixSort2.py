def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and key < arr[j]:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key


def bucket_sort(arr, exp):
    buckets = [[] for _ in range(10)]

    for num in arr:
        index = num // exp % 10
        buckets[index].append(num)

    for i in range(10):
        insertion_sort(buckets[i])

    index = 0
    for i in range(10):
        for num in buckets[i]:
            arr[index] = num
            index += 1


def radix_sort(arr):
    max_num = max(arr)
    exp = 1
    while max_num // exp > 0:
        bucket_sort(arr, exp)
        exp *= 10

