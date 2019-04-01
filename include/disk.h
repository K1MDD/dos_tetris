#ifndef DISK_H
#define DISK_H

typedef struct high_score {
	char initials[3];
	unsigned long int score;
}	HIGH_SCORE;

void load_scores(void);
int get_score_sheet_handle(void);
void save_scores(void);
HIGH_SCORE scores[10];

#endif
