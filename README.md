# Lichens - CPP

This is source library for generic Lichens CPP code that can be reused into different projects. 

## CLI 

More information on the CLI can be found inside the justfile and can be use with the 
https://github.com/casey/just tools.

## Sharing source

The source code should remain proprietary to Lichens, some right to use and modify can be 
provided to clients upon contract and should be verified before handing this source code to other
entity.

## Contributions Rules

### Visual Studio Code

The source and tests should work into Visual Studio Code with the following extensions:
- Dev container

The other extensions and tools will be install using the `dev container` file `.devcontainer/devcontainer.json`. You might need the same package as the `.devcontainer/Dockerfile` to compile depending on the option you activate. 

### Coding requirements

- Ensure to use a namespace to avoid name clash
- Make function as portable as possible.
