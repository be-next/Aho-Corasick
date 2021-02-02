# Aho & Corasick - C++ implementation

[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/be-next/Aho_Coratchick/blob/main/LICENSE)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/038495b66f4c42b599a66566fd6f1b83)](https://app.codacy.com/gh/be-next/Aho-Corasick?utm_source=github.com&utm_medium=referral&utm_content=be-next/Aho-Corasick&utm_campaign=Badge_Grade_Settings)

![GitHub last commit](https://img.shields.io/github/last-commit/be-next/Aho-Corasick)

## Introduction

There are a lot of Aho & Corasick algorithm implementations in many langages.
Easy to read and easy to use implementation of Aho & Corasick algorithm.

## Why another Aho & Corasick implementation ?

- Function to convert trie in DataViz graph file. This file can be converted in picture.

## Usage

```cpp
 aho_corasick::LexicographicTree lt;
 
 lt.addKeyword( "a" );
 lt.addKeyword( "ab" );
 lt.addKeyword( "bab" );
 lt.addKeyword( "bc" );
 lt.addKeyword( "bca" );
 lt.addKeyword( "c" );
 lt.addKeyword( "caa" );

 lt.finalize();
```

### How to visualize lexicographic tree

```sh
> dot -Tpng graph.plot > graph.png
```

To view [GraphViz](http://www.graphviz.org/) diagrams online, visit this project and try to [use GraphvizOnline online here](http://dreampuf.github.io/GraphvizOnline/).

## Further reading

- [Aho-Corasick white paper](http://cr.yp.to/bib/1975/aho.pdf), from Bell technologies
