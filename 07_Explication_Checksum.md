# Cours rapide : Le Checksum (CRC32)

## 1. Qu’est-ce qu’un checksum ?

Un **checksum** est une valeur numérique calculée à partir d’un ensemble de données (texte, fichier, message…).  
Il sert à **vérifier l’intégrité** des données :  
*c’est-à-dire détecter si elles ont été modifiées, même légèrement.*

C’est un outil essentiel en sécurité, en transmission de données, en fichiers numériques…  
et **dans ce projet**, il sert à vérifier que le **message caché dans l’image n’a pas été altéré**.

---

## 2. Pourquoi utiliser un checksum ?

Parce qu’une image contenant un message caché peut être :

- modifiée accidentellement  
- altérée lors de l’enregistrement  
- modifiée volontairement  

Même si la modification est invisible à l’œil, un checksum permet de la détecter.

### Exemple :
- Message original → `CRC32 = A1B2C3D4`
- Message extrait → `CRC32 = F0002288`

Les valeurs sont différentes → le message **n’est plus celui d’origine**.

---

## 3. Comment ça marche ?

### Étape 1 : On a un message (texte)
Exemple :
```

HELLO

```

### Étape 2 : On le convertit en octets
```

48 45 4C 4C 4F

```

### Étape 3 : On calcule le checksum (ex : CRC32)
Résultat :
```

0x1A2B3C4D

```

Cette valeur représente un "résumé" du message.

### Étape 4 : On stocke le checksum dans l’image
Dans notre projet :  
on écrit le CRC32 dans les **6 premiers pixels** du BMP 24 bits.

### Étape 5 : Lors de l’extraction, on recalcule le CRC32
Si le CRC extrait ≠ CRC calculé  
**le message est corrompu**.

---

## 4. Le CRC32 en particulier

Le **CRC32** (Cyclic Redundancy Check 32 bits) est un des checksums les plus utilisés :

- Taille : **32 bits (4 octets)**
- Très rapide
- Très fiable  
- Excellente détection des petites modifications

Il est utilisé dans :
- PNG  
- ZIP  
- Protocoles réseau  
- Systèmes de fichiers  

C’est pour cela qu’on l’utilise dans ce projet.

---

## 5. Exemple visuel simplifié

### Encodage :

| Donnée | Valeur |
|--------|---------|
| Message | "HELLO" |
| CRC32 | `0x1A2B3C4D` |
| Pixels 0-5 | contiennent le header |
| Pixels suivants | contiennent le message en LSB |

### Extraction :

1. Lire le header → récupérer CRC32 (`1A2B3C4D`)
2. Extraire le message caché
3. Refaire un CRC32 sur ce message
4. Si égal → message OK  
5. Sinon → message modifié

---

## 6. Résumé rapide (pour mémoriser)

- Un **checksum** vérifie qu’une donnée n’a pas été modifiée.  
- Le **CRC32** est un checksum très fiable et utilisé dans de nombreux formats.  
- Dans ce projet, il garantit que le message stéganographié est **intact**.  
- On stocke le checksum dans les **premiers pixels** de l’image.  
- Lors de l’extraction, on compare :  
  - **CRC extrait**  
  - **CRC calculé**

Si les deux correspondent → **message valide**  
Sinon → **message altéré**

---

## 7. Conclusion

Le checksum est un mécanisme simple mais indispensable pour assurer l’intégrité des données cachées.  
Il permet de détecter les erreurs, les corruptions et les modifications dans une image BMP, même si celles-ci sont invisibles à l’œil.

Ce petit mécanisme rend la stéganographie bien plus robuste et professionnelle.

