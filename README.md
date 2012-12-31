# Upcaste
------------ -------------- --------------
#### *Performance Libraries*

### Overview

Upcaste is a collection of optimized source libraries that provide a foundation
for graphical, visual, and heterogeneous computing using OpenCL. Upcaste is
released under the MIT License, with additional restrictions for certain
included components (see [`LICENSE.md`](https://github.com/upcaste/upcaste/blob/master/LICENSE.md)).

### Design

Upcaste has been heavily influenced by data-oriented and functional design
principles in order to maximize software performance. In essence of this,
it is written primarily in a "back-to-basics" subset of C++, which favors
explicit code using free functions and POD types over that of rich class
hierarchies. Think of it as C with stronger typing, namespaces, lambda
functions, compile-time templates, and a few other useful C++11 features. On
the rare occasion, classes and virtual member functions may be used in the
implementation of certain subsystems, but only because they happen to be the
best tool for the job in each particular case.

### Status

This project is in an early and experimental phase, and a lot its interfaces
are highly unstable.

Currently only the Upcaste core library, *upcore*, is in a semi-useable state
on Windows using MSVC++ 2012. Ports to other platforms and compilers is a
work-in-progress. The core library provides a portable wrapper layer for most
Standard C library facilities, some additional POSIX layer functions, and
other base level functions and data structures.

The *upsystem* library is currently in the planning stages and will provide
features for configuring and initializing an OpenCL / OpenGL / Direct3D
environment, for managing task-oriented flow control across compute devices,
and for high-performance filesystem streaming and data caching.
