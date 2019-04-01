#include "..\include\screen.h"
#include "..\include\input.h"
#include "..\include\game.h"
#include "..\include\timer.h"
#include "..\include\disk.h"
//#include <stdio.h>

#define FRAME_OFFSET_X 6
#define FRAME_OFFSET_Y 2
#define NEW_PIECE_X 12
#define NEW_PIECE_Y 6

int main(void)
{
	char speed[15] = {15, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 2, 1};
	char types[7] = {I, T, O, S, Z, J, L};
	unsigned char level = 1;
	char lines = 0;
	char down_keys = 0;
	unsigned long int score = 0;
	unsigned long int high_score = 10000;
	unsigned char i, j, k, l;
	char game_over = 0;
	char prev_y;
	//COLOR clr;
	TETRIS_PIECE current_piece;
	TETRIS_PIECE next_piece;
	PIECE_GRID current_piece_grid;
	//BOX_COORDINATES b;
	
	
	char game_matrix[20] = {		//population array
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
	
	init_kbd();
	init_screen();
	init_timer();
	init_pause_screen();
	title_screen();
	init_stats();
	load_scores();
	if(high_score < scores[0].score){
		high_score = scores[0].score;
	}
	update_stats(0);
	current_piece.r = up;
	current_piece.x = 18;
	for(i = 0; i < 7; i++){
		current_piece.type = types[i];
		current_piece.y = 3 * i + 2;
		draw_piece(current_piece);
	}
	
	current_piece = gen_piece(gen_type(S));
	next_piece = gen_piece(current_piece.type);
	
	draw_frame(FRAME_OFFSET_X, FRAME_OFFSET_Y, 30, 20, 0);
	draw_frame(41, 7, 14, 3, 0);
	draw_frame(41, 3, 14, 2, 0);
	draw_frame(41, 12, 14, 2, 0);
	draw_frame(41, 16, 14, 5, 0);
	write_string(7, 43, "Next Piece:", 0, white);
	write_string(3, 45, "Lines:", 0, white);
	write_num(4, 45, lines, 0, white);
	write_string(12, 45, "Level:", 0, white);
	write_num(13, 45, level, 0, white);
	write_string(16, 43, "High Score:", 0, white);
	write_num(17, 45, high_score, 0, white);
	write_string(19, 45, "Score:", 0, white);
	write_num(20, 45, score, 0, white);
	draw_next_piece(NEW_PIECE_X, NEW_PIECE_Y, next_piece);
	draw_piece(current_piece);
	update_stats(current_piece.type);

	while(key != 'q' && !game_over){
		switch(key){
			case 'p':
				key = 0;
				i = clock_ticks;
				change_page(1);
				while(key != 'p' && key != 'P');
				key = 0;
				change_page(0);
				clock_ticks = i;
				break;
			case ' ':
				key = 0;
				current_piece = rotate_piece(current_piece);
				break;
			case 'a':
				key = 0;
				current_piece = move_piece_left(current_piece);
				break;
			case 's':
				key = 0;
				prev_y = current_piece.y;
				current_piece = move_piece_down(current_piece);
				if(current_piece.y != prev_y){
					down_keys++;
				}else{
					score += down_keys;
					down_keys = 0;
					write_num(20, 45, score, 0, white);
				}
				break;
			case 'd':
				key = 0;
				current_piece = move_piece_right(current_piece);
				break;
			default:
				break;
		}
		if(clock_ticks >= speed[level - 1]){
			prev_y = current_piece.y;
			
			current_piece = move_piece_down(current_piece);
			if(current_piece.y == prev_y){
				score += down_keys;
				down_keys = 0;
				write_num(20, 45, score, 0, white);
				current_piece_grid = get_piece_grid(current_piece, 0, 0, 1);
				for(i = 0; i < get_height(current_piece_grid); i++){
					for(j = 0; j < get_width(current_piece_grid); j++){
						game_matrix[current_piece.y + i] += (current_piece_grid.a[i][j]) ? 1 : 0;
					}
				}
				l = 0;
				for(i = 0; i < 20; i++){
					if(game_matrix[i] == 10){
						clock_ticks = 0;
						del_row(i);	
						while(clock_ticks < 9);
						lines++;
						l++;
						if(lines > 0 && lines % 10 == 0){
							if(level != 15){
								level++;
								write_num(13, 45, level, 0, white);
							}
						}
						write_num(4, 45, lines, 0, white);
						scroll_game_down(i);
						for(j = i; j > 0; j--){
							game_matrix[j] = game_matrix[j - 1];
							/*for(k = 0; k < 10; k++){
								b.x = k;
								b.y = j - 1;
								clr = get_box_color(b);
								if(check_box(b) == 0x20){
									b.y++;
									del_box(b);
								}else{
									b.y++;
									draw_box(b, clr);
								}
							}*/
						}
						
						//del_row(0);
					}
				}
				switch(l){
					case 1:
					score += 40 * level;
					write_num(20, 45, score, 0, white);
					break;
					case 2:
					score += 100 * level;
					write_num(20, 45, score, 0, white);
					break;
					case 3:
					score += 300 * level;
					write_num(20, 45, score, 0, white);
					break;
					case 4:
					score += 1200 * level;
					write_num(20, 45, score, 0, white);
					break;
					default:
					break;
				}
				if(score > high_score){
					high_score = score;
					write_num(17, 45, high_score, 0, white);
				}
				current_piece = next_piece;
				next_piece = gen_piece(current_piece.type);
				update_stats(current_piece.type);
				game_over = draw_piece(current_piece);
				draw_next_piece(NEW_PIECE_X, NEW_PIECE_Y, next_piece);
			}
			clock_ticks = 0;
		}
	}
	j = 0;
	k = 0;
	for(i = 10; i > 0; i--){
		if(scores[i-1].score < score){
			if(i < 10){
				scores[i] = scores[i - 1];
			}
			scores[i - 1].score = score;
			scores[i - 1].initials[0] = '-';
			scores[i - 1].initials[1] = '-';
			scores[i - 1].initials[2] = '-';
			j = 1;
			k = i - 1;
		}
	}
	show_high_scores(j, k);
	save_scores();
	return_kbd();
	return_timer();
	return_screen();
	return (0);
}
