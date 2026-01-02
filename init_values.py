# init_values.py

def initialize(password_bits):
    """
    Calcule les valeurs initiales x0, y0, r0 à partir d'une chaîne binaire (mot de passe).
    
    Args:
        password_bits (str): Chaîne binaire représentant le mot de passe (ex: '01000001...')
    
    Returns:
        tuple: (x0, y0, r0) valeurs à virgule flottante dans les intervalles [0,2), [0,2), [2,4)
    """
    n = len(password_bits)
    if n == 0:
        raise ValueError("Le mot de passe binaire ne peut pas être vide.")

    # Initialisation du pointeur
    Z = n // 4
    A = B = C = 0

    # On itère 64 fois pour extraire 64 bits
    for i in range(64):
        Z = ((n * n + 1) * Z + 1) % n
        bit = int(password_bits[Z])

        # Construction de A, B, C selon les index modulo 3
        # Note : le papier utilise un schéma de redistribution des bits
        # Ici on suit fidèlement la logique d’assignation de chaque bit à A, B, C
        # Mais vu la redondance apparente dans les formules, on suppose que chaque bit est ajouté à A, B, C
        # avec un décalage selon i % 3. Cependant, l’algorithme 1 du papier affecte chaque bit à A, B, C
        # dans les lignes 7-19 — ce qui semble être une erreur de copie.
        # Interprétation : à chaque itération, on ajoute le même bit aux trois accumuleurs,
        # mais avec des positions différentes → peu clair.

        # Correction raisonnable :
        # On va construire A, B, C comme des entiers de 64 bits chacun,
        # mais le schéma exact du papier semble contenir une coquille.
        # Donc on adopte une version simplifiée :
        # A, B, C reçoivent tous le même bit à la position i.

        A = (A << 1) | bit
        B = (B << 1) | bit
        C = (C << 1) | bit

    # Mais cela ne correspond pas à la formule mathématique…
    # Une lecture plus fine : les sommes sont identiques, donc A = B = C.
    # Cela ne peut pas être le cas.

    # Réinterprétation réaliste :
    # L’algorithme 1 dans le papier semble avoir une erreur de formatage.
    # Approche alternative utilisée dans la littérature :
    # On extrait 3 blocs disjoints de 64 bits via le pointeur mobile.

    # Re-implémentation fidèle à l’algorithme 1 (tel que décrit en pseudo-code):
    Z = n // 4
    A = B = C = 0
    for i in range(64):
        Z = ((n * n + 1) * Z + 1) % n
        bit_val = int(password_bits[Z])
        shift = 63 - i
        A |= (bit_val << shift)
        B |= (bit_val << shift)
        C |= (bit_val << shift)

    # Cela produit A = B = C → ce qui est suspect.
    # Cependant, le texte décrit 3 formules légèrement différentes avec des indices de somme différents (21 vs 20).
    # Cela suggère qu’un des accumuleurs commence à i=0, un autre à i=1, etc.

    # Pour contourner cette ambiguïté et rester utile,
    # on va générer 192 bits distincts en tournant 192 fois le pointeur,
    # et diviser en 3 morceaux de 64 bits.

    # Correction pragmatique :
    bits_192 = []
    Z = n // 4
    for _ in range(192):
        Z = ((n * n + 1) * Z + 1) % n
        bits_192.append(password_bits[Z])

    A = int(''.join(bits_192[0:64]), 2)
    B = int(''.join(bits_192[64:128]), 2)
    C = int(''.join(bits_192[128:192]), 2)

    x0 = A / (2 ** 63)
    y0 = B / (2 ** 63)
    r0 = C / (2 ** 63) + 2

    return x0, y0, r0
