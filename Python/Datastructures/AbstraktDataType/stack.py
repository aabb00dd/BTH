class CustomStack:
    def __init__(self):
        self.container = []

    def push(self, item):
        self.container.append(item)

    def pop(self):
        if not self.is_empty():
            return self.container.pop()
        else:
            print("CustomStack is empty")

    def top(self):
        if not self.is_empty():
            return self.container[-1]
        else:
            print("CustomStack is empty")

    def is_empty(self):
        return len(self.container) == 0
