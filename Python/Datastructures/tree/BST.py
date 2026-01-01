class BST:
    class Node:
        def __init__(self, data):
            self.left = None
            self.right = None
            self.data = data

    def __init__(self):
        self._root = None
        self.node_id = 0  # ONLY USED WITHIN to_graphviz()!
        pass

    def insert(self, element):
        self._root = self._insert(self._root, element)

    def _insert(self, root, element):
        if root is None:
            return self.Node(element)
        if element < root.data:
            root.left = self._insert(root.left, element)
        elif element > root.data:
            root.right = self._insert(root.right, element)
        return root

    def remove(self, element):
        self._root = self._remove(self._root, element)

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
        return root

    def _get_min_value(self, root):
        current = root
        while current.left is not None:
            current = current.left
        return current.data

    def find(self, element):
        return self._find(self._root, element)

    def _find(self, root, element):
        if root is None:
            return False
        if element == root.data:
            return True
        elif element < root.data:
            return self._find(root.left, element)
        else:
            return self._find(root.right, element)

    def pre_order_walk(self):
        result = []
        self._pre_order_walk(self._root, result)
        return result

    def _pre_order_walk(self, root, result):
        if root:
            result.append(root.data)
            self._pre_order_walk(root.left, result)
            self._pre_order_walk(root.right, result)

    def in_order_walk(self):
        result = []
        self._in_order_walk(self._root, result)
        return result

    def _in_order_walk(self, root, result):
        if root:
            self._in_order_walk(root.left, result)
            result.append(root.data)
            self._in_order_walk(root.right, result)

    def post_order_walk(self):
        result = []
        self._post_order_walk(self._root, result)
        return result

    def _post_order_walk(self, root, result):
        if root:
            self._post_order_walk(root.left, result)
            self._post_order_walk(root.right, result)
            result.append(root.data)

    def get_tree_height(self):
        return self._get_tree_height(self._root)

    def _get_tree_height(self, root):
        if root is None:
            return -1
        left_height = self._get_tree_height(root.left)
        right_height = self._get_tree_height(root.right)
        return 1 + max(left_height, right_height)

    def get_min(self):
        if self._root is None:
            return None
        return self._get_min(self._root)

    def _get_min(self, root):
        while root.left is not None:
            root = root.left
        return root.data

    def get_max(self):
        if self._root is None:
            return None
        return self._get_max(self._root)

    def _get_max(self, root):
        while root.right is not None:
            root = root.right
        return root.data

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


def main():
    bst = BST()
    bst.insert(2)
    bst.insert(3)
    bst.insert(1)
    print(bst.to_graphviz())


if __name__ == '__main__':
    main()
