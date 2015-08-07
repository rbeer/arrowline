/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.h
 * v0.6.4 / 2015.07.28
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _SEGMENTS_h
#define _SEGMENTS_H

#define POSITION_LEFT 0     // points right
#define POSITION_RIGHT 1    // points left
#define POSITION_CENTER 2   // points left and right

/* signature of a segment generation function */
typedef int (*segment_generator)(char** dest, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);


/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* generate end separator with terminal color reset */
void al_segment_end(char** dest, unsigned int* maxlen, int bg, int orientation);

/* show current working dir */
int al_segment_cwd(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show current working dir prefix */
int al_segment_cwd_prefix(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show last exit status if command failed */
int al_segment_status(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show vcs status if cwd is part of a vcs */
int al_segment_vcs(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

#endif // _SEGMENTS_H
