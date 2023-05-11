lst = []

try:
    while True:
        lst.append(input())
except:
    pass

print(len(set(lst)))
print(len(lst))