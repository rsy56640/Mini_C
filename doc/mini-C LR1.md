# 关于mini-C LR1分析的实现

黎冠延实现的 LR1分析器： original branch：[ssyram/RulesTranslator](https://github.com/ssyram/RulesTranslator)   
我稍微加工了一下，[rsy56640/RulesTranslator](https://github.com/rsy56640/RulesTranslator)，也就是现在用的 branch。改进了输出，非侵入式地增加了移进规约冲突的处理和日志。（规约规约冲突还是直接中断）   
LR1分析器的输出就是 [`lr1.hpp`](https://github.com/rsy56640/Mini_C/blob/interpreter/src/lr1.hpp)，可以直接对接。   
使用前参考[文档](https://github.com/rsy56640/RulesTranslator/blob/master/README.md)，并且设置*文件路径*。   