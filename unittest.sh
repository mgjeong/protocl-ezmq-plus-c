###############################################################################
# Copyright 2018 Samsung Electronics All Rights Reserved.
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

# build ezmq-plus-c sdk
./build.sh

# run the unit test cases
cd out/linux/x86_64/release/unittests
export LD_LIBRARY_PATH=../extlibs
./cezmqx_config_test
./cezmqx_endpoint_test
./cezmqx_publisher_test
./cezmqx_topicdiscovery_test
./cezmqx_aml_subscriber_test
./cezmqx_topic_test
./cezmqx_xml_subscriber_test

