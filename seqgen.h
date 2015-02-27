#ifndef SEQGEN_H_
#define SEQGEN_H_

typedef struct Sequence_t Sequence;
struct Sequence_t{
	size_t size;
	unsigned char* seq;
};

void freeSequence(void);

#endif // SEQGEN_H_