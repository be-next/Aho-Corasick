# Aho & Corasick - C++ implementation

[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/be-next/Aho_Coratchick/blob/main/LICENSE)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/038495b66f4c42b599a66566fd6f1b83)](https://app.codacy.com/gh/be-next/Aho-Corasick?utm_source=github.com&utm_medium=referral&utm_content=be-next/Aho-Corasick&utm_campaign=Badge_Grade_Settings)

![GitHub last commit](https://img.shields.io/github/last-commit/be-next/Aho-Corasick)

## Introduction

Easy to read implementation of Aho & Corasick algorithm.

## Usage

```cpp
 LexicographicTree lt;
 
 lt.AddWord( "a" );
 lt.AddWord( "ab" );
 lt.AddWord( "bab" );
 lt.AddWord( "bc" );
 lt.AddWord( "bca" );
 lt.AddWord( "c" );
 lt.AddWord( "caa" );
```

### How to visualize lexicographic tree

```sh
> dot -Tpng graph.plot > graph.png
```

## Further reading

- [Aho-Corasick white paper](http://cr.yp.to/bib/1975/aho.pdf), from Bell technologies
