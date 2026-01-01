class StackNode:
    def __init__(self, data):
        self.data = data
        self.next_node = None


class CustomStack:
    def __init__(self):
        self.top_node = None

    def push(self, data):
        new_node = StackNode(data)
        new_node.next_node = self.top_node
        self.top_node = new_node

    def pop(self):
        if self.is_empty():
            raise IndexError("Cannot pop from an empty stack")
        popped_data = self.top_node.data
        temp_node = self.top_node.next_node
        del self.top_node
        self.top_node = temp_node
        return popped_data

    def is_empty(self):
        return self.top_node is None
