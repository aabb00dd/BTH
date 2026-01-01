class AVL:
    class Node:
        def __init__(self, data):
            self.left = None
            self.right = None
            self.data = data
            self.height = 1

    def __init__(self):
        self._root = None
        self.node_id = 0  # ONLY USED WITHIN to_graphviz()!
        pass

    def _get_height(self, node):
        if node is None:
            return 0
        return node.height

    def _update_height(self, node):
        if node is not None:
            node.height = 1 + max(self._get_height(node.left), self._get_height(node.right))

    def _balance_factor(self, node):
        if node is None:
            return 0
        return self._get_height(node.left) - self._get_height(node.right)

    def _rotate_right(self, y):
        x = y.left
        T2 = x.right

        x.right = y
        y.left = T2

        self._update_height(y)
        self._update_height(x)

        return x

    def _rotate_left(self, x):
        y = x.right
        T2 = y.left

        y.left = x
        x.right = T2

        self._update_height(x)
        self._update_height(y)

        return y

    def _insert(self, root, element):
        if root is None:
            return self.Node(element)

        if element < root.data:
            root.left = self._insert(root.left, element)
        elif element > root.data:
            root.right = self._insert(root.right, element)
        else:
            return root

        self._update_height(root)

        balance = self._balance_factor(root)

        if balance > 1 and element < root.left.data:
            return self._rotate_right(root)

        if balance < -1 and element > root.right.data:
            return self._rotate_left(root)

        if balance > 1 and element > root.left.data:
            root.left = self._rotate_left(root.left)
            return self._rotate_right(root)

        if balance < -1 and element < root.right.data:
            root.right = self._rotate_right(root.right)
            return self._rotate_left(root)

        return root

    def insert(self, element):
        self._root = self._insert(self._root, element)

    def _remove(self, root, element):
        if root is None:
            return root

        if element < root.data:
            root.left = self._remove(root.left, element)
        elif element > root.data:
            root.right = self._remove(root.right, element)
        else:
            if root.left is None:
                return root.right
            elif root.right is None:
                return root.left
            root.data = self._get_min_value(root.right)
            root.right = self._remove(root.right, root.data)

        self._update_height(root)

        balance = self._balance_factor(root)

        if balance > 1 and self._balance_factor(root.left) >= 0:
            return self._rotate_right(root)

        if balance > 1 and self._balance_factor(root.left) < 0:
            root.left = self._rotate_left(root.left)
            return self._rotate_right(root)

        if balance < -1 and self._balance_factor(root.right) <= 0:
            return self._rotate_left(root)

        if balance < -1 and self._balance_factor(root.right) > 0:
            root.right = self._rotate_right(root.right)
            return self._rotate_left(root)

        return root

    def remove(self, element):
        self._root = self._remove(self._root, element)

    def _find(self, root, element):
        if root is None:
            return False
        if element == root.data:
            return True
        elif element < root.data:
            return self._find(root.left, element)
        else:
            return self._find(root.right, element)

    def find(self, element):
        return self._find(self._root, element)

    def _pre_order_walk(self, root, result):
        if root:
            result.append(root.data)
            self._pre_order_walk(root.left, result)
            self._pre_order_walk(root.right, result)

    def pre_order_walk(self):
        result = []
        self._pre_order_walk(self._root, result)
        return result

    def _in_order_walk(self, root, result):
        if root:
            self._in_order_walk(root.left, result)
            result.append(root.data)
            self._in_order_walk(root.right, result)

    def in_order_walk(self):
        result = []
        self._in_order_walk(self._root, result)
        return result

    def _post_order_walk(self, root, result):
        if root:
            self._post_order_walk(root.left, result)
            self._post_order_walk(root.right, result)
            result.append(root.data)

    def post_order_walk(self):
        result = []
        self._post_order_walk(self._root, result)
        return result

    def get_tree_height(self):
        if self._root is None:
            return -1
        return self._root.height - 1

    def _get_min_value(self, root):
        current = root
        while current.left is not None:
            current = current.left
        return current.data

    def get_min(self):
        if self._root is None:
            return None
        return self._get_min_value(self._root)

    def _get_max_value(self, root):
        current = root
        while current.right is not None:
            current = current.right
        return current.data

    def get_max(self):
        if self._root is None:
            return None
        return self._get_max_value(self._root)

    def to_graphviz_rec(self, data, current):
        my_node_id = self.node_id
        data += "\t" + str(my_node_id) + \
            " [label=\"" + str(current.data) + "\"];\n"
        self.node_id += 1
        if current.left is not None:
            data += "\t" + str(my_node_id) + " -> " + \
                str(self.node_id) + " [color=blue];\n"
            data = self.to_graphviz_rec(data, current.left)
        else:
            data += "\t" + str(self.node_id) + " [label=nill,style=invis];\n"
            data += "\t" + str(my_node_id) + " -> " + \
                str(self.node_id) + " [style=invis];\n"

        self.node_id += 1
        if current.right is not None:
            data += "\t" + str(my_node_id) + " -> " + \
                str(self.node_id) + " [color=red];\n"
            data = self.to_graphviz_rec(data, current.right)
        else:
            data += "\t" + str(self.node_id) + " [label=nill,style=invis];\n"
            data += "\t" + str(my_node_id) + " -> " + \
                str(self.node_id) + " [style=invis];\n"

        return data

    def to_graphviz(self):
        data = ""
        if self._root is not None:
            self.node_id = 0
            data += "digraph {\n"
            data += "\tRoot [shape=plaintext];\n"
            data += "\t\"Root\" -> 0 [color=black];\n"
            data = self.to_graphviz_rec(data, self._root)
            data += "}\n"
        return data
