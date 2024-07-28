#include "../include/core.h"


Renderer* renderer = NULL;
SDL_Texture* boardtex = NULL;
SDL_Texture* piecetex = NULL;
SDL_Event event;

Piece kw = { WHITE | KING, {0, 0, 333, 334} };
Piece kb = { BLACK | KING, {0, 334, 333, 334} };
Piece qw = {WHITE | QUEEN, {333, 0, 333, 334}};
Piece qb = {BLACK | QUEEN, {333, 334, 333, 334}};
Piece pw = {WHITE | PAWN, {1665, 0, 333, 334}};
Piece pb = {BLACK | PAWN, {1665, 334, 333, 334}};
Piece bw = {WHITE | BISHOP, {666, 0, 333, 334}};
Piece bb = {BLACK | BISHOP, {666, 334, 333, 334}};
Piece nw = {WHITE | KNIGHT, {999, 0, 333, 334}};
Piece nb = {BLACK | KNIGHT, {999, 334, 333, 334}};
Piece rw = {WHITE | ROOK, {1332, 0, 333, 334}};
Piece rb = { BLACK | ROOK, {1332, 334, 333, 334} };
Piece none = { NONE, {0, 0, 0, 0} };

SDL_Rect defaultRect = { 0, 0, 0, 0 };
Piece defaultPiece = { NONE, {0, 0, 0, 0} };

CellState cell[64] = { {NONE, {0,0,0,}}, {0,0,0,0} };
const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void LoadPositionFromFen(const char* fen, CellState *board) {
    static char str_copy[100];
    strcpy_s(str_copy, 100, fen);

    /*char* positions = strtok(str_copy, ' ');*/ // doesnt work
    int file = 0, rank = 7;

    for (int i = 0; i < strlen(fen); ++i) {
        char symbol = fen[i];
        if (symbol == ' ') {
            break;
        }
        
        if ( symbol == '/') {
            file = 0;
            rank--;
        }
        else {
            if (isdigit(symbol)) {
                file += (int)(symbol - '0'); // converting to int if it is a number
            }
            else {
                int index = file * 8 + rank;
                switch (symbol) {
                    case 'r':
                        board[index].piece = rb;
                        break;
                    case 'n':
                        board[index].piece = nb;
                        break;
                    case 'b':
                        board[index].piece = bb;
                        break;
                    case 'q':
                        board[index].piece = qb;
                        break;
                    case 'k':
                        board[index].piece = kb;
                        break;
                    case 'p':
                        board[index].piece = pb;
                        break;
                    case 'R':
                        board[index].piece = rw;
                        break;
                    case 'N':
                        board[index].piece = nw;
                        break;
                    case 'B':
                        board[index].piece = bw;
                        break;
                    case 'Q':
                        board[index].piece = qw;
                        break;
                    case 'K':
                        board[index].piece = kw;
                        break;
                    case 'P':
                        board[index].piece = pw;
                        break;
                    default:
                        board[index].piece = none;
                        break;
                }

                file++;

            }
        }
    }
}


bool initialize_game() {
	renderer = createRenderer("Chess", winsize.width, winsize.height, NULL, NULL, 0);
	if (renderer == NULL) {
		return false;
	}

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int index = i * 8 + j; // i*cols+j
            (cell[index]).dstRect.x = i * squareSize;
            (cell[index]).dstRect.y = j * squareSize;
            (cell[index]).dstRect.w = squareSize;
            (cell[index]).dstRect.h = squareSize;
        }
    }

    LoadPositionFromFen(startingFen, cell);

    boardtex = createboardTexture(renderer);
    piecetex = loadTexture(renderer, pieceImagePath);

    return true;
}

Scene run_game() {

    bool run = true;
    SDL_Color bgColor = { 55, 61, 99, 255 };
       
    while (run) {
        // event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) run = false;
        }

        //update


        //rendering
        setBackgroundColor(renderer, &bgColor);
        renderTextureEx(renderer, boardtex, 0, 0, (squareSize * 8), (squareSize * 8));
        
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int index = i * 8 + j;
                if (cell[index].piece.state != NONE) {
                    renderTextureEa(renderer, piecetex, &(cell[index].piece.srcRect), &(cell[index].dstRect));
                }
            }
        }

        displayWindow(renderer);
    }

    return QUIT;
}


Scene runMainMenu() {
    bool run = true;
    while (run) {

        // event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) run = false;
        }

        //update


        //rendering
        clearWindow(renderer);

        renderTextureEx(renderer, boardtex, 0, 0, (squareSize * 8), (squareSize * 8));

        displayWindow(renderer);
    }

    return QUIT;
}

void clean()
{
    destroyRenderer(renderer);
}
