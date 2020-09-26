import random
random.seed(int(input("Seed: ")))
limit = int(input("Num limit: "))
s = -(10**limit)
e = 10**limit
with open("test.txt", "w") as f:
    for rep in range(10**6): # 1M lines, max 200MB file
        #f.write(f"{random.randint(s, e)} {random.randint(s, e)}\n")
        f.write(f"{random.randint(0, e)} {random.randint(0, e)}\n")

