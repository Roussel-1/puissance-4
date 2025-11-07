#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define CELL_SIZE 100
#define COLONNES 7
#define LIGNES 6
#define SCORE_WIDTH 200 // largeur pour afficher le score

typedef struct {
    bool victoire;
    int joueur;
} Victoire;

// Fonction pour afficher du texte
void afficherTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, SDL_Color couleur) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, couleur);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// === Menu de sélection du nombre de manches ===
int menuChoixManches(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event e;
    bool enMenu = true;
    int nbManches = 1; // valeur initiale
    SDL_Color noir = {0, 0, 0};
    SDL_Color bleu = {0, 120, 255};
    SDL_Color gris = {200, 200, 200};

    SDL_Rect btn1 = {150, 250, 80, 60};
    SDL_Rect btn3 = {280, 250, 80, 60};
    SDL_Rect btn5 = {410, 250, 80, 60};

    while (enMenu) {
        SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
        SDL_RenderClear(renderer);

        afficherTexte(renderer, font, "Choisissez le nombre de manches :", 100, 150, noir);

        // Boutons
        SDL_SetRenderDrawColor(renderer, gris.r, gris.g, gris.b, 255);
        SDL_RenderFillRect(renderer, &btn1);
        SDL_RenderFillRect(renderer, &btn3);
        SDL_RenderFillRect(renderer, &btn5);

        SDL_SetRenderDrawColor(renderer, noir.r, noir.g, noir.b, 255);
        SDL_RenderDrawRect(renderer, &btn1);
        SDL_RenderDrawRect(renderer, &btn3);
        SDL_RenderDrawRect(renderer, &btn5);

        afficherTexte(renderer, font, "1", btn1.x + 30, btn1.y + 15, bleu);
        afficherTexte(renderer, font, "3", btn3.x + 30, btn3.y + 15, bleu);
        afficherTexte(renderer, font, "5", btn5.x + 30, btn5.y + 15, bleu);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return 0;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= btn1.x && x <= btn1.x + btn1.w &&
                    y >= btn1.y && y <= btn1.y + btn1.h) {
                    nbManches = 1; enMenu = false;
                }
                else if (x >= btn3.x && x <= btn3.x + btn3.w &&
                         y >= btn3.y && y <= btn3.y + btn3.h) {
                    nbManches = 3; enMenu = false;
                }
                else if (x >= btn5.x && x <= btn5.x + btn5.w &&
                         y >= btn5.y && y <= btn5.y + btn5.h) {
                    nbManches = 5; enMenu = false;
                }
            }
        }
        SDL_Delay(20);
    }
    return nbManches;
}

// Affichage de la grille + score
void afficherGrille(SDL_Renderer *renderer, TTF_Font *font, int scoreJ1, int scoreJ2, int manche, int total) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Grille
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            SDL_Rect cell = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    // Zone de score
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect scoreRect = {COLONNES * CELL_SIZE, 0, SCORE_WIDTH, LIGNES * CELL_SIZE};
    SDL_RenderFillRect(renderer, &scoreRect);

    SDL_Color noir = {0, 0, 0};
    char buffer[128];

    snprintf(buffer, sizeof(buffer), "Score J1 : %d", scoreJ1);
    afficherTexte(renderer, font, buffer, COLONNES * CELL_SIZE + 10, 50, noir);
    snprintf(buffer, sizeof(buffer), "Score J2 : %d", scoreJ2);
    afficherTexte(renderer, font, buffer, COLONNES * CELL_SIZE + 10, 100, noir);

    snprintf(buffer, sizeof(buffer), "Manche : %d / %d", manche, total);
    afficherTexte(renderer, font, buffer, COLONNES * CELL_SIZE + 10, 20, noir);

    SDL_RenderPresent(renderer);
}

// === Main simplifié ===
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Puissance 4 - Manche & Score",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        COLONNES * CELL_SIZE + SCORE_WIDTH, LIGNES * CELL_SIZE, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 20);
    if (!font) {
        printf("Erreur police : %s\n", TTF_GetError());
        return 1;
    }

    int nbManches = menuChoixManches(renderer, font);
    if (nbManches == 0) return 0;

    int scoreJ1 = 0, scoreJ2 = 0;
    int manche = 1;
    bool enCours = true;

    while (manche <= nbManches && enCours) {
        afficherGrille(renderer, font, scoreJ1, scoreJ2, manche, nbManches);
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) enCours = false;
        }

        // Simulation d'une manche gagnée aléatoirement
        SDL_Delay(1000); // pause pour visualiser
        if (manche % 2 == 1) scoreJ1++; else scoreJ2++; // juste pour test
        manche++;
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
