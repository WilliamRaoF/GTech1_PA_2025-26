# 1. Introduction

Ce document décrit deux fonctionnalités à implémenter dans le cadre du projet :

1. **Ajouter un checksum dans plusieurs pixels du début de l’image** (BMP 24 bits).
2. **Comparer deux images pixel par pixel** afin de détecter une modification.

Les consignes sont adaptées au format **BMP 24 bits**, qui stocke **3 octets par pixel** (B, G, R).

---

# 2. Exercice 1 — Insérer un Checksum dans les N premiers pixels

## Objectif

Insérer un **checksum (CRC32)** et quelques informations de contrôle (magic, taille du message…) dans un bloc de **pixels réservés** au début de l’image.

Étant donné qu’un BMP 24 bits ne contient **que 3 octets par pixel**, il faut utiliser **davantage de pixels** qu’un BMP 32 bits.

---

# 2.1 Nombre de pixels nécessaires

### Octets disponibles par pixel (BMP 24 bits) :
- **3 octets par pixel** : B, G, R

### Taille recommandée du header :
| Champ | Taille |
|--------|---------|
| Magic "STEG" | 4 octets |
| Version | 1 octet |
| Réservé | 1 octet |
| Taille du message | 4 octets |
| CRC32 | 4 octets |
| **Total du header** | **14 octets** |

### Pixels nécessaires :
```

14 octets ÷ 3 octets/pixel = 4.66 → 5 pixels

```

### Recommandation :
Réserver **6 pixels** pour avoir une marge de 18 octets.

---

# 2.2 Structure du header (18 octets / 6 pixels)

```

Offset 0 : 'S'
Offset 1 : 'T'
Offset 2 : 'E'
Offset 3 : 'G'
Offset 4 : Version (1 octet)
Offset 5 : Réservé
Offset 6-9   : Taille du message (4 octets)
Offset 10-13 : CRC32 (4 octets)
Offset 14-17 : Padding ou extension

```

Total : **18 octets = 6 pixels BMP 24 bits**

---

# 2.3 Modes d’écriture

### Mode conseillé : écriture directe (octets complets)
On écrit directement les 18 octets dans les 6 premiers pixels :

- Pixel 0 → bytes 0,1,2  
- Pixel 1 → bytes 3,4,5  
- Pixel 2 → bytes 6,7,8  
- etc.

Ce mode est **visible** dans certains cas (pixels modifiés), mais généralement acceptable si les 6 pixels sont peu importants.

---

### Mode alternatif : écriture en LSB (très discrète)
Chaque pixel permet d’écrire **3 bits** (1 par canal) → extrêmement faible capacité :

- 6 pixels × 3 bits = **18 bits** (~2 octets) → insuffisant.

Donc **ce mode nécessite plus de pixels**.

---

# 2.4 Étapes à réaliser

### Étape 1 : Calcul du CRC32 du message  
### Étape 2 : Construire le header (18 octets)  
### Étape 3 : Écrire le header dans les 6 premiers pixels  
### Étape 4 : Lors de la lecture :
- Lire les 18 octets
- Vérifier :
  - Magic `"STEG"`
  - Longueur valide
  - CRC32 correct

En cas d’erreur → message corrompu / fichier modifié.

---

# 3. Exercice 2 — Comparaison de Deux Images (BMP 24 bits)

## Objectif

Comparer deux images BMP **pixel par pixel** :
- Image A (référence)
- Image B (modifiée ou non)

pour détecter une éventuelle modification (stéganographie ou autre).

---

# 3.1 Contraintes

- Les deux BMP doivent avoir :  
  - même largeur  
  - même hauteur  
  - même profondeur (24 bits)  

Sinon → comparaison impossible.

---

# 3.2 Méthodes de comparaison

---

## Mode 1 : Comparaison stricte (octet par octet)

Pour chaque pixel :
- Comparer les 3 octets : B, G, R
- Si un octet diffère → pixel différent

Détecte :
- stéganographie
- modification d’image
- corruption de fichier

---

## Mode 2 : Comparaison LSB (spécifique stéganographie)

Pour chaque pixel :
- `(A.B & 1)` vs `(B.B & 1)`
- `(A.G & 1)` vs `(B.G & 1)`
- `(A.R & 1)` vs `(B.R & 1)`

Détecte uniquement :
- les messages cachés par LSB

---

# 3.3 Résultats attendus

Le programme doit afficher :

### Nombre total de pixels modifiés  
### Nombre total de canaux modifiés  
### Nombre total de bits LSB modifiés  
### Verdict global :
```

Aucune différence détectée.

```
ou
```

Différences détectées : l’image a été modifiée.

```

---

# 3.4 (Optionnel) Image de différence visuelle

Créer une nouvelle image BMP :
- noir si pixel identique,
- rouge (255,0,0) si pixel différent.

Permet une visualisation claire des modifications.

---

# 4. Conclusion

Ces deux exercices permettent de :

- manipuler une structure BMP 24 bits,  
- comprendre les limites de la capacité par pixel,  
- implémenter un header et un checksum fiable,  
- détecter les modifications apportées à une image,  
- visualiser l’effet d’un encodage LSB.

Ces éléments complètent parfaitement le projet de stéganographie Win32.
