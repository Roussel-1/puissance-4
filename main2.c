#include <SDL2/SDL.h>
#include <stdio.h>

#define LIGNES 6
#define COLONNES 7
#define CELL_SIZE 100

typedef struct {
    int x1, y1;     // début de la ligne de victoire
    int x2, y2;     // fin de la ligne
    int gagnant;    // joueur gagnant
    int victoire;   // 1 = victoire trouvée
} VictoireInfo;

// --- Dessine un cercle rempli ---
void DrawCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius)
                SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        }
    }
}

// --- Vérifie victoire et renvoie les coordonnées gagnantes ---
VictoireInfo verifierVictoire(int grille[LIGNES][COLONNES], int joueur) {
    VictoireInfo v = {0,0,0,0,0,0};

    // horizontale
    for (int i = 0; i < LIGNES; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i][j+1] == joueur &&
                grille[i][j+2] == joueur && grille[i][j+3] == joueur) {
                v.x1 = j; v.y1 = i; v.x2 = j+3; v.y2 = i;
                v.gagnant = joueur; v.victoire = 1;
                return v;
            }

    // verticale
    for (int i = 0; i < LIGNES - 3; i++)
        for (int j = 0; j < COLONNES; j++)
            if (grille[i][j] == joueur && grille[i+1][j] == joueur &&
                grille[i+2][j] == joueur && grille[i+3][j] == joueur) {
                v.x1 = j; v.y1 = i; v.x2 = j; v.y2 = i+3;
                v.gagnant = joueur; v.victoire = 1;
                return v;
            }

    // diagonale descendante ↘
    for (int i = 0; i < LIGNES - 3; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i+1][j+1] == joueur &&
                grille[i+2][j+2] == joueur && grille[i+3][j+3] == joueur) {
                v.x1 = j; v.y1 = i; v.x2 = j+3; v.y2 = i+3;
                v.gagnant = joueur; v.victoire = 1;
                return v;
            }

    // diagonale montante ↗
    for (int i = 3; i < LIGNES; i++)
        for (int j = 0; j < COLONNES - 3; j++)
            if (grille[i][j] == joueur && grille[i-1][j+1] == joueur &&
                grille[i-2][j+2] == joueur && grille[i-3][j+3] == joueur) {
                v.x1 = j; v.y1 = i; v.x2 = j+3; v.y2 = i-3;
                v.gagnant = joueur; v.victoire = 1;
                return v;
            }

    return v;
}

// --- Joue un coup dans une colonne ---
int jouerCoup(int grille[LIGNES][COLONNES], int joueur, int colonne) {
    if (colonne < 0 || colonne >= COLONNES) return 0;
    for (int i = LIGNES - 1; i >= 0; i--) {
        if (grille[i][colonne] == 0) {
            grille[i][colonne] = joueur;
            return 1;
        }
    }
    return 0;
}

// --- Affiche la grille SDL ---
void afficherGrilleSDL(SDL_Renderer *renderer, int grille[LIGNES][COLONNES], VictoireInfo v) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // fond blanc
    SDL_RenderClear(renderer);

    int margin = 10;

    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            SDL_Rect caseRect = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };

            // contour gris
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawRect(renderer, &caseRect);

            // pion
            if (grille[i][j] != 0) {
                if (grille[i][j] == 1)
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // rouge
                else
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // jaune

                int radius = (CELL_SIZE - 2 * margin) / 2;
                int centerX = j * CELL_SIZE + CELL_SIZE / 2;
                int centerY = i * CELL_SIZE + CELL_SIZE / 2;
                DrawCircle(renderer, centerX, centerY, radius);
            }
        }
    }

    // --- Si victoire, dessine une ligne verte ---
    if (v.victoire) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        int x1 = v.x1 * CELL_SIZE + CELL_SIZE / 2;
        int y1 = v.y1 * CELL_SIZE + CELL_SIZE / 2;
        int x2 = v.x2 * CELL_SIZE + CELL_SIZE / 2;
        int y2 = v.y2 * CELL_SIZE + CELL_SIZE / 2;
    	int thickness = 10; // épaisseur de la ligne (en pixels)
	for (int offset = -thickness/2; offset <= thickness/2; offset++) {
    		SDL_RenderDrawLine(renderer, x1 - offset, y1 + offset, x2 - offset, y2 + offset);
	}
}

    SDL_RenderPresent(renderer);
}

// --- Programme principal ---
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Puissance 4 SDL2",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          COLONNES * CELL_SIZE, LIGNES * CELL_SIZE, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int grille[LIGNES][COLONNES] = {0};
    int joueur = 1;
    int enCours = 1;
    VictoireInfo victoire = {0,0,0,0,0,0};

    afficherGrilleSDL(renderer, grille, victoire);

    SDL_Event e;
    while (enCours) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                enCours = 0;

            // clic souris
            if (!victoire.victoire && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int colonne = e.button.x / CELL_SIZE;
                if (jouerCoup(grille, joueur, colonne)) {
                    victoire = verifierVictoire(grille, joueur);
                    afficherGrilleSDL(renderer, grille, victoire);
                    if (victoire.victoire) {
                        printf("🎉 Joueur %d a gagné !\n", joueur);
                    } else {
                        joueur = (joueur == 1) ? 2 : 1;
                    }
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
