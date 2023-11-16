from multiprocessing.pool import Pool
import math

def task(n):
    if n<=1:
         return
    if (n==2):
         return n
    if (n%2==0):
         return

    for i in range(3,int(math.sqrt(n)+1), 2):     
        if(n%i==0):
             return
    return n

if __name__ == '__main__':
    pool = Pool()
    numbers = list(range(100))
    results = pool.map(task, numbers)
    res = list(filter(lambda item: item is not None, results)) #filtrando valores nulos
    print("total de primos: ", len(res))
    print("primos: ", res )
