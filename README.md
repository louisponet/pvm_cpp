# pvm_cpp

My personal [pvm](https://en.wikipedia.org/wiki/Parallel_Virtual_Machine)-like playground.
The idea is that a network of machines / processes can be controlled to load data and be passed plugin-like dynamic libraries with functions to be executed on the loaded data.
The main observation to be explored is that it is easier to pass code around rather than data, i.e. that the different pvm instances can load their local data and receive the code to be ran on that data.

# Build
`bazel` is used as the build framework.
The code can be built by running first `bazel build //plugins/0:plugin_0` to build an example plugin followed by `bazel build //src:main` to build the main executable.
The executable can then be found in `bazel-bin/pvm_cpp/main`. Change `src/main.cpp` according to your setup.
