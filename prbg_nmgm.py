# prbg_nmgm.py

import math

def nmgm_map(x, y, r):
    """
    Fonction de la carte Gingerbreadman modifiée (NMGM).
    """
    x_next = 1 - y + abs(x)
    y_next = x + r * math.sin(y)
    return x_next, y_next

def generate_sequence(password_str, F):
    """
    Génère une séquence binaire pseudo-aléatoire de longueur F bits.
    
    Args:
        password_str (str): Mot de passe en texte clair (ex: "ABIDINE")
        F (int): Nombre de bits désirés en sortie (multiple de 32 recommandé)
    
    Returns:
        str: Séquence binaire de longueur F
    """
    # Conversion du mot de passe en binaire (ASCII)
    password_bits = ''.join(format(ord(c), '08b') for c in password_str)
    n = len(password_bits)

    if n == 0:
        raise ValueError("Mot de passe vide")

    # Étape 1 : Initialisation
    from init_values import initialize
    x, y, r = initialize(password_bits)

    # Calcul de M = r * length(Pw)
    M = int(r * n)

    S_bits = []
    i = 0
    generated_blocks = 0
    max_blocks = (F + 31) // 32  # nombre de blocs de 32 bits nécessaires

    # Étape 2 : Discard des premières itérations (transitoire)
    for _ in range(M):
        x, y = nmgm_map(x, y, r)
        i += 1

    # Étape 3 : Génération
    while generated_blocks < max_blocks:
        if (i % n) != 0:
            x, y = nmgm_map(x, y, r)
            # Extraire la partie fractionnaire
            x_frac = x - math.floor(x)
            y_frac = y - math.floor(y)
            # Convertir en entiers 32 bits
            X = int((x_frac % 1) * (2 ** 32)) & 0xFFFFFFFF
            Y = int((y_frac % 1) * (2 ** 32)) & 0xFFFFFFFF
            R = X ^ Y
            # Ajouter les 32 bits en binaire
            S_bits.append(format(R, '032b'))
            generated_blocks += 1
        else:
            # Mise à jour de r
            r = ((r + 1) ** 2) % 2 + 2
            x, y = nmgm_map(x, y, r)  # on avance quand même
        i += 1

    full_sequence = ''.join(S_bits)
    return full_sequence[:F]  # tronquer à F bits exactement
