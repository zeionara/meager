# Meager

<p align="center">
    <img src="data/images/logo.png"/>
</p>

Graph embeddings measurer - a tool for computing metrics for testing knowledge graph embedding models.

The tool borrows some code and ideas from the [OpenKE](https://github.com/thunlp/OpenKE) repo.

## Installation

To install via cget (the lib file is saved as `/usr/lib/libmeager.so`) execute the following command:

```sh
sudo cget install zeionara/meager
``` 

## Building

To build the tool, execute the following command, first, create the build folder:

```sh
mkdir -p build/src
cd build/src
```

Then build `Makefile`:

```sh
cmake ../../src
```

Then build and install `.so` libraries:

```sh
sudo make install
```

## Testing

Create the build folder for tests:

```sh
mkdir -p build/test
cd build/test
```

After that, build `Makefile`:

```sh
cd build/test
cmake ../../test
```

Then build tests:

```sh
make
```

Then run tests:

```sh
ctest
```
