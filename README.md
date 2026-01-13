# Lichens - CPP

This is source library for generic Lichens CPP code that can be reused into different projects. 

## Usage

There is an examples folder to display some integration with your project.

## CLI 

More information on the CLI can be found inside the justfile and can be use with the 
[just](https://github.com/casey/just) tools. You can compile, test or run example from the just cli.

## Sharing source

The source code is copyright to Lichens, but it is distributed as MIT license and user can use it under the MIT license term.

## Contributions Rules

### Visual Studio Code

The source and tests should work into Visual Studio Code with the following extensions:

- Dev container

The other extensions and tools will be installed using the `dev container` file `.devcontainer/devcontainer.json`. You might need the same package as the `.devcontainer/Dockerfile` to compile depending on the option you activate.

### Coding requirements

- Ensure to use a namespace to avoid name clash
- Make function as portable as possible.
