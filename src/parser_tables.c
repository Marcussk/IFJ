#include "parser_tables.h"

precedenceRule prTable[16][16] =
 //+       -       *       /       <       >       <=      >=      =       <>      (      )       ,       $       var    fn
{{ reduce, reduce, shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // +
 { reduce, reduce, shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // -
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // *
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // /
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // <
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // >
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // <
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // >
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // =
 { shift,  shift,  shift,  shift,  reduce, reduce, reduce, reduce, reduce, reduce, shift, reduce, reduce, reduce, shift, shift   },   // <
 { shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift, equal,  equal,  error,  shift, shift   },   // (
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, equal, reduce, reduce, reduce, error, reduce  },   // )
 { shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift, equal,  equal,  error,  shift, shift   },   // ,
 { shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift,  shift, error,  error,  error,  shift, shift   },   // $
 { reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, reduce, error, reduce, reduce, reduce, error, error   },   //var
 { error,  error,  error,  error,  error,  error,  error,  error,  error,  error,  equal, error,  error,  error,  error, error   }    //fn
};
