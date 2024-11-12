#include <stdio.h>
#include <stdlib.h>  // for rand()
#include "markovchain.h"

#define NUM_NOTES 12   // 12 notes (C~B)
#define NUM_OCTAVES 3  // 3 octaves (0, 1, 2)
#define NUM_LENGTHS 3  // 1/4, 1/8, 1/16


const char *notes[NUM_NOTES] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

const char *lengths[NUM_LENGTHS] = {"1/4", "1/8", "1/16"};

// main note markov Chain(12x12)
float note_transition[NUM_NOTES][NUM_NOTES] = {
    {0.2, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0},
    {0.3, 0.1, 0.2, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0}
};

// octave Markov Chain (3x3)
float octave_transition[NUM_OCTAVES][NUM_OCTAVES] = {
    {0.5, 0.3, 0.2},
    {0.3, 0.4, 0.3},
    {0.2, 0.3, 0.5}
};

// note length transition probability (3x3)
float length_transition[NUM_LENGTHS][NUM_LENGTHS] = {
    {0.6, 0.3, 0.1},  // 1/4 -> 1/4, 1/8, 1/16
    {0.2, 0.5, 0.3},  // 1/8 -> 1/4, 1/8, 1/16
    {0.1, 0.2, 0.7}   // 1/16 -> 1/4, 1/8, 1/16
};

// accompany note probability (3, 5, 7)
float harmony_prob[3] = {0.5, 0.3, 0.2};

// select next note
int select_next(float transition[][NUM_NOTES], int current, int num_states)
{
    float r = (float)rand() / RAND_MAX;
    float cumulative = 0.0;

    for (int i = 0; i < num_states; i++) {
        cumulative += transition[current][i];
        if (r < cumulative) {
            return i;
        }
    }
    return num_states - 1; // default (final state)
}

// select next harmony note
int select_harmony(int tonic)
{
    float r = (float)rand() / RAND_MAX;
    float cumulative = 0.0;

    for (int i = 0; i < 3; i++) {
        cumulative += harmony_prob[i];
        if (r < cumulative) {
            return (tonic + (i + 2) * 2) % NUM_NOTES; // 3도, 5도, 7도 선택
        }
    }
    return (tonic + 4) % NUM_NOTES; // default(5도)
}

int mc_test(){

	int current_note = 0;   // starting note (C)
	int current_octave = 1; // starting octave
	int current_length = 0; // starting length (1/4)

	for (int i = 0; i < 10; i++) {
		// main note
		current_note = select_next(note_transition, current_note, NUM_NOTES);
		current_octave = select_next(octave_transition, current_octave, NUM_OCTAVES);

		// two accompanying note
		int harmony1 = select_harmony(current_note);
		int harmony2 = select_harmony(current_note);

		// note length
		current_length = select_next(length_transition, current_length, NUM_LENGTHS);

	printf("Chord: %s%d - %s%d - %s%d, Length: %s\n",
			notes[current_note], current_octave,
			notes[harmony1], current_octave,
			notes[harmony2], current_octave,
			lengths[current_length]);
	}

	return 0;
}


