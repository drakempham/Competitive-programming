def isPalindrome(head):
    if not head or not head.next:
        return True

    sl = head
    ft = head
    while ft and ft.next:
        sl = sl.next
        ft = ft.next.next
    prev = None
    curr = sl
    while curr:
        nxt = curr.next
        curr.next = prev
        prev = curr
        curr = nxt

    first = head
    second = prev

    while second:
        if first.val != second.val:
            return False
        first = first.next
        second = second.next

    return True
