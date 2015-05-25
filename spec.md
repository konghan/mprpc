

##base types:
0. nil:
1. bool: a boolean value
2. i8: an 8-bit signed integer
3. u8: an 8-bit unsigned integer
4. i16: a 16-bit signed integer
5. u16
6. i32:
7. u32:
8. i64:
9. u64:
10. float:
11. double:
12. string:
13. binary:

## container
1. array<>:
2. map<>:

##MPRPC Interface description language
1. Document ::= Header* Definition*

2. Header ::= Include | CppInclude | Namespace
3. Include ::= 'include' Literal
4. CppInclude ::= 'cpp_include' Literal
5. Namespace ::= ('namespace' (NamespaceScope Identifier) |
                            ('smalltalk.category' STIdentifier) |
                            ('smalltalk.prefix' Identifier)) |
                  ('php_namespace' Literal) |
                  ('xsd_namespace' Literal)
6. NamespaceScope ::= '*'|'cpp'|'java'|'py'|'perl'|'rb'|'cocoa'| 'csharp'

7. Definition ::= Const | Typedef | Enum | Struct | Union | Exception | Service | Topic

8. Const ::= 'const' FieldType Identifier '=' ConstValue ListSeparator?
9. Typedef ::= 'typedef' DefinitionType Identifier
10. Enum ::= 'enum' Identifier '{'(Identifier('=' IntConstant)? ListSeparator?)* '}'
11. Struct ::= 'struct' Identifier '{' Field* '}'
12. Union ::= 'union' Identifier '{' Field* '}'
13. Exception ::= 'exception' Identifier '{' Field* '}'
14. Service ::= 'service' Identifier ('extends' Identifier)? '{' Function* '}'
15. Field ::= FieldID? FieldReq? FieldType Identifier ('=ConstValue)? XsdFieldOptions ListSeparator?
16. FieldID ::= IntConstant ':'
17. FieldReq ::= 'required' | 'optional'

18. Function ::= 'oneway'? FunctionType Identifier '(' Field* ')' Throws? ListSeparator?
19. FunctionType ::= FieldType | 'void'
20. Throws ::= 'throws' '(' Field* ')'

###Types
21. FieldType ::= Identifier | BaseType | ContainerType
22. DefinitionType ::= BaseType | ContainerType
23. BaseType ::= 'bool' | 'int8_t' | 'uint8_t' | 'int16_t' | 'uint16_t' | 'int32_t' | 'uint32_t' | 'int64_t' | 'uint64' | 'float' | 'double' | 'binary'
24. ContainerType ::= Maptype | ListType
25. Maptype ::= 'map' Cpptype? '<' FieldType ',' FieldType '>'
26. ListType ::= 'list' '<' FieldType '>' CppType?
27. CppType ::= 'cpp_type' Literal

###Constant Values
28. ConstValue ::= IntConstant | FloatConstant | DoubleConst | Literal | Identifier | ConstList | ConstMap
29. IntConstant ::= ('+' | '-')? Digit+
30. DoubleConstant ::= ('+' | '-')?Digit*('.' Digit+)?(('E'|'e')IntConstant)?
31. ConstList ::= '['(ConstValue ListSeparator?)* ']'
32. ConstMap ::= '{' (ConstValue ':' ConstValue ListSeparator?)* '}'

###Literal
33. Literal ::= ('"' [^"]* '"')|("'" [^']* "'")

###Identifier
34. Identifier ::= (Letter | '_')(Letter|Digit|'.'|'_')*

###List Separator
35. ListSeparator ::= ',' | ';'

###Letters and Digits
36. Letter ::= ['A'-'Z'] | ['a' - 'z']
37. Digit ::= ['0'-'9']

###Topic
38. Topic ::= 'topic' Identifier '{' Field* '}'



