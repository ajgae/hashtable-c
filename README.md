# C Hashtable Implementation

This is a simple implementation of a string-to-string hashtable (both keys and values are strings). The hash function used is [FNV-1a](http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a). Collision resolution is done using linear probing.

## Compilation

`gcc -o a.out hashtable.c main.c`

## Usage

The user is presented with a simple prompt. A few commands can be issued. Arguments are space-separated, with no support for including space characters in arguments yet.

- `exit`: exit the program.
- `put`: adds the given key-value pair to the hashtable. Requires two arguments.
- `show`: display information about the hashtable, such as capacity, load factor, and existing key-value pairs.
- `hash`: display hash of first argument. If there is a second argument, and it is a valid positive non-zero integer, show hash of first argument modulo second argument.
