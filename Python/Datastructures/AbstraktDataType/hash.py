class CustomHashTable:
    def __init__(self, initial_capacity=10, max_load_factor=0.7):
        self.capacity = initial_capacity
        self.max_load_factor = max_load_factor
        self.current_size = 0
        self.buckets = [None] * initial_capacity

    def compute_hash(self, key):
        return key % self.capacity

    def insert(self, key):
        if (self.current_size / self.capacity) > self.max_load_factor:
            self._resize_and_rehash()

        bucket_index = self.compute_hash(key)

        if self.buckets[bucket_index] is None:
            self.buckets[bucket_index] = [key]
        else:
            self.buckets[bucket_index].append(key)

        self.current_size += 1

    def search(self, key):
        bucket_index = self.compute_hash(key)

        if self.buckets[bucket_index] is None or key not in self.buckets[bucket_index]:
            return False
        else:
            return True

    def remove(self, key):
        bucket_index = self.compute_hash(key)

        if self.buckets[bucket_index] is not None and key in self.buckets[bucket_index]:
            self.buckets[bucket_index].remove(key)
            self.current_size -= 1

    def _resize_and_rehash(self):
        new_capacity = self.capacity * 2
        new_buckets = [None] * new_capacity

        for bucket in self.buckets:
            if bucket is not None:
                for key in bucket:
                    new_index = key % new_capacity
                    if new_buckets[new_index] is None:
                        new_buckets[new_index] = [key]
                    else:
                        new_buckets[new_index].append(key)

        self.capacity = new_capacity
        self.buckets = new_buckets
