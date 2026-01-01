def counting_sort(A, B, k):
    C = [0] * (k + 1)

    for j in range(len(A)):
        C[A[j]] += 1

    for i in range(1, k + 1):
        C[i] += C[i - 1]

    for j in range(len(A) - 1, -1, -1):
        B[C[A[j]] - 1] = A[j]
        C[A[j]] -= 1
