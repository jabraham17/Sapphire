# Type Rules

## Assignment

`call(=, lhs, rhs)`

```text
(lhs: unknown), (rhs: type1) -> (lhs: type1), (call: type1)
(lhs: type1),   (rhs: type1) -> (call: type1)
```

## New

### Array

Result type is the array type

## Function call

Result type is return type of function

## Subscript

`call([], sequence, index)`

```text
(sequence: arrayType(elmType)),  (index: int|uint) -> (call: elmType)
(sequence: tupleType(elmTypes)), (index: int|uint) -> (call: elmTypes[index])
```

## Add

`call(+, a, b)`

```text
(a: uint),          (b: uint)          -> (call: uint)
(a: uint),          (b: int)           -> (call: int)
(a: int),           (b: int|uint)      -> (call: int)
(a: real),          (b: int|uint|real) -> (call: real)
(a: int|uint|real), (b: real)          -> (call: real)
(a: string),        (b: string)        -> (call: string)
(a: byte),          (b: byte)          -> (call: byte)
```

## Minus

`call(-, a, b)`

```text
(a: uint),          (b: uint)          -> (call: uint)
(a: uint),          (b: int)           -> (call: int)
(a: int),           (b: int|uint)      -> (call: int)
(a: real),          (b: int|uint|real) -> (call: real)
(a: int|uint|real), (b: real)          -> (call: real)
(a: byte),          (b: byte)          -> (call: byte)
```

## Multiply

`call(*, a, b)`

```text
(a: uint),          (b: uint)          -> (call: uint)
(a: uint),          (b: int)           -> (call: int)
(a: int),           (b: int|uint)      -> (call: int)
(a: real),          (b: int|uint|real) -> (call: real)
(a: int|uint|real), (b: real)          -> (call: real)
(a: string),        (b: int|uint)      -> (call: string)
(a: byte),          (b: byte)          -> (call: byte)
```

## Divide

`call(/, a, b)`

```text
(a: uint),          (b: uint)          -> (call: uint)
(a: uint),          (b: int)           -> (call: int)
(a: int),           (b: int|uint)      -> (call: int)
(a: real),          (b: int|uint|real) -> (call: real)
(a: int|uint|real), (b: real)          -> (call: real)
(a: byte),          (b: byte)          -> (call: byte)
```

## Left Shift

`call(<<, a, b)`

```text
(a: uint), (b: uint|int)      -> (call: uint)
(a: int),  (b: int|uint)      -> (call: int)
(a: byte), (b: byte|int|uint) -> (call: byte)
```

## Right Shift

`call(>>, a, b)`

```text
(a: uint), (b: uint|int)      -> (call: uint)
(a: int),  (b: int|uint)      -> (call: int)
(a: byte), (b: byte|int|uint) -> (call: byte)
```

## Modulo

`call(%, a, b)`

```text
(a: uint), (b: uint)     -> (call: uint)
(a: uint), (b: int)      -> (call: int)
(a: int),  (b: int|uint) -> (call: int)
```

## Negate

`call(-, a)`

```text
(a: int|uint|byte|real) -> (call: a.type)
```

## Boolean And

`call(&&, a, b)`

```text
(a: int|uint|byte|bool), (b: int|uint|byte|bool) -> (call: bool)
```

## Boolean Or

`call(||, a, b)`

Same as [Boolean And](##Boolean-And)

## Equal To

`call(==, a, b)`

```text
(a: int), (b: int) -> (call: bool)
(a: uint), (b: uint) -> (call: bool)
(a: byte), (b: byte) -> (call: bool)
(a: real), (b: real) -> (call: bool)
(a: bool), (b: bool) -> (call: bool)
(a: string), (b: string) -> (call: bool)
```

## Not Equal To

`call(!=, a, b)`

Same as [Equal To](##Equal-To)

## Less Than

`call(<, a, b)`

```text
(a: int), (b: int) -> (call: bool)
(a: uint), (b: uint) -> (call: bool)
(a: byte), (b: byte) -> (call: bool)
(a: real), (b: real) -> (call: bool)
```

## Greater Than

## Less Than or Equal To

## Greater Tahn or Equal To

## Bitwise And

## Bitwise Or

## Bitwise Negate

## Not
