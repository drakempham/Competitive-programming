# Definition for a Node.
# class Node:
#     def __init__(self, val, prev, next, child):
#         self.val = val
#         self.prev = prev
#         self.next = next
#         self.child = child

class Solution:
    def flatten(self, head: 'Optional[Node]') -> 'Optional[Node]':
        def dfs(node):
            cur = node
            tail = node

            while cur:
                nxt = cur.next

                if cur.child:
                    child_head = cur.child
                    child_tail = dfs(child_head)

                    cur.next = child_head
                    child_head.prev = cur
                    cur.child = None

                    child_tail.next = nxt
                    if nxt:
                        nxt.prev = child_tail

                    tail = child_tail
                    cur = nxt
                else:
                    tail = cur
                    cur = cur.next

            return tail

        if not head:
            return None

        dfs(head)
        return head
