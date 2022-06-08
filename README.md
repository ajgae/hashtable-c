# C Hashtable implementation

This is a simple implementation of a string-to-string hashtable (both keys and values are strings). The hash function used is [FNV-1a](http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a). Collision resolution is done using linear probing.

## Compilation

```console
gcc -o a.out hashtable.c
```
