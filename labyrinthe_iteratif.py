import numpy as np
import matplotlib.pyplot as plt
import random
from pile import *

def creerLabyrinthe(m, n):
    laby = [[[0, [0, 0, 0, 0]] for i in range(n)] for k in range(m)]
    l = creerPile()
    empile(l, (0, 0))
    while not estVide(l):
        casserCloison(laby, l)
    return laby

def choisirCloison(laby, case):
    """
    Renvoie une valeur appartenant à (0,1,2,3)
    correspondant respectivement à (Nord, Est, Sud, Ouest)
    Renvoie -1 si toute les cases autour ont été visités
    """
    l, c = case
    """
    a = [0, 1, 2, 3]
    if l == 0:
        a.remove(0)
    if c == 0:
        a.remove(3)
    if l == len(laby)-1:
        a.remove(2)
    if c == len(laby[0])-1:
        a.remove(1)
    if(0 in a):
        if (laby[l-1][c][0]):
            a.remove(0)
    if 1 in a and laby[l][c+1][0]:
        a.remove(1)
    if 2 in a and laby[l+1][c][0]:
        a.remove(2)
    if 3 in a and laby[l][c-1][0]:
        a.remove(3)
    """

    a=[]
    if l != 0 and laby[l-1][c][0] == 0:
        a.append(0)
    if c != len(laby[0])-1 and not laby[l][c+1][0]:
        a.append(1)
    if l != len(laby)-1 and not laby[l+1][c][0]:
        a.append(2)
    if c != 0 and not laby[l][c-1][0]:
        a.append(3)

    if a == []:
        return -1

    return random.choice(a)

def casserCloison(laby, l):
    x, y = last(l)
    if 0 <= x < len(laby) and 0 <= y < len(laby[0]):
        laby[x][y][0] = 1
        b = choisirCloison(laby, (x, y))
        if b == 0:
            # laby.laby[l][c][1][0] = 1
            laby[x-1][y][1][2] = 1
            x, y = x-1, y
        elif b == 1:
            laby[x][y][1][1] = 1
            # laby.laby[l][c+1][1][3] = 1
            x, y = x, y+1
        elif b == 2:
            laby[x][y][1][2] = 1
            # laby.laby[l+1][c][1][0] = 1
            x, y = x+1, y
        elif b == 3:
            # laby.laby[l][c][1][3] = 1
            laby[x][y-1][1][1] = 1
            x, y = x, y-1
        if not b == -1:
            empile(l, (x, y))
        else:
            depile(l)
    else:
        depile(l)
m, n = 9, 16
laby = creerLabyrinthe(m, n)
print(laby)
matrice = np.zeros((m*10+(m)*2+1, n*10+(n)*2+1)) + 1
matrice[::12, :] = 0
matrice[:, ::12] = 0
for l in range(m):
    for c in range(n):
                #matrice[l*12:l*12+1, c*12+1:c*12+12] = laby.laby[l][c][1][0]  # Bordure nord
        matrice[l*12+1:l*12+12, c*12+12:c*12+13] = laby[l][c][1][1]
        matrice[l*12+12:l*12+13, c*12+1:c*12+12] = laby[l][c][1][2]
                #matrice[l*12+1:l*12+12, c*12:c*12+1] = laby.laby[l][c][1][3]

plt.matshow(matrice, fignum=100, cmap='gray')
plt.show()
