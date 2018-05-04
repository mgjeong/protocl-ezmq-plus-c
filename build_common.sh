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
TARGET_ARCH="x86"

RELEASE="1"
LOGGING="0"

install_dependencies() {
    if [ -d "./dependencies" ] ; then
        echo "dependencies folder exist"
    else
        mkdir dependencies
    fi

    #clone and build protocol-ezmq-plus-cpp
    cd $DEP_ROOT
    git clone git@github.sec.samsung.net:RS7-EdgeComputing/protocol-ezmq-plus-cpp.git
    cd ./protocol-ezmq-plus-cpp
    echo -e "${GREEN}Building ezmq-plus library and its dependencies${NO_COLOUR}"
    if [ "x86_64" = ${EZMQX_TARGET_ARCH} ]; then
         ./build.sh;
    elif [ "armhf-native" = ${EZMQX_TARGET_ARCH} ]; then
         ./build_arm.sh;
    else
         echo -e "${RED}ezmq-plus-cpp: Not a supported architecture${NO_COLOUR}"
         usage; exit 1;
    fi
    echo -e "${GREEN}Installation of ezmq-plus library and its dependencies done${NO_COLOUR}"

    #clone and build datamodel-aml-c
    cd $DEP_ROOT
    git clone git@github.sec.samsung.net:RS7-EdgeComputing/datamodel-aml-c.git
    cd ./datamodel-aml-c
    echo -e "${GREEN}Building datamodel-aml-c library and its dependencies${NO_COLOUR}"
    ./build_common.sh --target_arch=${EZMQX_TARGET_ARCH} --build_mode=${EZMQX_BUILD_MODE} --logging=${EZMQX_LOGGING} --disable_protobuf=${EZMQX_DISABLE_PROTOBUF}
    echo -e "${GREEN}Installation of datamodel-aml-c library and its dependencies done${NO_COLOUR}"
}

build_x86() {
    echo -e "Building for x86"
    scons TARGET_OS=linux TARGET_ARCH=x86 RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_x86_64() {
    echo -e "Building for x86_64"
    scons TARGET_OS=linux TARGET_ARCH=x86_64 RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_arm() {
    echo -e "Building for arm"
    scons TARGET_ARCH=arm TC_PREFIX=/usr/bin/arm-linux-gnueabi- TC_PATH=/usr/bin/ RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_arm64() {
    echo -e "Building for arm64"
    scons TARGET_ARCH=arm64 TC_PREFIX=/usr/bin/aarch64-linux-gnu- TC_PATH=/usr/bin/ RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_armhf() {
    echo -e "Building for armhf"
    scons TARGET_ARCH=armhf TC_PREFIX=/usr/bin/arm-linux-gnueabihf- TC_PATH=/usr/bin/ RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_armhf_native() {
    echo -e "Building for armhf_native"
    scons TARGET_ARCH=armhf RELEASE=${RELEASE} LOGGING=${LOGGING}
}

build_armhf_qemu() {
    echo -e "Building for armhf-qemu"
    scons TARGET_ARCH=armhf RELEASE=${RELEASE} LOGGING=${LOGGING}

    if [ -x "/usr/bin/qemu-arm-static" ]; then
        echo -e "${BLUE}qemu-arm-static found, copying it to current directory${NO_COLOUR}"
        cp /usr/bin/qemu-arm-static .
    else
        echo -e "${RED}No qemu-arm-static found${NO_COLOUR}"
        echo -e "${BLUE} - Install qemu-arm-static and build again${NO_COLOUR}"
    fi
}

clean_ezmqx() {
    echo -e "Cleaning ${BLUE}${PROJECT_ROOT}${NO_COLOUR}"
    echo -e "Deleting  ${RED}${PROJECT_ROOT}/out/${NO_COLOUR}"
    echo -e "Deleting  ${RED}${PROJECT_ROOT}/.sconsign.dblite${NO_COLOUR}"
    rm -r "${PROJECT_ROOT}/out/" "${PROJECT_ROOT}/.sconsign.dblite"
    find "${PROJECT_ROOT}" -name "*.memcheck" -delete -o -name "*.gcno" -delete -o -name "*.gcda" -delete -o -name "*.os" -delete -o -name "*.o" -delete
    echo -e "Deleting  ${RED}${PROJECT_ROOT}/dependencies/${NO_COLOUR}"
    rm -rf ./dependencies/protocol-ezmq-plus-cpp
    echo -e "Finished Cleaning ${BLUE}${EZMQ}${NO_COLOUR}"
}

usage() {
    echo -e "${BLUE}Usage:${NO_COLOUR} ./build_common.sh <option>"
    echo -e "${GREEN}Options:${NO_COLOUR}"
    echo "  --target_arch=[x86_64|armhf-native]                                :  Choose Target Architecture"
    echo "  --with_dependencies=(default: false)                               :  Build cezmq-plus with its dependency ezmq-plus and datamodel-aml-c"
    echo "  --build_mode=[release|debug](default: release)                     :  Build in release or debug mode"
    echo "  -c                                                                 :  Clean ezmq-plus Repository and its dependencies"
    echo "  -h / --help                                                        :  Display help and exit"
    echo -e "${GREEN}Examples: ${NO_COLOUR}"
    echo -e "${BLUE}  build:-${NO_COLOUR}"
    echo "  $ ./build_common.sh --target_arch=x86_64"
    echo "  $ ./build_common.sh --with_dependencies=true --target_arch=x86_64 "
    echo -e "${BLUE}  debug mode build:-${NO_COLOUR}"
    echo "  $ ./build_common.sh --target_arch=x86_64 --build_mode=debug"
    echo -e "${BLUE}  clean:-${NO_COLOUR}"
    echo "  $ ./build_common.sh -c"
    echo -e "${BLUE}  help:-${NO_COLOUR}"
    echo "  $ ./build_common.sh -h"
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
    if [ "x86" = ${EZMQX_TARGET_ARCH} ]; then
         build_x86;
    elif [ "x86_64" = ${EZMQX_TARGET_ARCH} ]; then
         build_x86_64;
    elif [ "arm" = ${EZMQX_TARGET_ARCH} ]; then
         build_arm;
    elif [ "arm64" = ${EZMQX_TARGET_ARCH} ]; then
         build_arm64;
    elif [ "armhf" = ${EZMQX_TARGET_ARCH} ]; then
         build_armhf;
    elif [ "armhf-qemu" = ${EZMQX_TARGET_ARCH} ]; then
         build_armhf_qemu;
    elif [ "armhf-native" = ${EZMQX_TARGET_ARCH} ]; then
         build_armhf_native;
    else
         echo -e "${RED}Not a supported architecture${NO_COLOUR}"
         usage; exit 1;
    fi
}

copy_required_libs() {
   echo -e "${BLUE}Copy required libraries to extlibs folder${NO_COLOUR}"

   if  [ "armhf-native" = ${EZMQX_TARGET_ARCH} ]; then
         TARGET_ARCH="armhf"
   else
         TARGET_ARCH=${EZMQX_TARGET_ARCH}
   fi

   cd $PROJECT_ROOT/out/linux/${TARGET_ARCH}/${EZMQX_BUILD_MODE}
   if [ -d "./extlibs" ] ; then
        echo "extlibs folder exist"
   else
        mkdir extlibs
   fi

    cd extlibs
    # copy ezmq_plus.so, ezmq.so, caml.so and aml.so to extlibs folder
    PREFIX=../../../../..
    cp $PREFIX/dependencies/datamodel-aml-c/out/linux/${TARGET_ARCH}/${EZMQX_BUILD_MODE}/libcaml.so .
    cp $PREFIX/dependencies/datamodel-aml-c/dependencies/datamodel-aml-cpp/out/linux/${TARGET_ARCH}/${EZMQX_BUILD_MODE}/libaml.so .
    cp $PREFIX/dependencies/protocol-ezmq-plus-cpp/out/linux/${TARGET_ARCH}/${EZMQX_BUILD_MODE}/libezmq_plus.so .
    cp $PREFIX/dependencies/protocol-ezmq-plus-cpp/dependencies/protocol-ezmq-cpp/out/linux/${TARGET_ARCH}/${EZMQX_BUILD_MODE}/libezmq.so .
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
echo -e "${GREEN}protocol-ezmq-plus-c build done${NO_COLOUR}"

