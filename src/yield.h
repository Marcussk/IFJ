#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>

#define YIELD(func, n) if (! setjmp(func##_gen_jmp)) {  \
      func##_ret = n;                                   \
         longjmp(func##_caller_jmp, 1);                 \
  }

// returntype, functionIdentificator, argumentType, argumentVariable
#define GENERATOR(ret, func, argt, argv)        \
  static jmp_buf func##_caller_jmp;             \
  static jmp_buf func##_gen_jmp;                \
  static bool func##_continue=false;            \
  static ret func##_ret;                        \
                                                \
  void func##__real(argt argv);                 \
                                                \
  ret func(argt argv) {                         \
    if (!func##_continue) {                     \
    func##_continue=true ;                      \
      if (! setjmp(func##_caller_jmp)) {        \
        func##__real(argv);                     \
      } else {                                  \
        return func##_ret;                      \
      }                                         \
    }                                           \
     else {                                     \
      longjmp(func##_gen_jmp,1);                \
    }                                           \
    return 0;                                   \
  }                                             \
  void func##__real(argt argv)


/*
 // example usage


GENERATOR(int, getNext, int, n) {
  static int counter;

  counter = n;
    while (true) {
        counter = counter+1;
        YIELD(getNext, counter);
    }
}

int main() {
    while (true) {
      int n = getNext(1);
        if (n > 42)
           break;
        printf("%d\n",n);
    }
    return 0;
}

*/
