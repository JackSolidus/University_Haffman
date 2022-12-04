import heapq
from collections import Counter, namedtuple
from pathlib import Path

class Node(namedtuple("Node", ["left", "right"])):
    def walk(self, code, acc):
        self.left.walk(code, acc + "0")
        self.right.walk(code, acc + "1")

class Leaf(namedtuple("Leaf", ["char"])):
    def walk(self, code, acc):
        code[self.char] = acc or "0"
    def get(self):
        return self.char


def huffmanEncode(path):
    h = []#Счётчик элементов списка
    with open(path,"r",encoding='utf-8') as f:
        for line in f:            
            Counter(line)
    for ch, freq in Counter(line).items():
        h.append((freq, len(h), Leaf(ch)))
    heapq.heapify(h) #Сортируем элементы списка
    #print(h)
    count = len(h)
    y=[int(count)]
    x = bytes(y)
    with open (path+'.haffman','wb') as f: #Записываем ко-во элементов в списке
        f.write(x)
        for i in h:
            f.write(bytes(i[2].get(),"utf-8"))
            y=[i[1]]
            f.write(bytes(y))
            y=[i[0]]
            f.write(bytes(y))

    while len(h)>1: #Собираем дерево
        freq1, _count1, left = heapq.heappop(h)
        freq2, _count2, right = heapq.heappop(h)
        heapq.heappush(h, (freq1 + freq2, count, Node(left, right)))
        count += 1
    code={}
    if h:
        [(_freq, count, root)] = h
        root.walk(code, "")

    return code

def toBytes(encoded, path):
  b = bytearray()
  for i in range(0, len(encoded), 8):
    b.append(int(encoded[i:i+8], 2))
    with open(path + '.haffman', 'ab') as f: #Перевод закодированной байтовой строки в байты для записи
        f.write(b)
    b = bytearray()
  return bytes(b)


def doArchive(path):
    code = huffmanEncode(path) 
    encoded = ''
    with open(path,"r",encoding='utf-8') as f:
        for line in f:
            for i in line:
                for k, v in code.items():
                    if i == k:
                        encoded +=v
    #print(len(code), len(encoded))
    for ch in sorted(code):
        print("{}: {}".format(ch, code[ch]))
    #print(encoded)
    with open(path + '.haffman', 'ab') as f: #Записываем остаток битов
        #print(len(encoded)%8)
        mod=[len(encoded) % 8]
        f.write(bytes(mod))
    toBytes(encoded, path)

def readArchive(path):
    h = []
    elem = 0
    numberOfBytes=0
    with open (path,'rb') as f:#Перевод байтов в int
        elem = ord(f.read(1)) #Читаем кол-во элементов словаря
        #print(elem)
        for c in range(0, elem, 1): #Считываем словарь
            ch=f.read(1)
            ch = ch.decode('UTF-8')
            arc = ord(f.read(1))
            freq = ord(f.read(1))
            h.append((freq, arc, Leaf(ch)))
        #print(h)
        count = len(h)
        while len(h)>1: #Собираем дерево
            freq1, _count1,left = heapq.heappop(h)
            freq2, _count2,right = heapq.heappop(h)
            heapq.heappush(h, (freq1 + freq2, count, Node(left, right)))
            count += 1
        code={} #Словарь
      
        if h:
            [(_freq, count, root)] = h
            root.walk(code, "")
        #for ch in sorted(code):
            #print("{}: {}".format(ch, code[ch]))
        f.read(1)
        i=1
        EOF = f.read()
        size = len(EOF)


    with open(path, 'rb') as f:
        toDecode=''
        elem = elem * 3 + 1
        for i in range(elem):
            f.read(1)
        mod = ord(f.read(1))
        if mod > 0:
            while size > 1:
                x = f.read(1)           
                toDecode += '{:08b}'.format(ord(x))
                size -= 1
            x=f.read(1)
            mod = 8-mod
            toDecode += '{:0{}b}'.format(ord(x),8-mod)
        else:
            while size > 0:
                x = f.read(1)           
                toDecode += '{:08b}'.format(ord(x))
                size -= 1
       
        buff = ''
        decoded=''
        i=0
        for ch in toDecode:
            buff += toDecode[i]
            i += 1
            for k, v in code.items():
                if buff == v:
                    decoded +=k
                    with open(path + '.haf', 'a',encoding='utf-8') as f: #Записываем раскодированное сообщение
                        f.write(decoded)
                        #print(decoded)
                    decoded=''
                    buff = ''
    
    

def main():
    path=''
    i=1
    while i:
        print('select operating mode')
        s=input()   
        if s == 'w':
            print('Inputh path to file')
            path = input()
            doArchive(path)
            print('done')
            break
        elif s == 'r':
            print('Inputh path to file')
            path = input()
            readArchive(path)
            print('done')
            break
        elif s == 'exit':
            print('Bye')
            break;
        else:
           print('w - for packing\nr - for unpacking\nexit - for exit')
    

if __name__ == "__main__":
    main()
