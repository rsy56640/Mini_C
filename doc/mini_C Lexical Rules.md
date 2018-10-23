# mini-C Lexical Rules

## 1. identifier
  - can be compounded only by 0-9, '_', alphabet
  - must start with '_' or alphabet

## 2. numbers
  - all numbers can insert meaningless char '_' to divide the number
    - note that '_' can't be placed at the beginning of the number and just after '.'
  - integer:
   - can be organized by 0-9
   - if it's minus, there can be divider between '-' and the actual number
      - like: -  1
   - if you'd rather it be unsigned, you can append 'u' or 'U' after the number
      - note that minus and unsigned can't be placed together
      - like: 15u, 15U (there should not be any divider between number and 'u'/'U')
  - decimal number:
    - can start with '.', also if it's minus, there can be divider(s) between '-' and the actual number
        - like: - .7(-0.7)
    - decimal number can't be followed by 'u'/'U'
    - support scientific notation, it's divided by 'e'/'E'
        - note that the number follow 'e'/'E' must no be divided by divider
        - like: 10E-1(neither"10E -1" nor "10E- 1" is supported ), 123e5
    - the number use scientific notation will be recognized as f64,
    otherwise, f32


&nbsp;   
&nbsp;   


## Extended Backus–Naur Form ISO-14977

## Notation

This uses my XBNF Extended BNF Notation where "|" indicates "or", "(...)" indicates priority. For more information see [intro_ebnf.html](http://csci.csusb.edu/dick/maths/intro_ebnf.html)
The following abbreviations are also used:

- O(_) :=  0 or 1 occurrences,
- N(_) :=  1 or more occurrence
- L(_) :=  a comma separated list
- #(_) :=  0 or more occurrences.
- S(E,Op) : serial_operator_expression(E, Op)
- serial_operator_expression(E,Op) :  E #(Op E)

        S(E,Op) = E Op E Op E Op ... E


&nbsp;   
## Lexemes

    identifier = nondigit #(nondigit | digit) ;
    
    nondigit = "_" | "a" | "A" | "b" | "B" | "c" | "C" | "d" | "D" | "e" | "E" | "f" | "F" | "g" | "G" | "h" | "H" | "i" | "I" | "j" | "J" | "k" | "K" | "l" | "L" | "m" | "M" | "n" | "N" | "o" | "O" | "p" | "P" | "q" | "Q" | "r" | "R" | "s" | "S" | "t" | "T" | "u" | "U" | "v" | "V" | "w" | "W" | "x" | "X" | "y" | "Y" | "z" | "Z" ;
    
    digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
     
    punctuator = "[" | "]" | "(" | ")" | "{" | "}" | "*" | "," | ":" | "=" | ";" | "..." | "#" ;

    operator = "[" | "]" | "(" | ")" | "." | "->" | "++" | "--" | "&" | "*" | "+" | "-" | "~" | "!" | "sizeof" | "/" | "%" | "<<" | ">>" | "<" | ">" | "<=" | ">=" | "==" | "!=" | "^" | "|" | "&&" | "||" | "?" | ":" | "=" | "*=" | "/=" | "%=" | "+=" | "-=" | "<<=" | ">>=" | "&=" | "^=" | "||=" | "," | "#" | "##" ;

    infix = "->" | "&" | "*" | "+" | "-" | "/" | "%" | "<<" | ">>" | "<" | ">" | "<=" | ">=" | "==" | "!=" | "^" | "|" | "&&" | "||" | "=" | "*=" | "/=" | "%=" | "+=" | "-=" | "<<=" | ">>=" | "&=" | "^=" | "||=" | "," ;

    prefix = "++" | "--" | "&" | "*" | "+" | "-" | "~" | "!" | "sizeof" ;

    postfix = "++" | "--" ;
   
    integer_suffix = #(unsigned_suffix) | #(long_suffix) ;
    
    unsigned_suffix = "u" | "U" ;
    
    long_suffix = "l" | "L" ;
    
    sign = "+" | "-" ;
    
    octal_constant = "0" #(octal_digit) ; 
    
    octal_digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" ;
    
    hex_constant = ("0x" | "0X") (hex_digit) ;
    
    hex_digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" | "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" ;
    
    decimal_constant = non_zero_digit #(digit) ;
    
    non_zero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
    
    integer_constant = (decimal_constant | octal_constant | hex_constant) | integer_suffix ;
    
    float_suffix = "f" | "l" | "F" | "L" ;
    
    fraction = #digit "." digit #digit ;
    
    exponent_part = ("e" | "E") sign #(digit) ;
    
    float_constant = fraction (exponent_part|) (float_suffix|)|(decimal_constant (exponent_part|) float_suffix ;
    
    enumeration_constant = identifier ;
    
    char_constant = char~(double_quote|eoln|backslash)| escape_sequence ;
    
    escape_sequence = backslash (char | "0" #octal_digit |"0x"#hexadecimal_digit) ;
    
    character_constant = "'" char_constant"'" ;
    
    constant = float_constant | integer_constant | enumeration_constant | character_constant ;
    
    string__char = char~(double_quote|eoln|backslash)| escape_sequence ;
    
    string_literal = double_quote #(string_char) double_quote ;


&nbsp;   
## Expression

    primary_expression = variable | constant | string_literal | "(" expression ")" ;
    
    variable = identifier & declared and in scope of declaration ;
    
    argument_list = List(assignment_expression) ;

### Arithmetic

    post_fix = "++" | "--" ;
    
    post_fix_expression = (primary_expression) #(post_fix) ;
    
    unary_operator = "&" | "*" | "+" | "-" | "!" | "-" ;
    
    pre_fix = "++" | "--" | "sizeof" ;
    
    unary_expression = #(pre-fix) post_fix_expression | unary_operator cast_expression | "sizeof" "(" type_name")" ;
    
    cast_expression = #(type_name) unary_expression ;   
    // This implies that casts are done after doing post-fix operations.
    
    multiplicative_expression = S(cast_expression, multiplicative_operator). [ serial_operator_expression ] ;   
    // The rule above means that 'casts' are done before multiplication and division,    
    // and that multiplication and division are done from left to right.
    
    multiplicative_operator = "*" | "%" | "/" ;
    
    additive_expression = S(multiplicative_expression, additive_operator) ;   
    // This means that addition and subtraction occurs after multiplication and from left to right.
    
    additive_operator = "+" | "-" ;

### Shifts

    shift_expression = S(additive_expression, shift_operator) ;
    
    shift_operator = ">>" | "<<" ;

### Relations

    relational_expression = S(shift_expression, relational_operator) ;
    
    relational_operator = "<" | ">" | "<=" | ">=" ;
    
    equality_expression = S(relational_expression, equality_operator) ;
    
    equality_operator = "==" | "!=" ;

### Bitwise Expressions

    AND_expression = S(equality_expression, and_operator) ;
    
    and_operator = "&" ;
    
    XOR_expression = S(AND_expression, XOR_operator) ;
    
    XOR_operator = "^" ;
    
    OR_expression = S(XOR_expression, OR_operator) ;
    
    OR_operator = "|" ;

### Logical Expressions
    
    // In C, logical false is reresented by any zero value and true by any nonzero value.
    
    and = "&&" ;
    
    or = "||" ;
    
    not = "!" ;
    
    logical_AND_expression = S(OR_expression, logical_AND_operator) ;
    
    logical_AND_operator = and ;   
    // A&&B is true precisely when both A and B evaluate to be true. If A evaluates to false, B is not evaluated.
    
    logical_OR_expression = S(logical_AND_expression, logical_OR_operator) ;
    
    logical_OR_operator = or ;   
    // A||B is true if A evaluates to be true, or when A is false and B evaluates to be true.   
    // If both evaluate to false (zero) then A||B is false.

### Conditional Expressions

    conditional_expression = logical_OR_expression | logical_OR_expression "?" expression ":" conditional_expression ;

### Assignment Statements

    assignment_expression = S(unary_expression, assignment_operator) ;
    
    assignment_operator = "=" | "*=" | "/=" | "%=" | "+=" | "<<=" | ">>=" | "&=" | "^=" | "|=" ;
    
    expression = List(assignment_expression ) ;
    
    constant_expression = conditional_expression ;


&nbsp;   
## Declarations

    declaration = declaration_specifier | declarator_list ;
    
    declarator_list = List(declarator_initialized) ;
    
    declaration_specifier = (storage_class | type_specifier | type_qualifier) ;
    
    storage_class = "using" | "static" ;

### Types

    type_specifier = "void" | "char" | "i16" | "i32" | "u16" | "u32" | "f32" | "f64" 
                    | struct_specifier | enumeration_specifier | typedef_name ;
    
    type-qualifier = "const" ;
    
    typedef_name = identifier ;

### Initialization




### Structs




### Enums




### Functions



### Main Function






&nbsp;   
## Statements



### Branch



### Structured



### Compound




&nbsp;   
## Pre-Processor Commands


