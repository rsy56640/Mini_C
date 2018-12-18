# Mini_C

本项目为 “解释器构造实践” 课程作业。  
项目成员： [任思远](https://github.com/rsy56640)，[黎冠延](https://github.com/ssyram)，[刘瑞康](https://github.com/Endless-Fighting)，[朱申](https://github.com/Evan-Choo)。  


&nbsp;   
## 项目简介

1. 调用 `Mini_C::lexer::Lexer lexer; lexer.tokenize(const char* filename)` 扫描文件（使用 `lexer.print()` 输出 token 信息）
2. 调用 `Mini_C::LR1::analyze(lexer);` 进行 LR1 分析，并在规约时进行相应的语义动作
3. AST


&nbsp;   
## 其他

#### doc 中的 [rule_origin.tsl](https://github.com/rsy56640/Mini_C/blob/interpreter/doc/rule_origin.tsl) 是文法产生式的原版文件，作为 [RulesTranslator](https://github.com/rsy56640/RulesTranslator) 的输入，输出是2个文件：

- [`lr1.hpp`](https://github.com/rsy56640/Mini_C/blob/interpreter/src/lr1.hpp)（直接用于 Mini_C 项目）
- `rule.lr1`（展现了 LR1 的DFA）

#### 定义输出流 `out`，用于在 lr1.hpp 中分析过程的输出，即输出移进和规约的过程。

#### doc 中的 [rule_for_print_syntax_tree.tsl](https://github.com/rsy56640/Mini_C/blob/interpreter/doc/rule_for_print_syntax_tree.tsl) 用于画出文法树（只用来展示，不用于分析），输出的文件使用 graphiz 运行即可。


&nbsp;   
## 关于 RulesTranslator 的说明

原作者：黎冠延，[ssyram/RulesTranslator](https://github.com/ssyram/RulesTranslator)   
目前项目中使用的是我稍作修改的版本。   

有几个 path 我写死了：

- WholeTest.cpp
  - `"rule.tsl"`（文法产生式输入文件）
  - `"lr1.hpp"`（输出的 hpp 文件，直接用于 Mini_C 项目）
- TableGenerator.cpp 中 `"rule.lr1"`，输出 LR1 的 DFA
  - 如果文法中有 “移进规约冲突”，默认选择规约，并且最后所有的选择信息输出在 `"Collision choose list: x entries"`
  - 如果有 “规约规约冲突”，那么立即结束，最后输出 `"Collision occurs"` 和规约的产生式编号