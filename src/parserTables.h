#ifndef PAreduceshiftequalreduce_TABLequalshift_h
#define PAreduceshiftequalreduce_TABLequalshift_h

typedef enum {
	shift, reduce, error, equal
} prreduceule;

prreduceule prTable[][] =
 //+       -       *       /       <       >       <=      >=      =       <>      (      )       fn      ,       $       var
{{ reduce, reduce, shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // +
 { reduce, reduce, shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // -
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // *
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // /
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // <
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // >
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // <
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // >
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // =
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, shift,  reduce, reduce, shift },   // <
 { shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift, equal,  shift,  equal,  error,  shift },   // (
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, equal, reduce, reduce, reduce, reduce, error },   // )
 { error,  error,  error,  error,  error,  error,  error,  error,  error,  error,  equal, error,  error,  error,  error,  error },   //fn
 { shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift, equal,  shift,  equal,  error,  shift },   // ,
 { shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift, error,  shift,  error,  error,  shift },   // $
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, error, reduce, error,  reduce, reduce, error } };   //var

#endif
