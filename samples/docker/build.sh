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

#!/bin/sh

DOCKER_ROOT=$(pwd)

if [ -d "./ezmqx_csamples" ] ; then
        echo "ezmqx_csamples folder exists"
    else
        mkdir ezmqx_csamples
fi

cd ./ezmqx_csamples
# copy all the ezmqx_csamples
cp ./../../publisher .
cp ./../../amlsubscriber .
cp ./../../xmlsubscriber .
cp ./../../topicdiscovery .
#copy .aml file
cp ./../../sample_data_model.aml .

cd $DOCKER_ROOT
if [ -d "./ezmqx_cextlibs" ] ; then
        echo "ezmqx_cextlibs folder exists"
    else
        mkdir ezmqx_cextlibs
fi

cd ./ezmqx_cextlibs
# copy aml ezmqx_libs
cp ./../../../libcezmq_plus.so .
cp ./../../../extlibs/* .
#copy libzmq.so
cp /usr/local/lib/libzmq.so.5 .
#copy protobuf.so
cp /usr/local/lib/libprotobuf.so.14 .
#copy libcurl.so
cp /usr/local/lib/libcurl.so.4 .
#copy libjsoncpp.so
cp /usr/local/lib/libjsoncpp.so.19 .
#copy libstd++.so.6
cp /usr/lib/arm-linux-gnueabihf/libstdc++.so.6 .

cd $DOCKER_ROOT

# build the ezmq-plus-java sample image
sudo docker build -t docker.sec.samsung.net:5000/protocol-ezmq-plus-c-sample -f Dockerfile_arm .


