# pvm_cpp

My personal [pvm](https://en.wikipedia.org/wiki/Parallel_Virtual_Machine)-like playground combined with an exploration of different HFT programming techniques.

The structure of the system is a network of machines / processes which can be controlled through a RestAPI to load data and be passed plugin-like dynamic libraries with functions to be executed on the loaded data.
The main observation to be explored is that it is easier/cheaper/faster to pass code around rather than data, i.e. that the different pvm instances can load their local data and receive the code to be ran on that data.

The design is shown in the graph below.
[!](design.drawio.svg)

# Build
`bazel` is used as the build framework.
The code can be ran by running first `bazel build //plugins/0:plugin_0` to build an example plugin followed by `bazel run //src:main` to build the main executable.
Use `bazel <build/run> -c opt` for an optimized build.
The executable can then be found in `bazel-bin/pvm_cpp/main`.
Change `src/main.cpp` according to your setup.

To run the benchmark of the `spmc_queue` performance you can run `bazel run -c opt //benchmarks:spmc_queue`.









