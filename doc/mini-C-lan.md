# Mini C Lang Description
&nbsp;   
## Basics

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
    - `i16`, `i32`, `u16`, `u32`
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



### [`#include`](https://en.cppreference.com/w/c/preprocessor/include)
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
## Memory Management

- `new()`
  - `type` `id` `=` `new` `type` `[amount]`
  - `type` `id` `=` `new` `type` `()`
- `delete()`
  - `delete` `id`
  - `delete` `[]` `id`

<a></a>

    i32* i = new i32[7];
    for (i32 j = 0; j < 7; j++)
        i[j] = j;
    delete[] i;


&nbsp;   
&nbsp;   
## Expression
-----

    A::_a = 23u + x[12] + 0x0_1_000_000_100;
    
    A *a;
    
    a->_b = A::_a  -0x723;
    
    if(a->_b == 3) A::_a = 7e1_0;
    
    fn foo(i32 i, str s)-> str { return "hello" + str; }
    
    for(i32 i = 0; i < 100; i++)
    	a->_b++;
    	
    static const f64 &f= foo()->i++ -.343_233+17 * 8_.23_ + ++_u;
    
    b=-23 - -7- (--a)++ - -.723_233_9e-4+(0+(b-(b/(7.22)-4))-0u)+ --a.__x;
    
    bool b = true | false || false & true;
    
    b%=(a+++3.777_098)+ sizeof(a);
    
    f64 f = -9+(2+ -23-.455 +    3.423_2-5)* 1_070_022 %55+ 1_233_123.77 + -.7;
    
    const char* s ="hello world";
    
    char c= '\n';
    
    using Ap_t = A::A_ptr_t;


&nbsp;   
&nbsp;   
## Function


`fn [function_name] ([parameter-list]) -> [return_type]  [body]`   
e.g.  

    fn foo(i32 i, str s) -> void
    {
        i32 result = 0x7ff;
        if((i << 2) < result)
            print(s);
        else print(i);
    }
    
    fn goo() -> const i32 
    {
        return 27;
    }


&nbsp;   
&nbsp;   


## struct & union

    struct A { static i32 _a; i32 _b; using A_ptr_t = A*; enum{ a = 1;} };
    
    union B { i32 i; f32 f; };


&nbsp;   
&nbsp;   


## main function

    // A complete C program has to have a function with name 'main'.    
    // This is the function called by the operating system.   
    // It must return an int value indicating whether the prograam executed correctly or if there was an error.   
    // In UNIX, the main program returns 0 to indicate no errors.    
    // Their are several valid forms:   
    fn main() -> i32 
    fn main(argc, argv) -> i32 
    fn main(argc, argv, envp) -> i32 
    // The parameters are set up by the operating system when the program starts. The traditional arg stands for argument.   
    // Note that argv[argc] == NULL


&nbsp;   
&nbsp;   


## Code Sample

    struct Student {
        str name;
        u16 rank;
    }
    
    struct StudentNode {
        Student* student;
        StudentNode* next_student;
    }
    
    struct Class {
        StudentNode* dummy;
        i32 amount;
    }
    
    fn empty_class() -> Class {
        StudentNode* snode = new StudentNode();
        snode->student = 0;
        snode->next_student = 0;
        Class c;
        c.dummy = snode;
        c.amount = 0;
        return c;
    }
    
    fn destruct_class(Class* c) -> void {
        StudentNode* cur = c->dummy;
        for (i32 i = c->amount; i > 0 ; i--)
        {
            StudentNode* temp = cur;
            cur = cur->next_student;
            if(i != c->amount) delete temp->student;
            delete temp;
        }
    }
    
    fn register_in_class(Class* c, Student* s) -> void {
        StudentNode* snode = new StudentNode();
        snode->student = s;
        snode->next_student = c->dummy->next_student;
        c->dummy->next_student = snode;
        c->amount++;
    }
    
    fn main() -> i32
    {
        Class c = empty_class();
        Student* Alice = new Student();
        register_in_class(&c, Alice);
        Student* Bob = new Student();
        register_in_class(&c, Bob);
        destruct_class(&c);
        return 0;
    }