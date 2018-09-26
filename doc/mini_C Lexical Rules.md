# mini-C Lexical Rules
----
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

## 3. escape rules
  - supported rules are: \a, \b, \f, \xNN(N is a hex number),
  \nnn(n is an oct number), \', \", \n, \t, \r
  - not supported rules will be recognized as the char itself
      - like: '\c' is 'c'
