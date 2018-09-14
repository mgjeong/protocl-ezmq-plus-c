###############################################################################
# Copyright 2017 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###############################################################################

#!/bin/bash
set +e
#Colors
RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\033[0;34m"
NO_COLOUR="\033[0m"

PROJECT_ROOT=$(pwd)
DEP_ROOT=$(pwd)/dependencies
EZMQX_TARGET_ARCH="$(uname -m)"
EZMQX_WITH_DEP=false
EZMQX_BUILD_MODE="release"
EZMQX_LOGGING="off"
EZMQX_DISABLE_PROTOBUF=false
TARGET_ARCH="x86_64"

PROTOCOL_EZMQ_CPP_VERSION=v1.0_rc1
DATAMODEL_AML_C_VERSION=v1.0_rc1

RELEASE="1"
LOGGING="0"

install_dependencies() {
    if [ -d "./dependencies" ] ; then
        echo "dependencies folder exist"
    else
        mkdir dependencies
    fi

    cd $DEP_ROOT
    # Check/clone protocol-ezmq-plus-cpp
    if [ -d "protocol-ezmq-plus-cpp" ] ; then
        echo "protocol-ezmq-plus-cpp folder exist"
    else
        git clone git@github.sec.samsung.net:RS7-EdgeComputing/protocol-ezmq-plus-cpp.git
    fi
    git checkout ${PROTOCOL_EZMQ_CPP_VERSION}

    # build protocol-ezmq-plus-cpp
    cd $DEP_ROOT/protocol-ezmq-plus-cpp
    echo -e "${GREEN}Building ezmq-plus library and its dependencies${NO_COLOUR}"
    if [ "x86_64" = ${EZMQX_TARGET_ARCH} ]; then
         ./build.sh --with_dependencies=${EZMQX_WITH_DEP} --build_mode=${EZMQX_BUILD_MODE}
    elif [ "armhf" = ${EZMQX_TARGET_ARCH} ]; then
         ./build_arm.sh --with_dependencies=${EZMQX_WITH_DEP} --build_mode=${EZMQX_BUILD_MODE}
    else
         echo -e "${RED}ezmq-plus-cpp: Not a supported architecture${NO_COLOUR}"
         usage; exit 1;
    fi
    echo -e "${GREEN}Installation of ezmq-plus library and its dependencies done${NO_COLOUR}"

    cd $DEP_ROOT
    # Check/clone datamodel-aml-c
    if [ -d "datamodel-aml-c" ] ; then
        echo "datamodel-aml-c folder exist"
    else
        git clone git@github.sec.samsung.net:RS7-EdgeComputing/datamodel-aml-c.git
    fi
    git checkout ${DATAMODEL_AML_C_VERSION}

    TARGET_ARCH=${EZMQX_TARGET_ARCH}
    if [ "armhf" = ${TARGET_ARCH} ]; then
        TARGET_ARCH="armhf-native";
    fi  
    # build datamodel-aml-c
    cd $DEP_ROOT/datamodel-aml-c
    echo -e "${GREEN}Building datamodel-aml-c library and its dependencies${NO_COLOUR}"
    ./build_common.sh --target_arch=${TARGET_ARCH} --build_mode=${EZMQX_BUILD_MODE} --logging=${EZMQX_LOGGING} --disable_protobuf=${EZMQX_DISABLE_PROTOBUF}
    echo -e "${GREEN}Installation of datamodel-aml-c library and its dependencies done${NO_COLOUR}"
}

build_x86_64() {
    echo -e "Building for x86_64"
    scons TARGET_OS=linux TARGET_ARCH=x86_64 RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_armhf() {
    echo -e "Building for armhf"
    scons TARGET_ARCH=armhf RELEASE=${RELEASE} LOGGING=${LOGGING}
}

clean_ezmqx() {
    echo -e "Cleaning ${BLUE}${PROJECT_ROOT}${NO_COLOUR}"
    echo -e "Deleting  ${RED}${PROJECT_ROOT}/out/${NO_COLOUR}"
    echo -e "Deleting  ${RED}${PROJECT_ROOT}/.sconsign.dblite${NO_COLOUR}"
    rm -r "${PROJECT_ROOT}/out/" "${PROJECT_ROOT}/.sconsign.dblite"
    find "${PROJECT_ROOT}" -name "*.memcheck" -delete -o -name "*.gcno" -delete -o -name "*.gcda" -delete -o -name "*.os" -delete -o -name "*.o" -delete
    echo -e "Deleting  ${RED}${PROJECT_ROOT}/dependencies/${NO_COLOUR}"
    rm -rf ./dependencies
    echo -e "Finished Cleaning ${BLUE}${EZMQ}${NO_COLOUR}"
}

usage() {
    echo -e "${BLUE}Usage:${NO_COLOUR} ./build_common.sh <option>"
    echo -e "${GREEN}Options:${NO_COLOUR}"
    echo "  --target_arch=[x86_64|armhf]                      :  Choose Target Architecture"
    echo "  --with_dependencies=[true|false](default: false)  :  Build cezmq-plus with its dependency ezmq-plus and datamodel-aml-c"
    echo "  --build_mode=[release|debug](default: release)    :  Build in release or debug mode"
    echo "  -c                                                :  Clean ezmq-plus Repository and its dependencies"
    echo "  -h / --help                                       :  Display help and exit"
    echo -e "${GREEN}Notes: ${NO_COLOUR}"
    echo "  - While building newly for any architecture use --with_dependencies=true option."
}

build() {
    if [ "debug" = ${EZMQX_BUILD_MODE} ]; then
        RELEASE="0"
        EZMQX_LOGGING="on"
        LOGGING="1"
    fi

    if [ ${EZMQX_WITH_DEP} = true ]; then
        install_dependencies
    fi

    cd $PROJECT_ROOT
    if [ "x86_64" = ${EZMQX_TARGET_ARCH} ]; then
         build_x86_64;
    elif [ "armhf" = ${EZMQX_TARGET_ARCH} ]; then
         build_armhf;
    else
         echo -e "${RED}Not a supported architecture${NO_COLOUR}"
         usage; exit 1;
    fi
}

copy_required_libs() {
    echo -e "${BLUE}Copy required libraries to extlibs folder${NO_COLOUR}"
    cd $PROJECT_ROOT/out/linux/${EZMQX_TARGET_ARCH}/${EZMQX_BUILD_MODE}
    if [ -d "./extlibs" ] ; then
         echo "extlibs folder exist"
    else
         mkdir extlibs
    fi
    cd extlibs
    # copy ezmq_plus.so, ezmq.so, caml.so and aml.so to extlibs folder
    cp $PROJECT_ROOT/dependencies/datamodel-aml-c/out/linux/${EZMQX_TARGET_ARCH}/${EZMQX_BUILD_MODE}/libcaml.so .
    cp $PROJECT_ROOT/dependencies/datamodel-aml-c/dependencies/datamodel-aml-cpp/out/linux/${EZMQX_TARGET_ARCH}/${EZMQX_BUILD_MODE}/libaml.so .
    cp $PROJECT_ROOT/dependencies/protocol-ezmq-plus-cpp/out/linux/${EZMQX_TARGET_ARCH}/${EZMQX_BUILD_MODE}/libezmq_plus.so .
    cp $PROJECT_ROOT/dependencies/protocol-ezmq-plus-cpp/dependencies/protocol-ezmq-cpp/out/linux/${EZMQX_TARGET_ARCH}/${EZMQX_BUILD_MODE}/libezmq.so .
    echo -e "${BLUE}done${NO_COLOUR}"
}

copy_docker_script() {
    echo -e "${BLUE}Copy docker sample files to out${NO_COLOUR}"
    cd $PROJECT_ROOT/out/linux/${EZMQX_TARGET_ARCH}/${EZMQX_BUILD_MODE}/samples
    if [ -d "./docker" ] ; then
         echo "docker folder exist"
    else
         cp -r $PROJECT_ROOT/samples/docker .
    fi
    echo -e "${BLUE}done${NO_COLOUR}"
}

process_cmd_args() {
    if [ "$#" -eq 0  ]; then
        echo -e "No argument.."
        usage; exit 1
    fi

    while [ "$#" -gt 0  ]; do
        case "$1" in
            --with_dependencies=*)
                EZMQX_WITH_DEP="${1#*=}";
                if [ ${EZMQX_WITH_DEP} = true ]; then
                    echo -e "${BLUE}Build with depedencies${NO_COLOUR}"
                elif [ ${EZMQX_WITH_DEP} = false ]; then
                    echo -e "${BLUE}Build without depedencies${NO_COLOUR}"
                else
                    echo -e "${BLUE}Build without depedencies${NO_COLOUR}"
                    shift 1; exit 0
                fi
                shift 1;
                ;;
            --target_arch=*)
                EZMQX_TARGET_ARCH="${1#*=}";
                echo -e "${GREEN}Target Arch is: $EZMQX_TARGET_ARCH${NO_COLOUR}"
                shift 1
                ;;
            --build_mode=*)
                EZMQX_BUILD_MODE="${1#*=}";
                echo -e "${GREEN}Build mode is: $EZMQX_BUILD_MODE${NO_COLOUR}"
                shift 1;
                ;;
            -c)
                clean_ezmqx
                shift 1; exit 0
                ;;
            -h)
                usage; exit 0
                ;;
            --help)
                usage; exit 0
                ;;
            -*)
                echo -e "${RED}"
                echo "unknown option: $1" >&2;
                echo -e "${NO_COLOUR}"
                usage; exit 1
                ;;
            *)
                echo -e "${RED}"
                echo "unknown option: $1" >&2;
                echo -e "${NO_COLOUR}"
                usage; exit 1
                ;;
        esac
    done
}

process_cmd_args "$@"
build
copy_required_libs
copy_docker_script
echo -e "${GREEN}protocol-ezmq-plus-c build done${NO_COLOUR}"

