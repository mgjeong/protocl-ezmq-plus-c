# ezmq plus library (c)

protocol-ezmq-plus-c is wrapper library which is written on the top of [protocol-ezmq-plus-cpp](https://github.sec.samsung.net/RS7-EdgeComputing/protocol-ezmq-plus-cpp), that provides a standard messaging interface over various data streaming and serialization / deserialization middlewares along with some added functionalities</br>
  - Currently supports streaming using 0mq and serialization / deserialization using protobuf
  - Publisher -> Multiple Subscribers broadcasting
  - High speed serialization and deserialization
  - Topic name discovery [TNS]
  - Automation Markup language [AML]


## Prerequisites ##
- SCons
  - Version : 2.3.0 or above
  - [How to install](http://scons.org/doc/2.3.0/HTML/scons-user/c95.html)
- protocol-ezmq-plus-cpp
  - Since [protocol-ezmq-plus-cpp](https://github.sec.samsung.net/RS7-EdgeComputing/protocol-ezmq-plus-cpp) will be downloaded and built when protocol-ezmq-plus-c is built, check the prerequisites of it. It can be installed via build option (See 'How to build')  
- datamodel-aml-c
  - Since [datamodel-aml-c](https://github.sec.samsung.net/RS7-EdgeComputing/datamodel-aml-c) will be downloaded and built when protocol-ezmq-plus-c is built, check the prerequisites of it. It can be installed via build option (See 'How to build')  

## How to build ##
1. Goto: ~/protocol-ezmq-plus-c/
2. Run the script:

   ```
   ./build.sh <options>        : Native build for x86_64 with default flags
   ./build_arm.sh <options>    : Native build for armhf [Raspberry pi board]) with default flags
   ./unittest.sh <options>     : Native unit tests build for x86_64
   ./unittest_arm.sh <options> : Native unit tests build for armhf
   ```

**Note:** </br>
1. For getting help about script option: **$ ./build_common.sh --help** </br>
2. If building for first time, use **--with_dependencies=true** flag.</br>
3. Unit test script should be invoked with **--with_dependencies=true** flag, if ezmq-plus library is not built already in **RELEASE** mode.

## How to run ezmq-plus samples [binary/executables] ##
ezmq-plus has publisher, amlsubscriber, xmlsubscriber and topic-discovery sample applications. Build and run using the following guide to experiment different options in sample.

### Prerequisites ###
 Built ezmq-plus library

### Topic Discovery sample ###
1. Goto: ~/protocol-ezmq-plus-c/out/linux/{ARCH}/{MODE}/samples/
2. export LD_LIBRARY_PATH=../extlibs/
3. Run the sample:
    ```
    $ ./topicdiscovery
    ```
**Note:** It will give list of options for running the sample. 

### Publisher sample ###
1. Goto: ~/protocol-ezmq-plus-c/out/linux/{ARCH}/{MODE}/samples/
2. export LD_LIBRARY_PATH=../extlibs/
3. Run the sample:
    ```
    $ ./publisher
    ```
**Note:** It will give list of options for running the sample. 

### AML Subscriber sample ###
1. Goto: ~/protocol-ezmq-plus-c/out/linux/{ARCH}/{MODE}/samples/
2. export LD_LIBRARY_PATH=../extlibs/
3. Run the sample:
    ```
    $ ./amlsubscriber
    ```
**Note:** It will give list of options for running the sample.  

### XML Subscriber sample ###
1. Goto: ~/protocol-ezmq-plus-c/out/linux/{ARCH}/{MODE}/samples/
2. export LD_LIBRARY_PATH=../extlibs/
3. Run the sample:
    ```
    $ ./xmlsubscriber
    ```
**Note:** It will give list of options for running the sample. 

### Generate the code coverage report

### Prerequisites ###
 Built ezmq-plus library in debug mode.

### Generate report ###
1. Goto `~/protocol-ezmq-plus-c/` </br>
2. Run the script:
   ```
   $ ./generate_report.sh     : Script to generate code coverage and unit test report.
   ```
**Note:** </br>
For getting help about script options: **$ ./generate_report.sh --help** </br>

## Usage guide for c ezmq plus library (for microservices)

1. The microservice which wants to use c ezmq plus APIs has to link following libraries:</br>
   **(A) If microservice wants to link c ezmq plus dynamically following are the libraries it needs to link:**</br>
        - cezmq_plus.so</br>
        - ezmq.so</br>
        - ezmq_plus.so </br>
        - aml.so</br>
        - caml.so </br>
2. Reference cezmqX library APIs : [Documentation](docs/doxygen/docs/html/index.html)
3. Topic naming convention guide : [Naming Guide](https://github.sec.samsung.net/RS7-EdgeComputing/protocol-ezmq-plus-cpp/blob/master/TOPIC_NAMING_CONVENTION.md)
