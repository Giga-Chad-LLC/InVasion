# Instructions on environment setting.

#### Setup

Choose a directory to hold locally installed packages. This page assumes that
the environment variable `MY_INSTALL_DIR` holds this directory path. For
example:

```sh
$ export MY_INSTALL_DIR=$HOME/.local
```

Ensure that the directory exists:

```sh
$ mkdir -p $MY_INSTALL_DIR
```

Add the local `bin` folder to your path variable, for example:

```sh
$ export PATH="$MY_INSTALL_DIR/bin:$PATH"
```

#### Install cmake

You need version 3.13 or later of `cmake`. Install it by
following these instructions:

- Linux

  ```sh
  $ sudo apt install -y cmake
  ```

- macOS:

  ```sh
  $ brew install cmake
  ```



Check the version of `cmake`:

```sh
$ cmake --version
```

Under Linux, the version of the system-wide `cmake` can often be too old. You
can install a more recent version into your local installation directory as
follows:

```sh
$ wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v{{< param cmake-version >}}/cmake-{{< param cmake-version >}}-Linux-x86_64.sh
$ sh cmake-linux.sh -- --skip-license --prefix=$MY_INSTALL_DIR
$ rm cmake-linux.sh
```

#### Install other required tools

Install the basic tools required to build gRPC:

- Linux

  ```sh
  $ sudo apt install -y build-essential autoconf libtool pkg-config
  ```

- macOS:

  ```sh
  $ brew install autoconf automake libtool pkg-config
  ```

#### Clone the `grpc` repo

Clone the `grpc` repo and its submodules:

```sh
$ git clone --recurse-submodules -b v1.43.0 https://github.com/grpc/grpc
```
#### Build and install gRPC and Protocol Buffers

The following commands build and locally install gRPC and Protocol Buffers:

```sh
$ cd grpc
$ mkdir -p cmake/build
$ pushd cmake/build
$ cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
$ make -j
$ make install
$ popd
```
if there are problems or freezes when calling the **{make -j}** command, try replacing it with the **{make -j4}** command

We **strongly** encourage you to install gRPC _locally_ &mdash; using an
appropriately set `CMAKE_INSTALL_PREFIX` &mdash; because there is no easy way
to uninstall gRPC after you've installed it globally.



###  Build the files

Open the code editor, and compile cmake file `hw_grpc_proto`. Temporary files will appear in the cmake-build-debug folder, now compile the `client` and `server` file via cmake. Everything is ready, you can start!




##### P.S
**Getting build failures?** Most issues, at this point, are a result of a
faulty installation. Ensure that the have the right versions of `cmake`, and
carefully recheck your installation.


