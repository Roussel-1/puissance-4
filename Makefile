# === Makefile pour le projet SDL Puissance 4 ===

# Nom de l'exécutable
EXEC = jeu_graphique

# Fichier source
SRC = jeu_graphique.c

# Commande de compilation
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_ttf

# === Règle par défaut : compilation complète ===
all: $(EXEC)

$(EXEC): $(SRC)
	@echo "🔧 Compilation de $(SRC)..."
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LIBS)
	@echo "✅ Compilation terminée !"

# === Exécuter le programme ===
run: $(EXEC)
	@echo "🚀 Lancement du jeu SDL..."
	./$(EXEC)

# === Nettoyer les fichiers compilés ===
clean:
	@echo "🧹 Suppression de l'exécutable..."
	rm -f $(EXEC)
	@echo "✅ Nettoyage terminé !"
