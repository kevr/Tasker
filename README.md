tasker
======

[![build](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/kevr/98692fb94e55e0667afb58f20b67f3f2/raw/tasker__heads_master_build.json)](https://github.com/kevr/tasker/actions/workflows/build.yaml)
[![test](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/kevr/98692fb94e55e0667afb58f20b67f3f2/raw/tasker__heads_master_test.json)](https://github.com/kevr/tasker/actions/workflows/test.yaml)
[![coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/kevr/98692fb94e55e0667afb58f20b67f3f2/raw/tasker__heads_master_coverage.json)](https://github.com/kevr/tasker/actions/workflows/test.yaml)

A task management program which interfaces with various services
and linux task formats.

See our MIT [LICENSE](LICENSE) file.

## Building

See [Build dependencies](#build-dependencies) for prerequisite software.

First, configure a build directory:

    $ meson build

You can customize some build configuration:

    # Set a version
    $ meson -Dversion="1.6.0" build

<span id="test-cov-config">
You can configure meson to <u>enable test coverage</u>:
</span>

    # Enable test coverage
    $ meson -Db_coverage=true build

If you're only building tests or the binary, you can tell `meson`:

    # Build tests with coverage enabled
    $ meson -Db_coverage=true -Dbuild_bin=false -Dbuild_tests=true build

    # Build only the binary without coverage enabled
    $ meson -Dbuild_tests=false -Dbuild_bin=true build

Enter into the new `build` directory and compile the project:

    $ cd build
    build$ ninja

## Running

See [Runtime dependencies](#runtime-dependencies) for prerequisite software.

Once you've [built](#building) the project, you can run the
program directly:

    build$ ./src/tasker

Each variable in [Configuration](#configuration) can be set on the
command line, as well as a few shell-specific options. See `--help`
for a complete breakdown:

    build$ ./src/tasker --help

## Configuration

See [doc/config.txt](doc/config.txt) for an example config which
contains documentation about config locations that tasker searches.

## Dependencies

- [Runtime dependencies](#runtime-dependencies) must be installed on any
  system wishing to run this program after compilation
- [Build dependencies](#build-dependencies) must be installed on any
  system meant to compile this program
- [Internal dependencies](#internal-dependencies) will be acquired and built
  via `meson` and do not require user management

#### Runtime dependencies

A user intending to run a compiled binary of this project requires
the following library dependencies:

- [ncurses](https://github.com/mirror/ncurses) for terminal user interface
  framework
- [boost::program_options](https://github.com/boostorg/boost) for symbols for
  command-line/config parser
- [boost::system](https://github.com/boostorg/boost) for symbols for system
utilities

#### Build dependencies

In addition to [Runtime dependencies](#runtime-dependencies), a user
intending to build this project requires the following dependencies:

- `-std=c++20` supporting C++ compiler
- [meson](https://github.com/mesonbuild/meson) build framework
- [git](https://github.com/git/git) source code management

#### Internal dependencies

*Internal dependencies* are dependencies which are fetched via `meson`
to build the project. Users don't have to worry about obtaining these
dependencies otherwise.

- [fmt](https://github.com/fmtlib/fmt) (static runtime) for complex string
  formatting
- [gtest](https://github.com/google/googletest) (shared runtime) test framework
  subproject

## Testing

Coverage support depends on [gcovr](https://github.com/gcovr/gcovr) and/or
[lcov](https://github.com/linux-test-project/lcov).

After [building tests](#building), you can run all of them with `ninja`:

    $ ninja test

After running tests from a
[build configured to enable test coverage](#test-cov-config),
you can produce coverage:

    # Generate gcovr report
    $ ninja coverage-text && cat meson-logs/coverage.txt

    # Generate lcov report
    $ ninja coverage-html
