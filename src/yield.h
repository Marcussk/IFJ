#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>


//based on https://www.securecoding.cert.org/confluence/display/seccode/MSC22-C.+Use+the+setjmp%28%29,+longjmp%28%29+facility+securely

/*this implementation support only one instance of generator at same time so you can't call generator on multiple places or on yield it jumps
 * back to first caller
 */
#define YIELD(func, n) if (! setjmp(func##_gen_jmp)) {  \
    func##_ret = n;                                     \
	longjmp(func##_caller_jmp, 1);                      \
  }

#define GENERATOR(ret, func, argt, argv, stopIterationVal)\
  static jmp_buf func##_caller_jmp;             \
  static jmp_buf func##_gen_jmp;                \
  static bool func##_continue=false;            \
  static ret func##_ret;                        \
  	  	  	  	  	  	  	  	  	  	  	  	\
  void func##__real(argt argv);                 \
  ret func(argt argv){                          \
    if (!func##_continue){                      \
      func##_continue=true; 					\
      if (!setjmp(func##_caller_jmp)) {         \
        func##__real(argv);                     \
      }else{                                    \
        return func##_ret;                      \
      }                                         \
    }else {                                     \
      longjmp(func##_gen_jmp,1);                \
    }											\
	return stopIterationVal;                    \
  }                                             \
  void func##__real(argt argv)
