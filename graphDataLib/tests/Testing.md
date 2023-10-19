# C++ Testing
At the time of this writing, no formal testing framework is used for this project.

The basic test is composed of the following:

1. Code for a main executable in a `category` folder
2. A `tests/<category>/CMakeLists.txt` file to build the required test executable
3. Entry in the `tests/CMakeLists.txt` file adding that executable as a test
4. Target and output settings to place the compiled executable in `tests/bin`


```markdown
📋 tests
   │
   ├─📂💽 bin
   ├─📂🗃️ data
   ├─📂⚒️ crud
   ├─🗒️ Testing.md
   └─📜 CMakeLists.txt
```
The basic types are indicated by the icons:

* 📂💽 Test executable folder
* 📂⚒️ Test code folder
* 📂🗃️ Test data folder
* 📜 CMake build file

Both the `tests/bin` and `tests/data` folder are created at build time.  Test data is downloaded from
the appropriate S3 bucket by CMake

## Graph Data Native API Tests
This is presented as an example structure, rather than a requirement.

Using the `LibFunctionTest` executable as an example, here is how it's set up to run.  The first part is the layout of the testing code.  The overall structure in the `tests/UnitTestA` folder gives us this setup:

```markdown
🎓 tests
├─📂⚒️ ...snip...
└─📂⚒️ UnitTestA
      ├─📜 CMakeLists.txt
      ├─💽 LibFunctionTest.cpp
      └─💽 MainOnly.h
```

As you can probably surmise, the `tests/UnitTestA/CMakeLists.txt` file handles all the build requirements for that particular set of tests.

Next, there is the code.  The `LibFunctionTest` test is a straightforward executable that creates a vector<string> and passes it to the library function.

```c++
//
// Created by david on 9/12/23.
//
#include <string>
#include <iostream>
#include <vector>
#include "MainOnly.h"
#include <graphData.h>

using namespace std;

int main() {

    vector<string> names;

    names.push_back("Dopey");
    names.push_back("Sneezy");
    names.push_back("Grumpy");
    names.push_back("Doc");
    try {
        ml_lib_function(names);
    } catch (...) {
        cerr << "An error occurred while performing the operation." << endl;
        return -1;
    }
    cout << "The geodatabase has been created." << endl;
    return 0;
}

```

For this test, the native library operation is called, and the continuation or error exit are returned.

The appropriate entries are made in `tests/UnitTestA/CMakeLists.txt` to build this test:

```cmake
add_executable(LibFunctionTest
        LibFunctionTest.cpp
)

target_link_libraries(LibFunctionTest
        ${PROJECT_NAME}
)
```

And finally, the testing itself is added to `tests/CMakeLists.txt`:

```cmake
add_test(NAME LibFunctionTest COMMAND "LibFunctionTest"
        WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
```

With all of this in place, running `./buildLinux.sh -t` will automatically build and run the tests, producing the following output:

```shell
>> ./buildLinux.sh -t
-- Configuring for Linux
-- Setting up link libraries for Linux
-- Populating testdata

  ...snip...

[100%] Built target LibFunctionTest
Running tests...
Test project /devel/graphData/graphData/build-linux-x64
    Start 1: LibFunctionTest
1/1 Test #1: LibFunctionTest...................   Passed    0.04 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.04 sec
```

To add more tests, simply make the appropriate additions to the `UnitTestA` folder and `CMakeLists.txt` files, or create a new
folder with the specific test requirements.
