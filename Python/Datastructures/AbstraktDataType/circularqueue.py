class CircularQueue:
    def __init__(self, max_size):
        self.max_size = max_size
        self.buffer = [None] * max_size
        self.head = self.tail = -1

    def add(self, element):
        if (self.tail + 1) % self.max_size == self.head:
            print("CircularQueue is full")
        elif self.is_empty():
            self.head = self.tail = 0
            self.buffer[self.tail] = element
        else:
            self.tail = (self.tail + 1) % self.max_size
            self.buffer[self.tail] = element

    def remove(self):
        if self.is_empty():
            print("CircularQueue is empty")
        elif self.head == self.tail:
            removed_element = self.buffer[self.head]
            self.head = self.tail = -1
            return removed_element
        else:
            removed_element = self.buffer[self.head]
            self.head = (self.head + 1) % self.max_size
            return removed_element

    def front(self):
        if not self.is_empty():
            return self.buffer[self.head]
        else:
            print("CircularQueue is empty")

    def is_empty(self):
        return self.head == -1
