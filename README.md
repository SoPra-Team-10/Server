[<img src="https://travis-ci.org/SoPra-Team-10/Server.svg?branch=master" alt="Build Status">](https://travis-ci.org/SoPra-Team-10/Server)
# Server
Server component for the Quidditch Game.

## Usage
In accordance to the standard, it is possible to change some options of the server using command line arguments.
For the server the following flags exist:
* `-m` / `--match` path to the MatchConfig JSON file (mandatory)
* `-h` / `--help` print the help page and exit (optional)
* `-p` / `--port` set the port (the port needs to be larger `0` and smaller `65536`) (optional, the default value is `8080`)
* `-v` / `--verbosity` change the verbosity level, for more information on log-levels see [SoPra-Team-10/Util](https://github.com/SoPra-Team-10/Util) (optional, the default value is `0`)

## Getting started
You can choose between using Docker or manually installing all dependencies.
Docker is the preferred method as it already installs the toolchain
and all dependencies.

### Docker

#### Prebuild Docker Image
Get a prebuild Docker Image [here](https://hub.docker.com/r/soprateamzehn/server) or run:
```
docker pull soprateamzehn/server
```

#### Build your own Docker Image from the provided Dockerfile
In the root directory of the project build the docker image
("server" is the name of the container, this can be replaced by a
different name):
```
docker build -t server .
```

Now start the container, you need to map the internal port
(8080 by default, to some external port 8080 as well in this case) and
map the external file (match.json) to an internal file:
```
docker run -p 8080:8080 server ./build/Server -m MatchConfigs/test.json -v 4 -p 8080
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
 * [SopraUtil](https://github.com/SoPra-Team-10/Util)
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



## External Librarys
 * [SopraNetwork](https://github.com/SoPra-Team-10/Network)
 * [SopraGameLogic](https://github.com/SoPra-Team-10/GameLogic)
 * [nlohmann::json](https://github.com/nlohmann/json)

## Doxygen Dokumentation
- [Master Branch](https://sopra-team-10.github.io/Server/master/html/index.html)
- [Develop Branch](https://sopra-team-10.github.io/Server/develop/html/index.html)

## SonarQube Analyse
Das Analyseergebniss von SonarQube ist [hier auf SonarCloud](https://sonarcloud.io/dashboard?id=SoPra-Team-10_Server) zu finden.
