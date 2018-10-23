# mini-C Lexical Rules

## Extended Backus–Naur Form ISO-14977

## Notation

This uses my XBNF Extended BNF Notation where "|" indicates "or", "(...)" indicates priority. For more information see [intro_ebnf.html](http://csci.csusb.edu/dick/maths/intro_ebnf.html)
The following abbreviations are also used:

- `O(_)` :=  0 or 1 occurrences,
- `N(_)` :=  1 or more occurrence
- `List(_)` :=  a comma separated list. **[[No use in Mini_C for expressions]]**
- `#(_)` :=  0 or more occurrences.
- `S(E,Op)` : serial_operator_expression(E, Op)
- serial_operator_expression(E,Op) :  E #(Op E)

        S(E,Op) = E Op E Op E Op ... E


&nbsp;   
## Lexemes

    identifier = nondigit #(nondigit | digit) ;
    
    nondigit = "_" | "a" | "A" | "b" | "B" | "c" | "C" | "d" | "D" | "e" | "E" | "f" | "F" | "g" | "G" | "h" | "H" | "i" | "I"
                   | "j" | "J" | "k" | "K" | "l" | "L" | "m" | "M" | "n" | "N" | "o" | "O" | "p" | "P" | "q" | "Q" | "r" | "R"
                   | "s" | "S" | "t" | "T" | "u" | "U" | "v" | "V" | "w" | "W" | "x" | "X" | "y" | "Y" | "z" | "Z" ;
    
    digit_delimiter = "_" ;
    
    digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
     
    punctuator = "[" | "]" | "(" | ")" | "{" | "}" | "*" | "," | ":" | "=" | ";" ;

    operator = "[" | "]" | "(" | ")" | "." | "->" | "++" | "--" | "&" | "*" | "+" | "-" | "~" | "!" | "sizeof" | "/" | "%"
            | "<<" | ">>" | "<" | ">" | "<=" | ">=" | "==" | "!=" | "^" | "|" | "&&" | "||" | "?" | ":"
            | "=" | "*=" | "/=" | "%=" | "+=" | "-=" | "<<=" | ">>=" | "&=" | "^=" | "||=" | "," ;

    infix = "->" | "&" | "*" | "+" | "-" | "/" | "%" | "<<" | ">>" | "<" | ">" | "<=" | ">=" | "==" | "!=" | "^" | "|"
          | "&&" | "||" | "=" | "*=" | "/=" | "%=" | "+=" | "-=" | "<<=" | ">>=" | "&=" | "^=" | "||=" | "," ;

    prefix = "++" | "--" | "&" | "*" | "+" | "-" | "~" | "!" | "sizeof" ;

    postfix = "++" | "--" ;
   
    integer_suffix = O(unsigned_suffix) ;
    
    unsigned_suffix = "u" | "U" ;
    
    sign = "-" ;
    
    hex_constant = ("0x" | "0X") (hex_digit) ;
    
    hex_digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
              | "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" ;
    
    number = digit #(digit | digit_delimiter) ;
    
    decimal_constant = O(sign) number ;
    
    non_zero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
    
    integer_constant = (decimal_constant O(integer_suffix)) | hex_constant) ;
    
    float_suffix = "f" | "F" ;
    
    fraction = number "." number ;
    
    exponent_part = ("e" | "E") O(sign) number ;
    
    float_constant = O(sign) fraction O(exponent_part) O(float_suffix) ;
    
    enumeration_constant = identifier ;
    
    char_constant = char~(double_quote|backslash)| escape_sequence ;
    
    escape_sequence = backslash (char |"0x"#hexadecimal_digit) ;
    
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
                   | struct_union_specifier | enumeration_specifier | typedef_name ;
    
    type-qualifier = "const" ;
    
    typedef_name = identifier ;

### Initialization

    initializer = assignment_expression | initializer_list ;
    
    initializer_list = List(initializer) ;
    
    declarator_initialized = declarator ("=" initializer) ;

### Structs

    structure_declarator = declarator | declarator ":" constant_expression ;
    
    structure_declarator_list = List(structure_declarator) ;
    
    structure_declaration = (type_specifier | type_qualifier) structure_declarator_list ";" ;
    
    struct_union_specifier = struct_union identifier | struct_union identifier "{"structure_declarator_list "}" ;
    
    struct_union = ( "struct" | "union" ) ;

### Enums

    enumeration_value = enumeration_constant ("=" constant_expression|) ;
    
    enumeration_list = List(enumeration_value ) ;
    
    enumeration_specifier = enumeration_identifier | "enum" identifier "{"enumeration_list"}" ;

### Functions

    function_definition = "fn" declarator declaration_list "->" declaration_specifier compound_statement ;
    
    parameter_declaration = #declaration_specifier declarator | abstract_declarator ;
    
    parameter_list = List(parameter_declaration) (",..."|) ;

### Main Function

    // A complete C program has to have a function with name 'main'.    
    // This is the function called by the operating system.   
    // It must return an int value indicating whether the prograam executed correctly or if there was an error.   
    // In UNIX, the main program returns 0 to indicate no errors.    
    // Their are several valid forms:   
    fn main()  -> i32 
    fn main(argc, argv)  -> i32 
    fn main(argc, argv, envp)  -> i32 
    // The parameters are set up by the operating system when the program starts. The traditional arg stands for argument.   
    // Note that argv[argc] == NULL

### Pointers

    pointer = #( "*" | #type_qualifier) ;
    
    declarator = pointer | direct_declarator ;

### Functions and Arrays

    post_declarator = "["constant_expression"]" | "("parameter_list")" | "("identifier_list")" ;
    
    direct_declarator = identifier | "("declarator")" | direct_declarator post_declarator ;
    
    abstract_declarator = pointer | pointer direct_abstract_declarator ;
    
    direct_abstract_declarator = "(" abstract_declarator ")" | O( direct_abstract_declarator) O("[" O(constant_expression) "]"
                               | "(" O(parameter_list) ")" ) ;


&nbsp;   
## Statements

    statement = labeled_statement | compound_statement | expression_statement
              | selection_statement | iteration_statement | jump_statement ;

### Branch

    jump_statement = "continue" ";" | "break;" | "return" expression ";" ;

### Structured

    loop = iteration_statement ;
    
    iteration_statement = "while" "("expression")" statement | "do" statement "while" "("expression")" ";" | for_statement ;
    
    for_statement = "for" "("expression ";" expression ";" expression")" statement, ;
    
    selection_statement = if_statement | "switch" "("expression")" statement ;
    
    if_statement = "if ("expression")" statement | "if" "("expression")" statement "else" statement ;
    
    expression_statement = expression ";" ;
    
    labeled_statement = "case"  constant_expression ":" statement | "default" ":" statement ;

### Compound

    compound_statement = block | "{" #statement "}" ;
    
    block = "{" declaration #declaration #statement "}" ;


&nbsp;   
## Pre-Processor Commands

    preprocess_token = identifier | constant | string_literal
                     | operator | punctuator | each Non-white space not one of the previous ;
    
    header_char = any character except new_line | and | > ;
    
    header_name = #(header_char) ;
    
    new_line = new_line character ;
    
    Left_paren = left parenthesis with no white space before it ;
    
    control_line = "#include" (#(preprocess_token | header_name) new_line 
                 | "#define" identifier #(preprocess_token) new_line
                 | "#define" identifier left_paren identifier_list #(preprocess_token) new_line,
                 | "#undef" identifier new_line ;
    
    endif_line = "#endif" new_line ;
    
    elif_group = "#elif" constant_expression new_line pp_group ;
    
    else_group = "#else" new_line pp_group ;
    
    if_group = ("#if" constant_expression | "#ifdef" identifier | "#ifndef" identifier) new_line pp_group ;
    
    if_part = if_group #(elif_group) else_group endif_line ;
    
    pp_part = #preprocess_token new_line | if_part | control_line ;
    
    pp_group = #(pp_part) ;
