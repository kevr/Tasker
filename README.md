tasker
======

[![Test Coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/kevr/98692fb94e55e0667afb58f20b67f3f2/raw/tasker__heads_master.json)](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/kevr/98692fb94e55e0667afb58f20b67f3f2/raw/tasker__heads_master.json)

A task management program which interfaces with various services
and linux task formats.

See our MIT [LICENSE](LICENSE) file.

## Dependencies

- `-std=c++20` supporting C++ compiler
- [meson](https://github.com/mesonbuild/meson) build framework
- [gtest](https://github.com/google/googletest) test framework subproject
    - Fetched and built via `meson` when you configure and build the project.

## Build

See [Dependencies](#dependencies) for required prerequisite software.

First, configure a build directory:

    $ meson build

You can customize some build configuration:

    # Set a version
    $ meson -Dversion="1.6.0" build

<span id="test-cov-config">You can configure meson to enable test coverage [1]:</span>

    # Enable test coverage
    $ meson -Db_coverage=true build

Enter into the new `build` directory and compile the project:

    $ cd build
    $ ninja

## Running

Once you've [built](#build) the project, you can run the
program directly:

    $ cd build
    $ ./src/tasker

## Testing

Coverage support depends on [gcovr](https://github.com/gcovr/gcovr) and/or
[lcov](https://github.com/linux-test-project/lcov).

If you want code coverage, [configure meson to enable test coverage](#test-cov-config) [1].

After [Building](#build), you can run tests:

    $ ninja test

After running tests, you can produce coverage:

    # Generate gcovr report
    $ ninja coverage-text && cat meson-logs/coverage.txt

    # Generate lcov report
    $ ninja coverage-html
