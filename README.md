# PopDB++
PopDB is a simple queue based database, made for persisting simple tasks in file or sharing tasks between processes in computer.
PopDB is provided with simple abstraction layer to the dbs, which is SEQL (Simple Extensible Query Language).


## Compiling

```
blah blah
todo
```

## Definition file

## SEQL
SEQL is provided with commands
* POP
* INSERT_BEGIN
* INSERT
* SELECT_NTH
* DELETE_NTH

### Expressions
Expressions in SEQL must be comma separated, and each fragment within it must be space separated.
eg.
```
INSERT_BEGIN,"HELLO",var1 + var2
VAR test = 1 + 2
```

For more detailed usage of PopDB please check /examples.
