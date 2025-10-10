#include <stdio.h>

#define LIGNES 6
#define COLONNES 7

// Fonction pour initialiser la grille à 0 (vide)
void initialiserGrille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            grille[i][j] = 0;
        }
    }
}

// Fonction pour afficher la grille
void afficherGrille(int grille[LIGNES][COLONNES]) {
    printf("\n");
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (grille[i][j] == 0) printf("| . ");
            else if (grille[i][j] == 1) printf("| X ");
            else printf("| O ");
        }
        printf("|\n");
    }
    printf("  1   2   3   4   5   6   7\n");
    printf("\n");
}

// Fonction pour jouer un coup (retourne 1 si OK, 0 si colonne pleine)
int jouerCoup(int grille[LIGNES][COLONNES], int joueur, int colonne) {
    if (colonne < 0 || colonne >= COLONNES) return 0;

    for (int i = LIGNES - 1; i >= 0; i--) {
        if (grille[i][colonne] == 0) {
            grille[i][colonne] = joueur;
            return 1;
        }
    }
    return 0; // colonne pleine
}

// Vérifie si un joueur a gagné
int verifierVictoire(int grille[LIGNES][COLONNES], int joueur) {
    // Vérification horizontale
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES - 3; j++) {
            if (grille[i][j] == joueur && grille[i][j+1] == joueur &&
                grille[i][j+2] == joueur && grille[i][j+3] == joueur)
                return 1;
        }
    }

    // Vérification verticale
    for (int i = 0; i < LIGNES - 3; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (grille[i][j] == joueur && grille[i+1][j] == joueur &&
                grille[i+2][j] == joueur && grille[i+3][j] == joueur)
                return 1;
        }
    }

    // Vérification diagonale (bas droite)
    for (int i = 0; i < LIGNES - 3; i++) {
        for (int j = 0; j < COLONNES - 3; j++) {
            if (grille[i][j] == joueur && grille[i+1][j+1] == joueur &&
                grille[i+2][j+2] == joueur && grille[i+3][j+3] == joueur)
                return 1;
        }
    }

    // Vérification diagonale (haut droite)
    for (int i = 3; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES - 3; j++) {
            if (grille[i][j] == joueur && grille[i-1][j+1] == joueur &&
                grille[i-2][j+2] == joueur && grille[i-3][j+3] == joueur)
                return 1;
        }
    }

    return 0; // pas de victoire
}

// Vérifie si la grille est pleine (match nul)
int estPleine(int grille[LIGNES][COLONNES]) {
    for (int j = 0; j < COLONNES; j++) {
        if (grille[0][j] == 0) return 0;
    }
    return 1;
}

int main() {
    int grille[LIGNES][COLONNES];
    int joueur = 1;
    int colonne;
    int victoire = 0;

    initialiserGrille(grille);

    printf("=== Puissance 4 ===\n");
    afficherGrille(grille);

    while (!victoire && !estPleine(grille)) {
        printf("Joueur %d, choisissez une colonne (1-7) : ", joueur);
        scanf("%d", &colonne);
        colonne--; // pour correspondre à l'indice du tableau (0 à 6)

        if (jouerCoup(grille, joueur, colonne)) {
            afficherGrille(grille);

            if (verifierVictoire(grille, joueur)) {
                printf("🎉 Joueur %d gagne !\n", joueur);
                victoire = 1;
            } else {
                joueur = (joueur == 1) ? 2 : 1; // changer de joueur
            }
        } else {
            printf("Colonne invalide ou pleine, réessayez.\n");
        }
    }

    if (!victoire) {
        printf("Match nul ! La grille est pleine.\n");
    }

    return 0;
}
