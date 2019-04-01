/*	Game.h
	Includes functions for creating, moving, and drawing tetris pieces, as well as setting up the title, pause, and high score screens
	Part of Tetris clone
	Marcus Drab
	2/28/2019*/
	
#ifndef GAME_H
#define GAME_H

typedef enum piece_rotation {
	up,
	left,
	down,
	right

} PIECE_ROTATION;

typedef enum piece_type {
	I = 0x4,
	T = 0x2,
	O = 0x1,
	S = 0x3,
	Z = 0x6,
	J = 0x7,
	L = 0x5
} PIECE_TYPE;

char stats[7];

typedef struct tetris_piece {
	char x;
	char y;
	PIECE_TYPE type;
	PIECE_ROTATION r;
} TETRIS_PIECE;

typedef struct piece_grid{
	char a[7][7];
} PIECE_GRID;

char title[11][78];

char draw_piece(TETRIS_PIECE p);

void init_pause_screen(void);

void draw_next_piece(char x, char y, TETRIS_PIECE p);

void update_stats(PIECE_TYPE type);

void title_screen(void);

void init_stats(void);

TETRIS_PIECE rotate_piece(TETRIS_PIECE p);

TETRIS_PIECE move_piece_left(TETRIS_PIECE p);

TETRIS_PIECE move_piece_right(TETRIS_PIECE p);

TETRIS_PIECE move_piece_down(TETRIS_PIECE p);

TETRIS_PIECE gen_piece(PIECE_TYPE prev_type);

PIECE_TYPE gen_type(PIECE_TYPE prev_type);

PIECE_GRID get_piece_grid(TETRIS_PIECE p, unsigned char x_offset, unsigned char y_offset, char n);

char get_width(PIECE_GRID g);

char get_height(PIECE_GRID g);

void show_high_scores(char user_score, unsigned char score_index);

#endif
