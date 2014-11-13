#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>

/*
 typedef struct {
 jmp_buf caller_jmp;
 jmp_buf gen_jmp;
 bool continue;
 } Generator;
 */

//based on https://www.securecoding.cert.org/confluence/display/seccode/MSC22-C.+Use+the+setjmp%28%29,+longjmp%28%29+facility+securely
/*this implementation support only one instance of generator at same time so you can't call generator on multiple places or on yield it jumps
 * back to first caller
 */
typedef struct {
	jmp_buf * callerFn;
	jmp_buf * genFn;
} iterator;

//store func##_gen_jmp , save ret and jump back to caller
//when longjump(func##_gen_jmp,1) continue from there
#define YIELD(fn, n)\
  if (! setjmp(*fn##_iter.genFn)) {  \
    fn##_ret = n;                \
	longjmp(*fn##_iter.callerFn, 1); \
  }

//this implementation work with calling on multiple places
//[TODO] find out how to reset jmp_buf without realocation (its performance hit) (but 100% working)
#define GENERATOR(ret, fn, argt, argv, stopIterationVal)\
  static iterator fn##_iter;                            \
  static ret fn##_ret;                                  \
  	                                                    \
  void fn##__real(argt argv);                           \
  ret fn(argt argv) {                                   \
  	if (fn##_iter.callerFn) {                           \
  		free(fn##_iter.callerFn);                       \
  	}                                                   \
  	fn##_iter.callerFn = calloc(1, sizeof(jmp_buf));    \
  	if (setjmp(*fn##_iter.callerFn))                    \
  		return fn##_ret;                                \
  	if (!fn##_iter.genFn) {                             \
  		fn##_iter.genFn = calloc(1, sizeof(jmp_buf));   \
  		fn##__real(argv);                               \
  		return fn##_ret;                                \
  	} else {                                            \
  		longjmp(*fn##_iter.genFn, 1);                   \
  		return fn##_ret;                                \
  	}                                                   \
  }                                                     \
  void fn##__real(argt argv)\
/*
#define GENERATOR(ret, func, argt, argv, stopIterationVal)\
  static jmp_buf func##_caller_jmp;    \
  static jmp_buf func##_gen_jmp;       \
  static bool func##_continue=false;   \
  static ret func##_ret;               \
									   \
  void func##__real(argt argv);        \
  ret func(argt argv){                 \
    if (!func##_continue){             \
      func##_continue=true; 		   \
      if (!setjmp(func##_caller_jmp)) {\
        func##__real(argv);            \
      }else{                           \
        return func##_ret;             \
      }                                \
    }else {                            \
      longjmp(func##_gen_jmp,1);       \
    }							       \
	return stopIterationVal;           \
  }                                    \
                                       \
  void func##__real(argt argv)
*/
