#! /usr/bin/env python

class Node:
	def __init__(self, v):
		self.v = v
		self.l, self.r = None, None
		self.h, self.size = 1, 1

	def fix(self):
		self.h = max(self.r.h if self.r else 0, self.l.h if self.l else 0) + 1
		self.size = (self.r.size if self.r else 0) + (self.l.size if self.l else 0) + 1

	def balance(self):
		return (self.r.h if self.r else 0) - (self.l.h if self.l else 0)

class AVLTree:
	def rotate(node, is_right):
		if is_right:
			t = node.l
			node.l = t.r
			t.r = node
		else:
			t = node.r
			node.r = t.l
			t.l = node
		
		node.fix()
		t.fix()

		return t

	def balance(node):
		node.fix()
		if node.balance() == -2:
			if node.l.balance() == 1:
				node.l = AVLTree.rotate(node.l, False)
			return AVLTree.rotate(node, True)
		if node.balance() == 2:
			if node.r.balance() == -1:
				node.r = AVLTree.rotate(node.r, True)
			return AVLTree.rotate(node, False)
		return node

	def get_min(node):
		return AVLTree.get_min(node.l) if node.l else node

	def get_max(node):
		return AVLTree.get_max(node.r) if node.r else node

	def __init__(self):
		self.size = 0
		self.head = None


	def add(self, v):
		self.size += 1

		def Add(node):
			if not node:
				return Node(v)
			t = node.r if v >= node.v else node.l
			if v >= node.v:
				node.r = Add(node.r)
			else:
				node.l = Add(node.l)
			return AVLTree.balance(node)

		self.head = Add(self.head)


	def delete(self, v):
		self.size -= 1
		def Delete(node):
			if v > node.v:
				node.r = Delete(node.r)
			elif v < node.v:
				node.l = Delete(node.l)
			else:
				l, r = node.l, node.r
				del node
				if not r:
					return l
				
				def del_min(n):
					if not n.l:
						return n.r
					n.l = del_min(n.l)
					return balance(n)
				t = AVLTree.get_min(r)
				t.r = del_min(r)
				t.l = l
				return AVLTree.balance(t)
			return AVLTree.balance(node)
		self.head = Delete(self.head)


	def max(self):
		return AVLTree.get_max(self.head).v

	def min(self):
		return AVLTree.get_min(self.head).v

	def nth_element(self, n):
		def Nth_element(node, k):
			l = node.l.size if node.l else 0
			if k == l:
				return node.v
			if k > l:
				return Nth_element(node.r, k - l - 1)
			return Nth_element(node.l, k)
		return Nth_element(self.head, n)

	def print(self):
		def Print(node):
			if node:
				Print(node.l)
				Print(node.r)
				print(node.v, node.h)
		Print(self.head)


if __name__ == '__main__':
	tree = AVLTree()
	for i in range(10):
		tree.add(i)
	tree.print();
	# print(tree.nth_element(64))
	# print(tree.min(), tree.max())
	# for i in range(15):
	# 	tree.delete(i)
	# print(tree.min(), tree.max())
	# for i in range(99, 80, -1):
	# 	tree.delete(i)
	# print(tree.min(), tree.max())
	# print(tree.nth_element(64))