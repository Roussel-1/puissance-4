#include <SDL2/SDL.h>
#include <stdio.h>

#define LIGNES 6
#define COLONNES 7
#define CELL_SIZE 100  // taille d'une case en pixels
int i, j;

void DrawCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderDrawPoint(renderer, cx + x, cy + y);
            }
        }
    }
}

// --- Fonction pour initialiser la grille ---
void initialiserGrille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++)
        for (int j = 0; j < COLONNES; j++)
            grille[i][j] = 0;
}

// --- Fonction pour placer un pion ---
int jouerCoup(int grille[LIGNES][COLONNES], int joueur, int colonne) {
    if (colonne < 0 || colonne >= COLONNES)
        return 0;

    for (int i = LIGNES - 1; i >= 0; i--) {
        if (grille[i][colonne] == 0) {
            grille[i][colonne] = joueur;
            return 1;
        }
    }
    return 0;
}

// --- Vérifie si un joueur a gagné ---
int verifierVictoire(int grille[LIGNES][COLONNES], int joueur) {
    // Horizontal
    for (int i = 0; i < LIGNES; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i][j + 1] == joueur &&
                grille[i][j + 2] == joueur && grille[i][j + 3] == joueur)
                return 1;

    // Vertical
    for (int i = 0; i < LIGNES - 3; i++)
        for (int j = 0; j < COLONNES; j++)
            if (grille[i][j] == joueur && grille[i + 1][j] == joueur &&
                grille[i + 2][j] == joueur && grille[i + 3][j] == joueur)
                return 1;

    // Diagonale /
    for (int i = 3; i < LIGNES; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i - 1][j + 1] == joueur &&
                grille[i - 2][j + 2] == joueur && grille[i - 3][j + 3] == joueur)
                return 1;

    // Diagonale \
    for (int i = 0; i < LIGNES - 3; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i + 1][j + 1] == joueur &&
                grille[i + 2][j + 2] == joueur && grille[i + 3][j + 3] == joueur)
                return 1;

    return 0;
}

// --- Dessine la grille et les pions ---
void afficherGrilleSDL(SDL_Renderer *renderer, int grille[LIGNES][COLONNES]) {
    // fond bleu
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // dessine la grille et les pions
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            SDL_Rect caseRect = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

            // contour des cases
            SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
            SDL_RenderDrawRect(renderer, &caseRect);

            // dessine le pion
            if (grille[i][j] != 0) {
                if (grille[i][j] == 1)
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // rouge
                else
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // jaune

                int margin = 10;
		int radius = (CELL_SIZE - 2 * margin) / 2;
		int centerX = j * CELL_SIZE + CELL_SIZE / 2;
		int centerY = i * CELL_SIZE + CELL_SIZE / 2;
		DrawCircle(renderer, centerX, centerY, radius);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main() {
    // --- Initialisation SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Puissance 4",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        COLONNES * CELL_SIZE, LIGNES * CELL_SIZE,
        SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Erreur creation fenetre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur creation renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int grille[LIGNES][COLONNES];
    initialiserGrille(grille);
    int joueur = 1;
    int enCours = 1;

    afficherGrilleSDL(renderer, grille);

    // --- Boucle principale ---
    SDL_Event e;
    while (enCours) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                enCours = 0;

            // Clic souris → placer un pion
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int colonne = e.button.x / CELL_SIZE;
                if (jouerCoup(grille, joueur, colonne)) {
                    afficherGrilleSDL(renderer, grille);
                    if (verifierVictoire(grille, joueur)) {
                        printf("🎉 Joueur %d a gagné !\n", joueur);
                        SDL_Delay(2000);
                        enCours = 0;
                    }
                    joueur = (joueur == 1) ? 2 : 1;
                }
            }
        }
    }

    // --- Nettoyage ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

