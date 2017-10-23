# Extended C Preprocessor

The Extended C Preprocessor is a preprocessor for C/C++ that is capable of adding operators, directly replacing text, and modifying syntax. It has a built-in uncommenter and C prettyprinter.

# Requirements

ECPP requires:

 - GNU Make
 - GCC (4.9 or higher)
 - A shell

# Building

To build, run `make`, then `make uncomment`. If you also want to individually use the prettyprinter, run `make pretty`.

# Running

To run, call `sh run\ecpp.sh` for Unix and `run\ecpp.bat` for Windows.

To run without the uncommenter, run `out\ecpp sourcefile <arguments...>`. To run the uncommenter only, run `out\uncomment sourcefile`.

To run the prettyprinter only, run `out\pretty < sourcefile`.