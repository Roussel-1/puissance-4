#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define CELL_SIZE 100
#define COLONNES 7
#define LIGNES 6

typedef struct {
    bool victoire;
    int joueur;
} Victoire;

// === FONCTIONS DE BASE ===

void reinitialiserGrille(int grille[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++)
        for (int j = 0; j < COLONNES; j++)
            grille[i][j] = 0;
}

void afficherGrilleSDL(SDL_Renderer *renderer, int grille[LIGNES][COLONNES], Victoire victoire) {
    // Fond blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Grille bleue + pions
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            SDL_Rect cell = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            SDL_RenderDrawRect(renderer, &cell);
            if (grille[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &cell);
            } else if (grille[i][j] == 2) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &cell);
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
    }
    SDL_RenderPresent(renderer);
}

bool jouerCoup(int grille[LIGNES][COLONNES], int joueur, int colonne) {
    if (colonne < 0 || colonne >= COLONNES) return false;
    for (int i = LIGNES - 1; i >= 0; i--) {
        if (grille[i][colonne] == 0) {
            grille[i][colonne] = joueur;
            return true;
        }
    }
    return false;
}

// === À adapter avec ta logique de victoire ===
Victoire verifierVictoire(int grille[LIGNES][COLONNES], int joueur) {
    Victoire v = {false, joueur};
    // (simplifié ici, à remplacer par ta logique réelle)
    return v;
}

// === MENU GRAPHIQUE DE CHOIX ===
int afficherMenu(SDL_Renderer *renderer) {
    SDL_Event e;
    int choix = 0;
    bool enMenu = true;

    SDL_Rect boutons[3];
    const int nbChoix[3] = {1, 3, 5};
    int boutonW = 200, boutonH = 80;
    int espacement = 50;
    int startY = 150;

    for (int i = 0; i < 3; i++) {
        boutons[i].x = (COLONNES * CELL_SIZE - boutonW) / 2;
        boutons[i].y = startY + i * (boutonH + espacement);
        boutons[i].w = boutonW;
        boutons[i].h = boutonH;
    }

    while (enMenu) {
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);

        // Titre
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect titreRect = { (COLONNES * CELL_SIZE) / 2 - 150, 50, 300, 50 };
        SDL_RenderDrawRect(renderer, &titreRect);

        // Dessiner boutons
        for (int i = 0; i < 3; i++) {
            SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
            SDL_RenderFillRect(renderer, &boutons[i]);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &boutons[i]);
        }
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x;
                int y = e.button.y;
                for (int i = 0; i < 3; i++) {
                    if (x >= boutons[i].x && x <= boutons[i].x + boutons[i].w &&
                        y >= boutons[i].y && y <= boutons[i].y + boutons[i].h) {
                        choix = nbChoix[i];
                        enMenu = false;
                    }
                }
            }
        }
    }
    return choix;
}

// === PROGRAMME PRINCIPAL ===
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Puissance 4 - Multi-Manches",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        COLONNES * CELL_SIZE, LIGNES * CELL_SIZE, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int grille[LIGNES][COLONNES] = {0};
    bool enCours = true;
    int joueur = 1;
    Victoire victoire = {false, 0};

    // === Menu de sélection du nombre de parties ===
    int nbParties = afficherMenu(renderer);
    if (nbParties == 0) {
        SDL_Quit();
        return 0;
    }

    printf("Nombre de parties sélectionné : %d\n", nbParties);

    int scoreJ1 = 0, scoreJ2 = 0;
    int mancheActuelle = 1;

    while (mancheActuelle <= nbParties && enCours) {
        reinitialiserGrille(grille);
        victoire.victoire = false;
        afficherGrilleSDL(renderer, grille, victoire);
        printf("=== Début de la manche %d ===\n", mancheActuelle);

        while (!victoire.victoire && enCours) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    enCours = false;
                    break;
                }

                if (!victoire.victoire && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int colonne = e.button.x / CELL_SIZE;
                    if (jouerCoup(grille, joueur, colonne)) {
                        victoire = verifierVictoire(grille, joueur);
                        afficherGrilleSDL(renderer, grille, victoire);
                        if (victoire.victoire) {
                            printf("🎉 Joueur %d gagne la manche %d !\n", joueur, mancheActuelle);
                            if (joueur == 1) scoreJ1++; else scoreJ2++;

                            int majorite = (nbParties / 2) + 1;
                            if (scoreJ1 >= majorite || scoreJ2 >= majorite) {
                                enCours = false;
                            }
                            mancheActuelle++;
                            SDL_Delay(1000);
                        } else {
                            joueur = (joueur == 1) ? 2 : 1;
                        }
                    }
                }
            }
        }
    }

    // === Fin du jeu ===
    if (scoreJ1 > scoreJ2)
        printf("\n🏆 Joueur 1 remporte le jeu (%d-%d)\n", scoreJ1, scoreJ2);
    else if (scoreJ2 > scoreJ1)
        printf("\n🏆 Joueur 2 remporte le jeu (%d-%d)\n", scoreJ2, scoreJ1);
    else
        printf("\n🤝 Match nul (%d-%d)\n", scoreJ1, scoreJ2);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
