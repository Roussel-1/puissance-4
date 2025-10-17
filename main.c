#include <stdio.h>
#define LIGNES 7
#define COLONNES 9

// Couleurs ANSI (fonctionne dans le terminal Ubuntu)
#define RESET   "\x1b[0m"
#define ROUGE   "\x1b[31m"
#define JAUNE   "\x1b[33m"


// Initialiser la grille à 0
void initialiserGrille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++)
        for (int j = 0; j < COLONNES; j++)            grille[i][j] = 0;
}

// Afficher la grille "agrandie"
void afficherGrille(int grille[LIGNES][COLONNES]) {
    printf("\n");

    for (int i = 0; i < LIGNES; i++) {
        // ligne du haut
        for (int j = 0; j < COLONNES; j++)
            printf("+----------");
        printf("+\n");

        // 🔹 Première sous-ligne vide (haut de la case)
        for (int j = 0; j < COLONNES; j++)
            printf("|          ");
        printf("|\n");

        // 🔹 Deuxième sous-ligne avec le pion
        for (int j = 0; j < COLONNES; j++) {
            if (grille[i][j] == 0)
                printf("|          ");
            else if (grille[i][j] == 1)
                printf("|     %s⬤%s    ", ROUGE, RESET);
            else
                printf("|     %s⬤%s    ", JAUNE, RESET);
        }
        printf("|\n");

        // 🔹 Troisième sous-ligne vide (bas de la case)
        for (int j = 0; j < COLONNES; j++)
            printf("|          ");
        printf("|\n");
    }

    // ligne du bas finale
    for (int j = 0; j < COLONNES; j++)
        printf("+----------");
    printf("+\n");

    // numéros de colonnes
    for (int j = 1; j <= COLONNES; j++)
        printf("     %d     ", j);
    printf("\n\n");
}
// Jouer un coup
int jouerCoup(int grille[LIGNES][COLONNES], int joueur, int colonne) {
    if (colonne < 0 || colonne >= COLONNES)
        return 0;

    for (int i = LIGNES - 1; i >= 0; i--) {
        if (grille[i][colonne] == 0) {
            grille[i][colonne] = joueur;
            return 1;
        }
    }
    return 0; // colonne pleine
}

// Vérifier la victoire (4 alignés)
int verifierVictoire(int grille[LIGNES][COLONNES], int joueur) {
    // Horizontale
    for (int i = 0; i < LIGNES; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i][j+1] == joueur &&
                grille[i][j+2] == joueur && grille[i][j+3] == joueur)
                return 1;

    // Verticale
    for (int i = 0; i < LIGNES - 3; i++)
        for (int j = 0; j < COLONNES; j++)
            if (grille[i][j] == joueur && grille[i+1][j] == joueur &&
                grille[i+2][j] == joueur && grille[i+3][j] == joueur)
                return 1;

    // Diagonale ↘
    for (int i = 0; i < LIGNES - 3; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i+1][j+1] == joueur &&
                grille[i+2][j+2] == joueur && grille[i+3][j+3] == joueur)
                return 1;

    // Diagonale ↗
    for (int i = 3; i < LIGNES; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i-1][j+1] == joueur &&
                grille[i-2][j+2] == joueur && grille[i-3][j+3] == joueur)
                return 1;

    return 0;
}

// Vérifier si la grille est pleine
int estPleine(int grille[LIGNES][COLONNES]) {
    for (int j = 0; j < COLONNES; j++)
        if (grille[0][j] == 0)
            return 0;
    return 1;
}

int main() {
    int grille[LIGNES][COLONNES];
    int joueur = 1;
    int colonne, victoire = 0;

    initialiserGrille(grille);

    printf("=== PUISSANCE 4 CLASSIQUE ===\n");
    afficherGrille(grille);

    while (!victoire && !estPleine(grille)) {
        printf("Joueur %d (%s%s%s), choisissez une colonne (1-%d) : ",
               joueur,
               (joueur == 1 ? ROUGE : JAUNE),
               (joueur == 1 ? "X" : "O"),
               RESET,
               COLONNES);

        scanf("%d", &colonne);
        colonne--;

        if (jouerCoup(grille, joueur, colonne)) {
            afficherGrille(grille);

            if (verifierVictoire(grille, joueur)) {
                printf("🎉 Bravo Joueur %d ! Vous avez gagné !\n", joueur);
                victoire = 1;
            } else {
                joueur = (joueur == 1) ? 2 : 1; // changement de joueur
            }
        } else {
            printf("⚠️ Colonne invalide ou pleine, réessayez.\n");
        }
    }

    if (!victoire)
        printf("Match nul ! La grille est pleine.\n");

    return 0;
}
