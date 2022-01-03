# Jazzed up Kaleidoscope

The LLVM website contains a pretty cool 
(tutorial)[https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html]
demonstrating how to create a compiler for a toy language they call 
Kaleidoscope.

Thanks to LLVM, the code generation and optimization is actually quite easy,
and a large chunk of the tutorial focuses on creating a parser for the 
Kaleidoscope language, which is kind of a separate topic form using LLVM. 
On top of this, the tutorial doesn't follow software development best 
practices, and even has a warning at the start not to do things the way they
are demonstrated in the tutorial.

This repo is an effort to tidy up the implementation of Kaleidoscope up a bit. 
Once completed it should implement the following changes:
 * Remove global variables 
 * Separate the AST from the code generation code
 * Add some unit tests
 * Add a build script
 * Use a parser generator instead of writing the parser from scratch. 


