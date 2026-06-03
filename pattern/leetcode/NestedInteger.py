class NestedInteger:
    def __init__(self, value=None):
        if value is None:
            self._integer = None
            self._list = []
        else:
            self._integer = value
            self._list = None

    # nestedInteger still create so maybe not fully available yet -> forward reference
    def add(self, elem: 'NestedInteger'):
        if self._list is None:
            self._list = []
            self._integer = None
        self._list.append(elem)

    def isInteger(self):
        """
        Return True if this object stores a single integer.
        """
        return self._integer is not None

    def setInteger(self, value):
        """
        Set this object to store one integer.
        """
        self._integer = value
        self._list = None

    def getInteger(self):
        """
        Return integer if it stores integer.
        Else return None.
        """
        return self._integer

    def getList(self):
        """
        Return list if it stores nested list.
        Else return None.
        """
        return self._list

    def __repr__(self):
        if self.isInteger():
            return str(self._integer)
        return "[" + ",".join(map(str, self.getList())) + "]"


root = NestedInteger()
child1 = NestedInteger(1)
print(child1)

child2 = NestedInteger()
child2.add(NestedInteger(2))
child2.add(NestedInteger(3))
print(child2)

root.add(child1)
root.add(child2)
print(root)
