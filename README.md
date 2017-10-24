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

To run ecpp with the uncommenter and prettyprinter, run `out/ecpp sourcefile`.

To run the uncommenter only, run `out/uncomment sourcefile`.

To run the prettyprinter only, run `out/pretty < sourcefile`.

# Installing

To install on your system, run `./install /path/to/installation/directory`. It will:

 - Build ecpp, the prettyprinter, and the uncommenter (cleanly)
 - Copy all the output executables to the specified installation directory
 - Add the installation directory to the `PATH` environment variable
 - Create a rule to add the installation directory to `$PATH` in the current user's `.bashrc` (or `.bash_profile`)
