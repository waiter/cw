#ifndef __GLOBLE_DEFINE_H__
#define __GLOBLE_DEFINE_H__

#define INIT_RAND()				srand((unsigned)time(NULL))
#define RAND_INT_0_B1(__I__)	(int)(1.0f * (int)__I__ * rand() / (RAND_MAX + 1.0f))

#endif //__GLOBLE_DEFINE_H__