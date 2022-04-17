

from collections import defaultdict
class graph:
    def __init__(self):
        self.graph = defaultdict(set)
    def connectnode(self,node1,node2):
        self.graph[node1].add(node2)
        self.graph[node2].add(node1)
    def __str__(self):
        return self.graph
    def visualize(self,size):
        maxy = max(self.graph.keys())
        l = [[" " for i in range(size*size)] for i in range(size+3)]
        #print("Initialization of l:",l)
        k = 0
        backslash = "\\"
        for i in range(0,size+3,2):
            #print("Moving row by row :",i)
            for j in range(0,size*size):
                #print("Moving column by column:",j)
                if j%(size+1) == 0:
                    #print("Encountered number postion :",k)
                    l[i][j] = str(k)
                else: continue
                for node in self.graph[k]:
                    #print("Node = ",node,"for k = ",k)
                    node = int(node)
                    if node == k-size:
                        #print("Top")
                        l[i-1][j] == "|"
                    elif node == k-size+1:
                        #print("Top Right")
                        if l[i-1][j+2] == backslash:
                            print("Updating connection to x")
                            l[i-1][j+2] = "x"
                        else:
                            l[i-1][j+2] = "/"
                    elif node == k+1:
                        #print("Right")
                        l[i][j+2] = "-"
                    elif node == k+size+1:
                        #print("Bottom Right")
                        if l[i+1][j+2] == "/":
                            #print("Updating connection to x")
                            l[i+1][j+2] = "x"
                        else:
                            l[i+1][j+2] = backslash
                    elif node == k+size:
                        #print("Bottom")
                        l[i+1][j] = "|"
                    elif node == k+size-1:
                        #print("Bottom Left")
                        if l[i+1][j-2] == backslash:
                            #print("Updating connection to x")
                            l[i+1][j-2] = "x"
                        else:
                            l[i+1][j-2] = "/"
                    elif node == k-1:
                        #print("Left")
                        l[i][j-2] == "-"
                    elif node == k-size-1:
                        #print("Top Left")
                        if l[i-1][j-2] == "/":
                            #print("Updating connection to x")
                            l[i-1][j-2] = "x"
                        else:
                            l[i-1][j-1] = backslash
                    else:
                        print("Error in relation b/w current node {} and connected node {}".format(k,node))
                        exit()
                k+=1
        #print(l)
        s = ""
        for sublst in l:
            for index,char in enumerate(sublst):
                if char == "\\" and sublst[index+1] == "\\":
                    #print("Repeating")
                    sublst[index+1] = " "
            #print("sublst",sublst)
            s += "".join(sublst)
            s += "\n"
        return s
                

g1 = graph()
g1.connectnode(0,1)
g1.connectnode(0,4)
g1.connectnode(0,3)
g1.connectnode(1,2)
g1.connectnode(1,4)
g1.connectnode(1,5)
g1.connectnode(2,4)
g1.connectnode(2,5)
g1.connectnode(3,4)  
g1.connectnode(3,6)
g1.connectnode(3,7)
g1.connectnode(4,5)
g1.connectnode(4,7)
g1.connectnode(4,6)
g1.connectnode(5,7)
g1.connectnode(5,8)
g1.connectnode(6,7)
g1.connectnode(7,8)
print(g1.visualize(3))

'''        
## 0123456789
   1 - 2 - 3
   | \ | x |
   4 - 5 - 6
   | x | / |
   7 - 8 - 9

0123456789
'''
