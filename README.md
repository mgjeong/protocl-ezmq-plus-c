# ezmq plus library (c)

protocol-ezmq-plus-c is wrapper library which is written on the top of [protocol-ezmq-plus-cpp](https://github.sec.samsung.net/RS7-EdgeComputing/protocol-ezmq-plus-cpp), that provides a standard messaging interface over various data streaming
and serialization / deserialization middlewares along with some added functionalities.</br>

## Prerequisites ##

- SCons
  - Version : 2.3.0 or above
  - [How to install](http://scons.org/doc/2.3.0/HTML/scons-user/c95.html)

- protocol-ezmq-plus-cpp
  - Since protocol-ezmq-plus-cpp will be downloaded and built when protocol-ezmq-plus-c is built, check the prerequisites of it.
  - [Prerequisites](https://github.sec.samsung.net/RS7-EdgeComputing/protocol-ezmq-plus-cpp)

## How to build ##
1. Goto: ~/protocol-ezmq-plus-c/
2. Run the script:

   ```
   ./build.sh <options>       : Native build for x86_64 with default flags
   ./build_arm.sh <options>   : Native build for armhf [Raspberry pi board])
   ./unittests.sh             : Native unit tests build for x86_64 [It will first build ezmq-plus-c library after that unittests]
   ./unittest_arm.sh          : Native unit tests build for armhf [It will first build ezmq-plus-c library after that unittests]
   ```

**Notes** </br>
(a) For getting help about script flags [ build_common.sh ] : **$ ./build_common.sh --help** </br>

## How to run ##

### Prerequisites ###
 Built c ezmq plus library.

### Subscriber sample ###
1. Goto: ~/protocol-ezmq-plus-c/out/linux/{ARCH}/{MODE}/samples/
2. export LD_LIBRARY_PATH=../extlibs
3. Run the sample:
    ```
     ./amlsubscriber
    ```
    - **Enter topic to subscribe.** </br>

### Publisher sample ###
1. Goto: ~/protocol-ezmq-plus-c/out/linux/{ARCH}/{MODE}/samples/
2. export LD_LIBRARY_PATH=../extlibs
3. Run the sample:
   ```
   ./publisher
   ```
    - **Enter topic for publishing data.** </br>

## Usage guide for c ezmq plus library (for microservices)

1. The microservice which wants to use c ezmq plus APIs has to link following libraries:</br></br>
   **(A) If microservice wants to link c ezmq plus dynamically following are the libraries it needs to link:**</br>
        - cezmq_plus.so</br>
        - ezmq.so</br>
        - ezmq_plus.so </br>
        - aml.so</br>
        - caml.so </br>
2. Reference cezmqX library APIs : [docs/docs/html/index.html](docs/docs/html/index.html)
