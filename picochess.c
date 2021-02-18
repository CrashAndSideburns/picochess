#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

enum MoveError {
	ILLEGALSTART,
	ILLEGALEND,
	HANGSKING,
	LEGAL
};

enum PieceType {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

enum Colour {
	WHITE,
	BLACK,
	EMPTYSQUARE
};

struct Board {
	uint64_t white_pieces;
	uint64_t black_pieces;
	uint64_t pawns;
	uint64_t rooks;
	uint64_t knights;
	uint64_t bishops;
	uint64_t queens;
	uint64_t kings;
};

enum MoveError move_piece(struct Board *board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool valid_pawn_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool valid_rook_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool valid_knight_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool valid_bishop_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool valid_queen_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool valid_king_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square);
bool check(struct Board board, enum Colour colour);
enum PieceType get_piece_type(struct Board board, uint64_t square);
enum Colour get_colour(struct Board board, uint64_t square);

char square_char(struct Board board, uint64_t square);
struct Board board_init(void);
void print_board(struct Board board);

/* Return the type of the piece on a given square on some board. Square must be occupied. */
enum PieceType get_piece_type(struct Board board, uint64_t square)
{
	if (square & board.pawns) {
		return PAWN;
	} else if (square & board.rooks) {
		return ROOK;
	} else if (square & board.knights) {
		return KNIGHT;
	} else if (square & board.bishops) {
		return BISHOP;
	} else if (square & board.queens) {
		return QUEEN;
	} else {
		return KING;
	}
}

/* Return the colour of the piece on a given square on some board, or EMPTYSQUARE if not occupied. */
enum Colour get_colour(struct Board board, uint64_t square)
{
	if (square & board.white_pieces) {
		return WHITE;
	} else if (square & board.black_pieces) {
		return BLACK;
	} else {
		return EMPTYSQUARE;
	}
}

/* Return true if the specified pawn move is valid, false otherwise. */
bool valid_pawn_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	return true;
}

/* Return true if the specified rook move is valid, false otherwise. */
bool valid_rook_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	return true;
}

/* Return true if the specified knight move is valid, false otherwise. */
bool valid_knight_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	return true;
}

/* Return true if the specified bishop move is valid, false otherwise. */
bool valid_bishop_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	return true;
}

/* Return true if the specified queen move is valid, false otherwise. */
bool valid_queen_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	return true;
}

/* Return true if the specified king move is valid, false otherwise. */
bool valid_king_move(struct Board board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	return true;
}

/* Return true if the king of the specified colour is in check, false otherwise. */
bool check(struct Board board, enum Colour colour)
{
	return false;
}

/*
 * move_piece is essentially the main move-making function for picochess. It takes a pointer to a board, the
 * colour of the side playing the move, and the start and end squares, formatted as bitboards with a single
 * 1 bit, marking the positions.
 *
 * Returns a MoveError, LEGAL in the event of the move being legal, and a more descriptive error code
 * otherwise. As a side effect, modifies the board to make the move requested of it, but only if it returns
 * LEGAL. No side effect in event of error.
 */
enum MoveError move_piece(struct Board *board, enum Colour colour, uint64_t start_square, uint64_t end_square)
{
	struct Board tentative_board = *board;

	if (get_colour(*board, start_square) != colour) {
		return ILLEGALSTART;
	}

	// Is there some inefficiency here? Yep. Is it significant enough that I can summon the mental
	// capacity to eliminate it? Nope.
	
	// First, we clear the destination square of the piece, essentially capturing before we actually
	// process the move. This doesn't wind up mattering since we aren't actually modifiying the board.
	tentative_board.white_pieces &= ~end_square;
	tentative_board.black_pieces &= ~end_square;
	tentative_board.pawns &= ~end_square;
	tentative_board.rooks &= ~end_square;
	tentative_board.knights &= ~end_square;
	tentative_board.bishops &= ~end_square;
	tentative_board.queens &= ~end_square;
	tentative_board.kings &= ~end_square;

	// Next, we begin processing the move, removing the piece from part of the board, then putting it in
	// its final position.
	switch (colour) {
		case WHITE:
			tentative_board.white_pieces &= ~start_square;
			tentative_board.white_pieces |= end_square;
			break;
		case BLACK:
			tentative_board.black_pieces &= ~start_square;
			tentative_board.black_pieces |= end_square;
			break;
	}

	// Here we simply finish processing the move, doing the same motion as earlier, but on the piece-specific
	// bitboard. This is the last step in making the move, and once this is done tentative_board should reflect
	// what the final position of our board would be should the move be made on the board.
	switch (get_piece_type(*board, start_square)) {
		case PAWN:
			if (!(valid_pawn_move(*board, colour, start_square, end_square))) {
				return ILLEGALEND;
			}
			tentative_board.pawns &= ~start_square;
			tentative_board.pawns |= end_square;
			break;
		case ROOK:
			if (!(valid_rook_move(*board, colour, start_square, end_square))) {
				return ILLEGALEND;
			}
			tentative_board.rooks &= ~start_square;
			tentative_board.rooks |= end_square;
			break;
		case KNIGHT:
                        if (!(valid_knight_move(*board, colour, start_square, end_square))) {
				return ILLEGALEND;
			}
			tentative_board.knights &= ~start_square;
			tentative_board.knights |= end_square;
			break;
		case BISHOP:
			if (!(valid_bishop_move(*board, colour, start_square, end_square))) {
				return ILLEGALEND;
			}
			tentative_board.bishops &= ~start_square;
			tentative_board.bishops |= end_square;
			break;
		case QUEEN:
			if (!(valid_queen_move(*board, colour, start_square, end_square))) {
				return ILLEGALEND;
			}
			tentative_board.queens &= ~start_square;
			tentative_board.queens |= end_square;
			break;
		case KING:
			if (!(valid_king_move(*board, colour, start_square, end_square))) {
				return ILLEGALEND;
			}
			tentative_board.kings &= ~start_square;
			tentative_board.kings |= end_square;
			break;
	}

	// Technically, the valid_move functions only check that the moves are pseudolegal. This is
	// the final step to verify that they are actually legal.
	if (check(tentative_board, colour)) {
		return HANGSKING;
	} else {
		*board = tentative_board;
		return LEGAL;
	}
}

/* Return a char representation of the piece at a given square, or a space if empty. */
char square_char(struct Board board, uint64_t square)
{
	switch(get_colour(board, square)) {
		case WHITE:
			switch(get_piece_type(board, square)) {
				case PAWN:
					return 'P';
				case ROOK:
					return 'R';
				case KNIGHT:
					return 'N';
				case BISHOP:
					return 'B';
				case QUEEN:
					return 'Q';
				case KING:
					return 'K';
			}
		case BLACK:
			switch(get_piece_type(board, square)) {
				case PAWN:
					return 'p';
				case ROOK:
					return 'r';
				case KNIGHT:
					return 'n';
				case BISHOP:
					return 'b';
				case QUEEN:
					return 'q';
				case KING:
					return 'k';
			}
		case EMPTYSQUARE:
			return ' ';
	}
}

/* Print a simple text representation of a board. */
void print_board(struct Board board)
{
	for (int row = 8; row >= 1; row--) {
		for (int col = 1; col <= 8; col++) {
			putchar(square_char(board, (uint64_t)0x1 << (col - 1 + 8 * (row - 1))));
		}
		putchar('\n');
	}
}

/* Return a board set up for normal play. */
struct Board board_init(void)
{
	struct Board board;

	// Magic numbers to instantiate board.
	board.white_pieces = 0x000000000000ffff;
	board.black_pieces = 0xffff000000000000;
	board.pawns        = 0x00ff00000000ff00;
	board.rooks        = 0x8100000000000081;
	board.knights      = 0x4200000000000042;
	board.bishops      = 0x2400000000000024;
	board.queens       = 0x0800000000000008;
	board.kings        = 0x1000000000000010;

	return board;
}

int main(void)
{
	struct Board board = board_init();
	print_board(board);
	return 0;
}
