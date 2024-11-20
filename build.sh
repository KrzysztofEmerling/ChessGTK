gcc -o Chess src/main.c src/app/loop.c src/app/theme.c src/chess/gameLoop.c src/chess/FEN_Parser.c `pkg-config --cflags --libs gtk4`
