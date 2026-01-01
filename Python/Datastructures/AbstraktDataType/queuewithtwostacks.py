class DualStackQueue:
    def __init__(self):
        self.input_stack = []
        self.output_stack = []

    def add(self, element):
        self.input_stack.append(element)

    def remove(self):
        if not self.output_stack:
            if not self.input_stack:
                print("DualStackQueue is empty")
                return None
            while self.input_stack:
                self.output_stack.append(self.input_stack.pop())
        return self.output_stack.pop()

    def front(self):
        if not self.output_stack:
            if not self.input_stack:
                print("DualStackQueue is empty")
                return None
            while self.input_stack:
                self.output_stack.append(self.input_stack.pop())
        return self.output_stack[-1] if self.output_stack else None

    def is_empty(self):
        return not self.input_stack and not self.output_stack
