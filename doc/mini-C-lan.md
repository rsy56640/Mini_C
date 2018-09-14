# Mini C Lang Description


&nbsp;   
&nbsp;   
## Basics
-----

<a id="comments"></a>
### [comments](https://en.cppreference.com/w/c/comment)
Syntax:  
> /* comment */  
> // comment


<a id="type"></a>
### [type](https://en.cppreference.com/w/c/language/type)
#### type classification

- `void`
- basic type
    - `char`
    - `bool`
    - `i32`, `i64`, `u32`, `u64`
    - `f32`, `f64`
- [enumerated type](#)
- derived type
    - [array]()
    - [structure]()
    - [function]()
    - [pointer]()
    - [thread]()



<a id="main_function"></a>
### [main function](https://en.cppreference.com/w/c/language/main_function)
Syntax:  
> `int main(void) { body }`   
> `int main(int argc, char* agrv[]) { body }`

NB: `argv[argc]` is guaranteed to be 0.





&nbsp;   
&nbsp;   
## Preprocessor
-----

### [Conditional inclusion](https://en.cppreference.com/w/c/preprocessor/conditional)
Syntax:  

- `#if` expression		
- `#ifdef` identifier		
- `#ifndef` identifier		
- `#elif` expression		
- `#else`		
- `#endif`

### [`#define`](https://en.cppreference.com/w/c/preprocessor/replace)
Syntax:

- `#define` identifier replacement-list(optional)
- `#define` identifier( parameters ) replacement-list
- `#define` identifier( parameters, ... ) replacement-list
- `#define` identifier( ... ) replacement-list
- `#undef` identifier



### `#include`
Syntax:  

- `#include <filename>`
- `#include "filename"`





&nbsp;   
&nbsp;   
## Statement
-----

### [`if`](https://en.cppreference.com/w/c/language/if)
Syntax:

- `if` ( expression ) statement_true
- `if` ( expression ) statement_true `else` statement_false

Note: The else is always associated with the closest preceding if.


### [`switch`](https://en.cppreference.com/w/c/language/switch)
Syntax:   
`switch` ( expression )   
`{`  
&nbsp;&nbsp;&nbsp;  `case` constant_expression : statement  
&nbsp;&nbsp;&nbsp;  `default` : statement  
`}`  


### [`for`](https://en.cppreference.com/w/c/language/for)
Syntax:   
`for` ( init_clause `;` cond_expression `;` iteration_expression ) loop_statement		


### [`while`](https://en.cppreference.com/w/c/language/while)
Syntax:  
`while` ( expression ) statement		

Note: Boolean and pointer expressions are often used as loop controlling expressions. The boolean value false and the null pointer value of any pointer type compare equal to zero.


### [`do-while`](https://en.cppreference.com/w/c/language/do)
Syntax:   
`do` statement `while` ( expression ) `;`  


### [`continue`](https://en.cppreference.com/w/c/language/continue)
Syntax:   
`continue;`  

The `continue` statement causes a jump, to the end of the loop body (it may only appear within the loop body of `for`, `while`, and `do-while` loops).


### [`break`](https://en.cppreference.com/w/c/language/break)
Syntax:   
`break;`

Appears only within the statement of a loop body (`while`, `do`, `for`) or within the statement of a `switch`.

After this statement the control is transferred to the statement or declaration immediately following the enclosing loop or switch.


### [`return`](https://en.cppreference.com/w/c/language/return)
Syntax:   

- `return` expression`;` (1)
- `return;` (2)

(1) Evaluates the expression, terminates the current function and returns the result of the expression to the caller (the value returned becomes the value of the function call expression). Only valid if the function return type is not `void`.

(2) Terminates the current function. Only valid if the function return type is `void`.

Note: If the type of the expression is different from the return type of the function, its value is converted as if by assignment to an object whose type is the return type of the function, except that overlap between object representations is permitted.






&nbsp;   
&nbsp;   
## Expression
-----








&nbsp;   
&nbsp;   