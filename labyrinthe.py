import numpy as np
import matplotlib.pyplot as plt
import random

longueur, largeur = 40, 40


class Labyrinthe:

    def __init__(self, l, c):
        """
        Le labyrinthe initial
        Chaque case est marquée comme non visitée (0)
        Chaque cloisons est fermée (0)
        """
        self.laby = [[[0, [0, 0, 0, 0]] for i in range(c)] for k in range(l)]

    def draw(self):
        matrice = np.zeros((longueur*10+(longueur)*2, largeur*10+(largeur)*2)) + 1
        matrice[::12, :] = 0
        matrice[:, ::12] = 0
        for l in range(longueur):
            for c in range(largeur):
                #matrice[l*12:l*12+1, c*12+1:c*12+12] = laby.laby[l][c][1][0]  # Bordure nord
                matrice[l*12+1:l*12+12, c*12+12:c*12+13] = self.laby[l][c][1][1]
                matrice[l*12+12:l*12+13, c*12+1:c*12+12] = self.laby[l][c][1][2]
                #matrice[l*12+1:l*12+12, c*12:c*12+1] = laby.laby[l][c][1][3]
        
        plt.matshow(matrice, fignum=100, cmap='gray')
        plt.show()


laby = Labyrinthe(longueur, largeur)

j = 0


def bordure_available(laby, l, c):
    """
    Renvoie une valeur appartenant à (0,1,2,3)
    correspondant respectivement à (Nord, Est, Sud, Ouest)
    Renvoie -1 si toute les cases autour ont été visités
    """
    a = [0, 1, 2, 3]
    if l == 0:
        a.remove(0)
    if c == 0:
        a.remove(3)
    if l == longueur-1:
        a.remove(2)
    if c == largeur-1:
        a.remove(1)
    if(0 in a):
        if (laby.laby[l-1][c][0]):
            a.remove(0)
    if 1 in a and laby.laby[l][c+1][0]:
        a.remove(1)
    if 2 in a and laby.laby[l+1][c][0]:
        a.remove(2)
    if 3 in a and laby.laby[l][c-1][0]:
        a.remove(3)

    print(a)
    if a == []:
        return -1

    return random.choice(a)


def ouvrir_bordure(laby, l, c, last):
    """
    Fonction récursive
    Créer le labyrinthe
    Ouvre les cloisons tant qu'elle peut
    """

    global j
    j += 1  # Compteur

    # Case actuelle signalée comme visitée
    laby.laby[l][c][0] = 1

    # On récupere une case non visitée autour (ou aucune)
    b = bordure_available(laby, l, c)

    # Sert pour la condition de sortie
    last = (l, c)

    if b == 0:
        # laby.laby[l][c][1][0] = 1
        laby.laby[l-1][c][1][2] = 1
        l, c = l-1, c
        print("Nord")
    elif b == 1:
        laby.laby[l][c][1][1] = 1
        # laby.laby[l][c+1][1][3] = 1
        l, c = l, c+1
        print("Est")
    elif b == 2:
        laby.laby[l][c][1][2] = 1
        # laby.laby[l+1][c][1][0] = 1
        l, c = l+1, c
        print("Sud")
    elif b == 3:
        # laby.laby[l][c][1][3] = 1
        laby.laby[l][c-1][1][1] = 1
        l, c = l, c-1
        print("Ouest")
    if b != -1:
        ouvrir_bordure(laby, l, c, last)
    if last != (l, c):  # Si on ne reste pas bloqué dans la même case
        ouvrir_bordure(laby, l, c, last)



#ouvrir_bordure(laby, random.randint(0, longueur-1), random.randint(0, largeur-1))
ouvrir_bordure(laby, 5, 5, (5, 5))
print(j)

laby.draw()
