# Fletcher : A framework to integrate FPGA accelerators with Apache Arrow

[![Build Status](https://travis-ci.com/abs-tudelft/fletcher.svg?branch=develop)](https://travis-ci.com/abs-tudelft/fletcher)
[![Pipeline Status](https://gitlab.com/mbrobbel/fletcher/badges/master/pipeline.svg)](https://gitlab.com/mbrobbel/fletcher/commits/master)
[![License](https://badgen.net/github/license/abs-tudelft/fletcher)](https://github.com/abs-tudelft/fletcher/blob/develop/LICENSE)
[![Last Commit](https://badgen.net/github/last-commit/abs-tudelft/fletcher/develop)](https://badgen.net/github/last-commit/abs-tudelft/fletcher/develop)

Fletcher is a framework that helps to integrate FPGA accelerators with tools and frameworks that use Apache Arrow in 
their back-ends.

[Apache Arrow](https://arrow.apache.org/) specifies an in-memory format for data and provides libraries to various 
languages to interface with the data in that format. The format prevents the need for serialization and through the
libraries Arrow, zero-copy interprocess communication is possible. Languages that have Arrow libraries (under 
development) include C, C++, Go, Java, JavaScript, Python, Ruby and Rust.

While many software projects can benefit from these advantages, also hardware accelerated applications have seen 
serious serialization bottlenecks. Fletcher focuses on FPGA accelerators. Through Fletcher and Arrow, interfacing
efficiently between FPGA accelerator and language runtime is made available to all the supported languages.

Given an Arrow Schema (description of a tabular datastructure), Fletcher generates the following:

* An **easy-to-use hardware interface** for the functional part of the
accelerator:
  * You provide a range indices of your Arrow RecordBatch (rather than byte address)
  * You receive streams of the data-type specified by the schema (rather than bus words)
  * No pointer arithmetic, reordering, buffering, etc.. required - Fletcher does this for you, with high throughput.
* A **template** for the functional part of accelerator (to be implemented manually or using high-level synthesis)
  * You connect directly to streams of data from/to your RecordBatch rather than some memory bus interface.
    
![](fletcher.svg)

## Current state
Our framework is functional, but in early stages of development.

Especially the development branch (which is currently our main branch) is very active and may break without notice.
The (platform-specific) examples are quite hard to integrate in a CI pipeline (they would take multiple days to 
complete and would incur significant costs for platforms such as Amazon's EC F1), so it's best to test them out using 
the [master branch](https://github.com/abs-tudelft/fletcher/tree/master). 
The master branch can be rather old, but will be in a state where all examples should work on all supported platforms. 

### Apache Arrow support
* Fletcher currently supports:
  * Reading/writing an Arrow Schema created from any combination of:
    - Fixed-width primitives
    - Lists
    - Structs
* Validity bitmaps.

* We are working on support for:
  - Sparse and dense unions
  - Dictionaries
  - Chunked tabular structures (`Arrow::Table`)

### Platform support
* Our __core hardware descriptions__ are vendor independent; __we don't use any
  vendor IP__.
  * You can simulate a Fletcher based design without a specific target platform.
  * Tested simulators include Questa/Modelsim, GHDL and XSIM.
  
* We provide top-level wrappers for the following platforms:
  * [Amazon EC2 F1](https://github.com/aws/aws-fpga)
  * [OpenPOWER CAPI SNAP](https://github.com/open-power/snap)
  * Our top-level can be generated to speak AXI, so it should be easy to integrate with many existing systems that provide:
    * AXI4 (full) interface to memory, and 
    * AXI4-lite interface for MMIO.

## Further reading
  * [Getting Started: Simple Column Sum Example](examples/sum/README.md)
  * [Fletcher wrapper generator](codegen/fletchgen) - The wrapper generator converts a set of Arrow schemas to a 
    Fletcher-based design. It also provides instantiation templates for your kernel for a HDL or HLS (under 
    construction) design flow.
  * [Host-side run-time libraries](runtime) - Software libraries for run-time on the host-machine.
  * [Hardware library](hardware) - Fletcher core hardware components that are not generated by Fletchgen.

## Example projects
  * [Getting Started: Simple Column Sum Example](examples/sum/README.md)  
  * [Regular Expression Matching](examples/regexp/README.md) (outdated)
  * [String writer](examples/stringwrite) (outdated)
  * [K-Means clustering](examples/k-means) (outdated)

External projects using Fletcher:
  * [Posit BLAS operations](https://github.com/lvandam/posit_blas_hdl) (outdated)
  * [Posit PairHMM](https://github.com/lvandam/pairhmm_posit_hdl_arrow) (outdated)
