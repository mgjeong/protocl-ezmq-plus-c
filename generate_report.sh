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

set -e
#Colors
RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\033[0;34m"
NO_COLOUR="\033[0m"

#Defaults
time_stamp=$(date -u +%Y-%b-%d)
module_name="EZMQX"
report_format="html"
report_flags="--html --html-details";

#EZMQX Flags
EZMQX="${PWD}"
EZMQX_TARGET_OS="linux"
EZMQX_TARGET_ARCH="$(uname -m)"
USE_TIMESTAMP="yes"
UNITTEST_XML_REPORT="yes"

usage() {

    echo "Pre-requisite: Built protocol-ezmq-plus-c library in debug mode"
    echo "Usage: generate_report.sh <options>"
    echo "Options:"
    echo "      -h / --help                                    : Display help and exit"
    echo "      -c                                             : Clean C EZMQX Repository; Should be used to clean existing repository"
    echo "      -f [html|gcov|xml] (default: html)             : Report Format."
    echo "      --format=[html|gcov|xml] (default: html)       : Report Format."
    echo "      --timestamp=[yes|no] (default: yes)            : Remove Time Stamp from report output.If directory exits, overwrites the report to the same directory"
    echo "      --ut_report=[yes|no] (default: yes)            : The unit test report will be generated in xml format (as gtest only supports xml)."
    echo "      --target_arch=[x86_64|armhf] (default: x86_64) : Choose Target Architecture for running test executables."
    echo "Pre-requisite:"
    echo "  Built protocol-ezmq-plus-c in debug mode"
    echo "Example:"
    echo "  $ ./generate_report.sh --format=html --timestamp=yes --ut_report=yes"
}

clean_ezmqx() {
    echo -e "Cleaning ${BLUE}${EZMQX}${NO_COLOUR}"
    echo -e "Deleting  ${RED}${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug${NO_COLOUR}"
    echo -e "Deleting  ${RED}${EZMQX}/.sconsign.dblite${NO_COLOUR}"
    rm -r "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug" "${EZMQX}/.sconsign.dblite"
    find "${EZMQX}" -name "*.memcheck" -delete -o -name "*.gcno" -delete -o -name "*.gcda" -delete -o -name "*.os" -delete -o -name "*.o" -delete
    echo -e "Finished Cleaning ${BLUE}${EZMQX}${NO_COLOUR}"
}

process_cmd_args() {
    while [ "$#" -gt 0  ]; do
        case "$1" in
            -c)
                clean_ezmqx
                shift 1; exit 0
                ;;

            -f)
                report_format="$2";
                if [ "gcov" != ${report_format} -a "html" != ${report_format} -a "xml" != ${report_format} ]; then
                    usage; exit 1;
                fi
                case "$report_format" in
                    "html")
                        report_flags="--html --html-details";
                        ;;
                    "gcov")
                        report_flags="";
                        ;;
                    "xml")
                        report_flags="--xml --xml-pretty";
                        ;;
                esac
                shift 2
                ;;

            --format=*)
                report_format="${1#*=}";
                if [ "gcov" != ${report_format} -a "html" != ${report_format} -a "xml" != ${report_format} ]; then
                    usage; exit 1;
                fi
                case "$report_format" in
                    "html")
                        report_flags="--html --html-details";
                        ;;
                    "gcov")
                        report_flags="";
                        ;;
                    "xml")
                        report_flags="--xml --xml-pretty";
                        ;;
                esac
                shift 1
                ;;

            --format)
                echo "$1 requires an argument [gcov|html|xml]" >&2;
                usage;
                exit 1
                ;;

            --timestamp=*)
                USE_TIMESTAMP="${1#*=}";
                if [ "yes" != ${USE_TIMESTAMP} -a "no" != ${USE_TIMESTAMP} ]; then
                    usage; exit 1;
                fi
                shift 1
                ;;
            --timestamp)
                echo "$1 requires an argument [yes|no]" >&2;
                usage;
                exit 1
                ;;

            --ut_report=*)
                UNITTEST_XML_REPORT="${1#*=}";
                if [ "yes" != ${UNITTEST_XML_REPORT} -a "no" != ${UNITTEST_XML_REPORT} ]; then
                    usage; exit 1;
                fi
                shift 1
                ;;
            --ut_report)
                echo "$1 requires an argument [yes|no]" >&2;
                usage;
                exit 1
                ;;

            --target_arch=*)
                EZMQX_TARGET_ARCH="${1#*=}";
                if [ "armhf" != ${EZMQX_TARGET_ARCH} -a "x86_64" != ${EZMQX_TARGET_ARCH} ]; then
                    usage; exit 1;
                fi
                shift 1
                ;;
            --target_arch)
                echo "$1 requires an argument" >&2;
                usage;
                exit 1
                ;;

            -h)
                usage;
                shift 1; exit 0
                ;;
            --help)
                usage;
                shift 1; exit 0
                ;;

            -*)
                echo "unknown option: $1" >&2;
                usage;
                exit 1
                ;;
        esac
    done
}

generate_report_EZMQX()
{
    # copy .aml file
    cp ${EZMQX}/unittests/sample_data_model.aml ${EZMQX}

    # Setting Parameters
    if [ "yes" = ${USE_TIMESTAMP} ]; then
        report_dir="${module_name}_${time_stamp}"
    else
        report_dir="${module_name}"
    fi
    report_file="report.${report_format}"
    test_report_dir="CoverageReport/${report_format}/${report_dir}"
    test_report_file="${test_report_dir}/${report_file}"

    rm -rf "${test_report_dir}"
    mkdir -p "${test_report_dir}"

    LD_LIBRARY_PATH="${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/extlibs"

    #Setting Proper Location for UnitTest XML report generation
    unittest_report_dir="UnitTestReport/${report_dir}"
    if [ "yes" = ${UNITTEST_XML_REPORT} ]; then
        rm -rf "${unittest_report_dir}"
        mkdir -p "${unittest_report_dir}"
        UNITTEST_XML_REPORT_FLAG_PREFIX="--gtest_output=xml:${unittest_report_dir}"
    fi

    tests_list=(
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_config_test"
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_endpoint_test"
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_publisher_test"
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_topicdiscovery_test"
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_aml_subscriber_test"
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_topic_test"
                "${EZMQX}/out/${EZMQX_TARGET_OS}/${EZMQX_TARGET_ARCH}/debug/unittests/cezmqx_xml_subscriber_test"
               );

    for exe in ${tests_list[@]}; do
        filename=$(basename -- "${exe}")
        if [ -n "${UNITTEST_XML_REPORT_FLAG_PREFIX}" ]; then
            UNITTEST_XML_REPORT_FLAG="${UNITTEST_XML_REPORT_FLAG_PREFIX}/${filename}.xml"
        fi
        eval "${exe} ${UNITTEST_XML_REPORT_FLAG}"
    done

    unset tests_list

    sleep 1

    echo -e "Generating ${GREEN}${module_name}${NO_COLOUR} Reports"

    # Printing Unit Test Report Location
    if [  "yes" = ${UNITTEST_XML_REPORT} ]; then
        echo -e "${GREEN}${module_name}${NO_COLOUR} UnitTest Report Location: ${BLUE}${unittest_report_dir}${NO_COLOUR}"
    fi

    gcovr -r . \
        -e "extlibs.*" \
        -e "out.linux.*" \
        -e "include.*" \
        -e "unittests.*" \
        -e "samples.*" \
        -e "dependencies.protocol-ezmq-plus-cpp.*" \
        -e "dependencies.datamodel-aml-c.*" \
        ${report_flags} -o ${test_report_file}

    if [  $? -eq 0 ]; then
        echo -e "${GREEN}${module_name}${NO_COLOUR} Coverage Report Location: ${BLUE}${test_report_file}${NO_COLOUR}"
        echo -e "${GREEN}${module_name}${NO_COLOUR} Report Generated ${GREEN}Successfully!${NO_COLOUR}"
    else
        echo -e "${RED}${module_name}${NO_COLOUR} Report Generation ${RED}Failed!${NO_COLOUR}"
    fi
}

process_cmd_args "$@"
generate_report_EZMQX

