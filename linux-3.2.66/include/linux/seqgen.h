/*
 * seqgen_repeater.h
 * ==================================================
 * Valkenberg Benoit, Boileau Quentin
 *
 */

#ifndef SEQGEN_H_
#define SEQGEN_H_

typedef struct Sequence_t* Sequence;
struct Sequence_t{
	size_t size;
	unsigned char* seq;
};

extern Sequence sequence;

/*
 * Free kernel memory
 */
void freeSequence(void);

#endif // SEQGEN_H_
