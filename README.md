# Meager

Graph embeddings measurer - a tool for computing metrics for testing knowledge graph embedding models.

The tool borrows a significant portion of code from the [OpenKE](https://github.com/thunlp/OpenKE) repo.

## Building

To build the tool, execute the following command:

```sh
cmake . && make all
```

To install via cget (the lib file is saved as `/usr/lib/libmeager.so`) execute the following command:

```sh
sudo cget install zeionara/meager
``` 

