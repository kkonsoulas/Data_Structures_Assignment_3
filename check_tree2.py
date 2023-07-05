
class Node:
  def __init__(self, value, color, parent = None, left=None, right = None):
    self.value = value
    self.parent = parent
    self.left = left
    self.right = right
    self.color = color
    self.black_height = 0;
    
  def calc_black_height(self):
    if self == None:
      return 0
    left_black_height = 0
    right_black_height = 0
    left_color = 'B'
    right_color = 'B'
    
    if self.left != None:
      self.left.calc_black_height();
      left_black_height = self.left.black_height
      left_color = self.left.color
    if self.right != None:
      self.right.calc_black_height();
      right_black_height = self.right.black_height
      right_color = self.right.color
    
    if right_black_height == -1 or left_black_height == -1:
      self.black_height = -1
      return
      
    if right_black_height != left_black_height:
      self.black_height = -1
      return
    
    if self.color == 'R' and (left_color == 'R' or right_color == 'R'):
      self.black_height = -1;
      return;
    
    if self.color == 'R':
      self.black_height = left_black_height
    else:
      self.black_height = left_black_height + 1
      
  
  def find_node(self, value):
    if self == None:
      return none
    if value < self.value:
      if self.left == None:
        return self
      else:
        return self.left.find_node(value)
    elif value > self.value:
      if self.right == None:
        return self
      else:
        return self.right.find_node(value)
    else:
      return self
  
  def print_preorder(self):
    print(str(self.value), end = ' ')
    if self.left != None:
      self.left.print_preorder()
    if self.right != None:
      self.right.print_preorder()
    
class BinTree:

  def __init__(self):
    self.root = None
    self.size = 0
    
  def find_node(self, value):
    if self.size == 0:
      return None
    return self.root.find_node(value)
    
  def add(self, value, color):
    curr = self.root;
    if self.size == 0:
      node = Node(value, color)
      self.root = node
      self.size = 1;
      return True
    found = self.find_node(value)
    if found != None and found.value != value:
      node = Node(value, color, found)
      self.size += 1
      if value < found.value:
        found.left = node
      else:
        found.right = node
          
  def calc_black_height(self):
    self.root.calc_black_height()
    return self.root.black_height >= 0
    
  def print_preorder(self):
    self.root.print_preorder()
    print("")
    

''' Read student output and build binary tree '''
 
import math
import sys

def main():
  tree = BinTree()
  input_list = []
  
  with open(sys.argv[1], 'r') as reader:
    for line in reader:
      tokens = line.split()
      for token in tokens:
        token = int(token)
        if token < 0:
          break;
        input_list.append(token)
  
  input_list = set(input_list)
  #print("sys.argv[1]:", sys.argv[1])
  
  with open(sys.argv[2], 'r') as reader:
    for line in reader:
      first_std_line = line[:-1];
      break;
  
  #print("first std line:", first_std_line)
  
  i=0
  with open(sys.argv[3], 'r') as reader:
    for line in reader:
      line = line[:-1]
      #print('->', line);
      i += 1
      if i == 1:
        if line != first_std_line:
          sys.exit(100)
          print("\nFAILURE!!!\n")
        elif line == 'RB NOK':
          print("[NO] RedBlack Tree!")
          print("\nSUCCESS!!!\n")
          sys.exit(0)
        else:
          continue
          
      tokens = line.split()
      for token in tokens:
        token = token.split('#')
        #print(token)
        token[0] = int(token[0])
        if token[0] not in input_list:
          print(token[0], 'not in input_list!')
          print("[ERROR 1] Incompatible element between input list and tree!!")
          sys.exit(100)
        tree.add(token[0], token[1])
  
  #print("tree.size:", tree.size, "input_list.size:", len(input_list))
  if tree.size != len(input_list):
    print("[ERROR 2]:(", sys.argv[1],",", sys.argv[2], ") Incompatible size between input list and tree!!")
    sys.exit(100)
  
  if not tree.calc_black_height():
    print("[NO] RedBlack Tree!");
    print("\nFAILURE!!!\n")
    sys.exit(100)
  else:
    print("[YES] RedBlack Tree!");
    print("\nSUCCESS!!!\n")
    sys.exit(0)

if __name__ == "__main__":
    main()
 
