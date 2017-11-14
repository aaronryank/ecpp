<h3 align="center">Extended C Preprocessor</h1>

<h3 align="center">
 <a href="https://travis-ci.org/aaronryank/ecpp">
   <img src="https://travis-ci.org/aaronryank/ecpp.svg?branch=master" />
 </a>
 <a href="LICENSE">
   <img src="https://camo.githubusercontent.com/6def34e1aa4e2e9e81448c8a57cf3e09d8af28cf/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f6c6963656e73652d4253442d626c75652e737667" />
 </a>
 <a href="https://codecov.io/github/aaronryank/ecpp?branch=master">
   <img src="https://codecov.io/github/aaronryank/ecpp/coverage.svg?branch=master" />
 </a>
 
</h1>

The Extended C Preprocessor is a preprocessor for C/C++ that is capable of adding operators, directly replacing text, and modifying syntax. It has a built-in uncommenter and C prettyprinter.

## Docs

To read the "docs", head over to the [wiki](//github.com/aaronryank/ecpp/wiki).

## Requirements

ECPP requires:

 - GNU Make
 - GCC

## Building

To build, run `make`, To build the uncommenter alone, run `make uncomment`. If you also want to individually use the prettyprinter, run `make pretty`.

## Running

To run ecpp with the uncommenter and prettyprinter, run `out/ecpp sourcefile`.

To run the uncommenter only, run `out/uncomment sourcefile`.

To run the prettyprinter only, run `out/pretty < sourcefile`.

## Installing

To install on your system, run `./install /path/to/installation/directory`. It will:

 - Build ecpp, the prettyprinter, and the uncommenter (cleanly)
 - Copy all the output executables to the specified installation directory
 - Add the installation directory to the `PATH` environment variable
 - Create a rule to add the installation directory to `$PATH` in the current user's `.bashrc` (or `.bash_profile`)
