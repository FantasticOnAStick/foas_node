# Node [![Build Status](https://travis-ci.org/FantasticOnAStick/foas_node.svg?branch=master)](https://travis-ci.org/FantasticOnAStick/foas_node)

Main node code that serves as a runner application for all added functionality. This code serves as a main entry point for other foas components:

* foas_plugin: Plugin loader
* foas_message: Message bus
* foas_common: Common library



## Next things on the list

* Introducing service concept: Request, reply
* Network concept: Communication plugin, translation layer between native messages/services and network communication


## Getting the source, compiling it, running the project

This section describes -- step by step -- how to download, compile and run foas_node.


### Get all repositories

First, you have to get the base repository of `foas_node` and all its dependencies. Follow these steps to get everything set up:

1. Create a working directory where all `foas_*` repositories will reside:

```bash
mkdir foas
```

2. In that repository, `git clone` all required repositories:

```bash
git clone https://github.com/FantasticOnAStick/foas_node.git
git clone https://github.com/FantasticOnAStick/foas_common.git
git clone https://github.com/FantasticOnAStick/foas_message.git
git clone https://github.com/FantasticOnAStick/foas_plugin.git
git clone https://github.com/FantasticOnAStick/plugins.git
```


### Compiling the project

Go into the `foas_node` directory, set up its build environment and compile it:

```bash
cd foas_node
mkdir build
cd build
cmake ..
make
```


### Run the main executable

The main program resides in `foas_node/bin` and is called `foas_node-bin`. Assuming you are still in the `build` directory, run it like this:

```bash
./../bin/foas_node-bin
```
