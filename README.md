# Algorithmic-Trading-Platform
PC01: Algorithmic Trading Platform

## Submodule Setup
This project uses the submodule [DataFrame](https://github.com/hosseinmoein/DataFrame). To clone the project with submodules, use the following command:
```
git clone --recurse-submodules https://github.com/Chahel28/Algorithmic-Trading-Platform.git
```
Alternatively, if you already cloned the project without submodules, to initialize and update the submodules, use the following commands in the project directory:
```
git submodule update --init --recursive
```

## Requirements
- cmake
- make
- gcc

## Build and Run
Use `build.sh` for UNIX and `build.bat` for Windows to build the project. The executables will be in the `build` folder.
