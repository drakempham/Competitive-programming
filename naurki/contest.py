from os import *
from sys import *
from collections import *
from math import *

class TreeNode:
    def __init__(self, data) :
        self.data = data
        self.left = None
        self.right = None

    def __del__(self):
        if self.left:
            del self.left
        if self.right:
            del self.right

def mergeBST(root1, root2):
	# Write your code here.
    def inorder(arr, node):
        if not node:
            return

        inorder(arr, node.left)
        arr.append(node.data)
        inorder(arr, node.right)
    
    arr1, arr2 = [], []
    inorder(arr1, root1)
    inorder(arr2, root2)

    i, j = 0,0
    ans = []
    while i <len(arr1) and j < len(arr2):
        if arr1[i] <= arr2[j]:
            ans.append(arr1[i])
            i+=1
        else:
            ans.append(arr2[j])
            j+=1
    
    while i < len(arr1):
        ans.append(arr1[i])
        i+=1
    
    while j < len(arr2):
        ans.append(arr2[j])
        j+=1
    
    return ans




