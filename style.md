# Code styling

We use [Google C++ Style Guide] with several small changes.

Changes in the guide are:
  
  - Line length is 100
  - File extensions are .hh and .cc
  - Abbreviations are entirely capitalized (that means we use "SMG" in type/variable names, not "Smg")
  - Class definitions starts with private section, then protected section and public section is at the end
  - Pointer and reference symbols are aligned to the left (int* a;)
  - We use `#pragma once` as the header guard
  - `long` and `unsigned` types can be used
  - Names of constants are allowed to be in uppercase

Everything else is formatted according to the guide.

It is recommended that you use [cpplint] to check style of your source files.
Recommended syntax is following:
  ```sh
  cpplint --linelength=100 --extensions=cc,hh --filter=-legal,-runtime/int src/*/*
  ```
The most important rules are:
  
  - Indenting is done with 2 spaces
  - private, protected and public keywords are indented 1 space
  - Class and method (function) names are written in camel case and start with capital letter
  - Variable names are written in lowercase with underscores ('_') between words
  - Class member variables are written in same way, with an extra trailing underscore
  - Opening curly brace is on the same line as keyword, or function header
  - Order of includes is following: related header, C headers, C++ headers, other libraries headers, project headers
  - Project header files are listed as descendants of project source and surrounded with "" (eg. `#include "objects/SMGObject.hh"`)
   
[Google C++ Style Guide]: <https://google.github.io/styleguide/cppguide.html>
   [cpplint]: <https://pypi.python.org/pypi/cpplint>


