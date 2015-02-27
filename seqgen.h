#ifndef SEQGEN_H_   /* Include guard */
#define SEQGEN_H_

typedef struct Seq_t Sequence;
struct seq_t{
	size_t size;
	unsigned char* seq;
};

Sequence getSequence(void); 

void freeSequence(void);

#endif // SEQGEN_H_