class NodeInQueue:
    def __init__(self, data):
        self.data = data
        self.next_in_line = None


class CustomQueue:
    def __init__(self):
        self.head = None
        self.tail = None

    def add(self, data):
        new_node = NodeInQueue(data)
        if self.is_empty():
            self.head = new_node
            self.tail = new_node
        else:
            self.tail.next_in_line = new_node
            self.tail = new_node

    def remove(self):
        if not self.is_empty():
            removed_data = self.head.data
            self.head = self.head.next_in_line
            if self.head is None:
                self.tail = None
            return removed_data
        else:
            print("CustomQueue is empty")

    def front(self):
        if not self.is_empty():
            return self.head.data
        else:
            print("CustomQueue is empty")

    def is_empty(self):
        return self.head is None
