# """
# This is the interface that allows for creating nested lists.
# You should not implement it, or speculate about its implementation
# """
# class NestedInteger:
#    def __init__(self, value=None):
#        """
#        If value is not specified, initializes an empty list.
#        Otherwise initializes a single integer equal to value.
#        """
#
#    def isInteger(self):
#        """
#        @return True if this NestedInteger holds a single integer, rather than a nested list.
#        :rtype bool
#        """
#
#    def add(self, elem):
#        """
#        Set this NestedInteger to hold a nested list and adds a nested integer elem to it.
#        :rtype void
#        """
#
#    def setInteger(self, value):
#        """
#        Set this NestedInteger to hold a single integer equal to value.
#        :rtype void
#        """
#
#    def getInteger(self):
#        """
#        @return the single integer that this NestedInteger holds, if it holds a single integer
#        Return None if this NestedInteger holds a nested list
#        :rtype int
#        """
#
#    def getList(self):
#        """
#        @return the nested list that this NestedInteger holds, if it holds a nested list
#        Return None if this NestedInteger holds a single integer
#        :rtype List[NestedInteger]
#        """

class Solution:
    def deserialize(self, s: str) -> NestedInteger:
        if s[0] != '[':
            return NestedInteger(int(s))

        i = 0
        curr = None
        stack = []
        num = 0

        while i < len(s):
            if s[i] == '[':
                if curr is None:
                    curr = NestedInteger()
                else:
                    stack.append(curr)
                    curr = NestedInteger()
            elif s[i] == ']':
                if stack:
                    child = curr
                    parent = stack.pop()
                    parent.add(child)
                    curr = parent
            elif s[i] == ',':
                pass
            else:
                j = i
                if s[j] == '-':
                    j += 1

                while j < len(s) and s[j].isnumeric():
                    j += 1
                num = int(s[i:j])
                curr.add(NestedInteger(num))

                i = j-1

            i += 1
        return curr
