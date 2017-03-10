# evilibc

Work-in-progress C standard library implementation that aims to be as counterintuitive and underhanded as possible while still being technically correct (conforming to C11 standard).

## Why?

* To see what can one actually rely on when attempting to write portable code.
* To get to know libc better.
* Because [technically correct is the best kind of correct](https://www.youtube.com/watch?v=hou0lU8WMgo).
* To be able to say "I implemented libc once. It had an undefined behavior handler.".

## Contributing

If you want to be a part of the evil master plan:

* If you don't already have an evil idea in mind, take a look at TODO.md to find out what is not implemented yet.
* Make sure there is no Github issue regarding the task. If there is none, create one **before you start working**, to let everybody know what you're working on.
* Read [coding guidelines](#coding-guidelines) below.
* Read the C spec regarding the feature you want to implement.
  * Become a machine. Do not make assumptions.
  * Think of any non-obvious ways of interpreting the standard.
* Fork the repository, implement the code on your own fork. Mark the feature as complete in TODO.md.
* Create a pull request. Be prepared for more evil suggestions to come up.

## Coding guidelines

* Be technically correct.
* Be evil, unless this the spec prevents you from doing so.
* Document as much as possible with excerpts from the C11 spec (final draft is OK), including section numbers. Explain questionable decisions.
* In cases defined by the spec as undefined behavior, call `__evil_ub` with appropriate description. Similarly, where the behavior is implementation-defined, use `__evil_idb`.
* Do not implement stuff the spec explicitly marks as optional, unless some other spec makes it mandatory (e.g. POSIX). Use appropriate `#if`s in such cases.
* Assume GCC compiler. Use [GCC builtins](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html) where appropriate.
* When implementing OS-dependant stuff, assume Linux.
* Use C11.
* No external dependencies in the library. If there will be any tests in the future, dependencies are fine there.
* One folder under `src/` for each C header, one source file per public function, unless you have a good reason (e.g.: `ctypes.c` would be insane).
* Prefix static functions/variables with a single `_`. Use `__evil_` prefix for internal symbols visible across multiple C files.
* 4-space indent, no tabs, 80-character line limit. I'll probably prepare some clang-format config at some point.
