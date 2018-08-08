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

################ CEZMQ+ build script ##################

import os

Import('env')

cezmqx_env = env.Clone()
target_os = cezmqx_env.get('TARGET_OS')
target_arch = cezmqx_env.get('TARGET_ARCH')

if cezmqx_env.get('RELEASE'):
    cezmqx_env.AppendUnique(CCFLAGS=['-Os'])
else:
    cezmqx_env.AppendUnique(CCFLAGS=['-g'])

cezmqx_env.AppendUnique(CPPPATH=[
    './dependencies/protocol-ezmq-plus-cpp/dependencies/protocol-ezmq-cpp/include',
    './dependencies/protocol-ezmq-plus-cpp/dependencies/datamodel-aml-cpp/include',
	'./dependencies/protocol-ezmq-plus-cpp/extlibs/zmq',
	'./dependencies/protocol-ezmq-plus-cpp/include',
    './dependencies/datamodel-aml-c/include',
	'./dependencies/datamodel-aml-c/dependencies/datamodel-aml-cpp/include',
	'./include',
	'./include/internal'
])

build_mode = 'release'
if not cezmqx_env.get('RELEASE'):
    build_mode = 'debug'

cezmqx_env.PrependUnique(LIBS=['ezmq_plus'], LIBPATH=[os.path.join('./dependencies/protocol-ezmq-plus-cpp/out/linux/', target_arch, build_mode)])
cezmqx_env.PrependUnique(LIBS=['aml'], LIBPATH=[os.path.join('./dependencies/datamodel-aml-c/dependencies/datamodel-aml-cpp/out/linux/', target_arch, build_mode)])
cezmqx_env.PrependUnique(LIBS=['caml'], LIBPATH=[os.path.join('./dependencies/datamodel-aml-c/out/linux/', target_arch, build_mode)])

if target_os not in ['windows']:
    cezmqx_env.AppendUnique(
        CXXFLAGS=['-O2', '-g', '-Wall', '-fPIC', '-fmessage-length=0', '-std=c++0x', '-I/usr/local/include'])

if target_os not in ['windows']:
    cezmqx_env.AppendUnique(LINKFLAGS=['-Wl,--no-undefined'])

if target_os in ['linux']:
    cezmqx_env.AppendUnique(LIBS=['pthread'])

if target_os in ['linux']:
    if not env.get('RELEASE'):
        cezmqx_env.PrependUnique(LIBS=['gcov'])
        cezmqx_env.AppendUnique(CXXFLAGS=['--coverage'])

CEZMQ_DIR = '.'
cezmqx_env.AppendUnique(cezmqx_src = [cezmqx_env.Glob(os.path.join(CEZMQ_DIR, 'src', '*.cpp'))])

ezmqshared = cezmqx_env.SharedLibrary('cezmq_plus', cezmqx_env.get('cezmqx_src'))
ezmqstatic = cezmqx_env.StaticLibrary('cezmq_plus', cezmqx_env.get('cezmqx_src'))

# Go to build C EZMQ plus sample apps
if target_os == 'linux':
       SConscript('samples/SConscript')

# Go to build C EZMQ plus unit test cases
if target_os == 'linux':
       SConscript('unittests/SConscript')

