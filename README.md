# PopDB++
PopDB is a simple queue based database, made for persisting simple tasks in file or sharing tasks between processes in computer.
PopDB is provided with simple abstraction layer to the dbs, which is SEQL (Simple Extensible Query Language).


## Compiling

```
todo
```

## Definition file
At the current moment, the definition of DB record supports setting additional fields.
Each field has to be written like this "NAME:CHARFIELD SIZE"
```
;;DEFINITION;;
    NAME:test
    MODE:file
    ;;FIELDS;;
        ID:CHARFIELD 8
        DATE:CHARFIELD 64
        MESSAGE:CHARFIELD 64
    ;;END_FIELDS;;
;;END_DEFINITION;;
```

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
