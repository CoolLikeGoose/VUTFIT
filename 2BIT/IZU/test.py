import numpy as np

# Nastavíme body zadané ve 3D prostoru
body = np.array([
    [ 6, -8,  2],
    [-3, -1,  0],
    [ 9, -9,  7],
    [-7,  2,  3],
    [-2, -2,  5],
    [ 0,  5, -9],
    [ 7, -4,  1],
    [ 9, -6,  6],
    [-6, -4, -4],
    [ 5,  5, -9]
])

# Nastavíme počáteční středy shluků
stredy = np.array([
    [ 9, -6,  6],
    [ 6, -8,  2],
    [ 9, -9,  7]
])

# Funkce pro výpočet vzdálenosti mezi body
def vzdalenost(body, stredy):
    vzdalenosti = np.sqrt(((body - stredy[:, np.newaxis])**2).sum(axis=2))
    return vzdalenosti

# K-means algoritmus
def kmeans(body, stredy, iterace=100):
    for i in range(iterace):
        print(f"Iteration {i+1} středy:")
        vzd = vzdalenost(body, stredy)
        shluky = np.argmin(vzd, axis=0)
        nove_stredy = np.array([body[shluky == k].mean(axis=0) for k in range(stredy.shape[0])])
        print("Body: ", end='')
        print(shluky)
        print(nove_stredy)
        
        # Pokud se středy nezměnily, ukončíme iterace
        if np.all(stredy == nove_stredy):
            break
        
        stredy = nove_stredy
    
    return shluky, stredy

# Spustíme algoritmus
shluky, vysledne_stredy = kmeans(body, stredy)

# Zaokrouhlíme výsledné středy na dvě desetinná místa
vysledne_stredy = np.round(vysledne_stredy, 2)

# Vypsání výsledků
print("Výsledné středy:")
print(vysledne_stredy)
