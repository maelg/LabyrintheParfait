def empile(pile, elem):
    pile.append(elem)
def creerPile():
    return []
def depile(pile):
    return pile.pop()
def estVide(pile):
    return pile == []
def last(pile):
	return pile[-1]