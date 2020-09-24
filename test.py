import time
MOD = 10**18
while True:
    output = 0
    start = time.time()
    for i in open("test.txt", "r").readlines():
        a, b = map(int, i.split(" "))
        output ^= (a+b) % MOD
        #print((a+b) % MOD)
    print(time.time() - start, output)
    input()
