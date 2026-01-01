# The "C with modules" programming language

## Intro
I **LOVE** C, but i still see lots of problems in it. So I'm creating a language (to learn how to create language) and i have ideas what language it will be, and language shouldn't be too hard to implement (at list, not that hard as OOP abstract language).

And main idea of that language: *Extend `C` with modules, and some other ideas, but **save** `C` style, bcs C is GOAT.*

My **Reasons** to create that:
1. everything in "What i don't like in C?"
2. i want to learn how to make languages

### What i don't like in C?

1. Headers, every who programed in C or C++ in big codebases understand downgrade (slow compilation, really not save, copy-pasting is not cool).
2. Type syntax - in C it is not realy good, in [`docs/lang/types_syntax.md`](./docs/lang/types_syntax.md) i show my idea, and the reason is constance and understatement.
3. It just have lots of thing that is outdated.

## Quick Links
- [Task List](./docs/project/tasks/current.md)
- [Architecture Overview(pseudocode)](./docs/api/architecture.txt)
- [Language Documentation](./docs/lang/)
- [Build](#build)

## Project state
Currently project is in early development. it cannot compile or execute code yet.
I've implement Lexer, Parser, common stuff (details [here](./docs/project/tasks_archive.md) and [here](./docs/project/tasks.md));
Pipeline, ClI-app, and Semantic is our current target to implement.

##  How to navigate over the project

### Docs
in every docs directory placed readme file, that contains main ideas of that dir, where what is stored, what contains etc. so starting from `docs` dir as root, you can fast find what you want.

e.g. in `docs/` placed readme file, that contains `Why here's that dirs, and what in it?`, and then in every dir readme about it.

### CodeBase

I only started to write doxygen docs, so mostly code documented in it in comments, i hope  (can not be sure) you will understand it :)

## How to help project

Right now project on that stage, where you can help. There are tasks in `/docs/project/` directory, so you can chose, implement and then pull-request (i will be happy) <3
(BUT in project still deficit of documentation, help will be kinda hard, and i on the way to fix it soon)

## Build

We use `CMake`, and for POSIX system's recommend `project.sh` script.

### Requirements
- CMake
- C++23
#### optional:
- doxygen for documentation generation.

### POSIX-os (Linux, MacOS, FreeBSD, etc)

**(tested only on linux (arch BTW))**

JUST RUN `./project.sh build` then `./project.sh run` to run.

### Windows
use CMake and generate .sln files, for VisualStudio. here there isn't copy-paste command, bcs project is writing on linux, and windows not target yet.
