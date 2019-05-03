[![Build Status](https://travis-ci.org/SoPra-Team-10/Server.svg?branch=master)](https://travis-ci.org/SoPra-Team-10/Server)
# Server
Server component for the Quidditch Game.

## Getting started
You can choose between using Docker or manually installing all dependencies.
Docker is the preferred method as it already installs the toolchain
and all dependencies.

### Docker
In the root directory of the project build the docker image
("server" is the name of the container, this can be replaced by a
different name):
```
docker build -t server .
```

Now start the container, you need to map the internal port
(8080 by default, to some external port 80 in this case):
```
docker run -p 80:8080 server
```
That's it you should now have a running docker instance.

### Manually installing the Server
If you need to debug the server it can be easier to do this outside
of docker.

### Prerequisites
 * A C++17 compatible Compiler (e.g. GCC-8)
 * CMake (min 3.10) and GNU-Make
 * Adress-Sanitizer for run time checks
 * [SopraNetwork](https://github.com/SoPra-Team-10/Network)
 * [SopraGameLogic](https://github.com/SoPra-Team-10/GameLogic)
 * [SopraMessages](https://github.com/SoPra-Team-10/Messages)
 * Either a POSIX-Compliant OS or Cygwin (to use pthreads)
 * Optional: Google Tests and Google Mock for Unit-Tests

### Compiling the Application
In the root directory of the project create a new directory
(in this example it will be called build), change in this directory.

Next generate a makefile using cmake:
```
cmake ..
```
if any error occurs recheck the prerequisites. Next compile the program:
```
make
```
you can now run the server by executing the created `Server` file:
```
./Server
```

## Log-Levels
| Log-Level | Color | Explanation |
| --- | --- |
| 0 | - | No log messages |
| 1 | Red | Only error messages |
| 2 | Yellow | Error messages and warning |
| 3 | Blue | Error messages, warning and info messages |
| 4 | White | All messages (error, warning, info and debug) |

## External Librarys
 * [SopraNetwork](https://github.com/SoPra-Team-10/Network)
 * [SopraGameLogic](https://github.com/SoPra-Team-10/GameLogic)
 * [nlohmann::json](https://github.com/nlohmann/json)
