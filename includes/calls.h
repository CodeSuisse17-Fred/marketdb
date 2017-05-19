#ifndef __H_CALLS
#define __H_CALLS

#define F_LOW 1
#define F_HIGH 2
#define F_GRPH 4
#define F_PRED 8
#define F_MOVE 16
#define F_PRICE 32

struct query {
	struct query *next;
	int f_type;
	struct s_constr *c;
};

struct s_date {
	int m, d, y;
};

struct s_time{
	int h, m, s;
};

struct s_timedate{
	struct s_date *d;
	struct s_time *t;
};

struct s_range {
	struct s_timedate *start, *end;
};

struct s_constr {
	struct s_constr *next;
	char *f, *v;
	int isrange;
	struct s_range *r;
	struct s_timedate *td;
};

int process(struct query *);

#endif
