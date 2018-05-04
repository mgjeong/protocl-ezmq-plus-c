/*******************************************************************************
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *******************************************************************************/

#include <string>
#include <cstring>
#include <vector>

#include "cezmqxutils.h"

using namespace std;

char* ConvertStringToCharStr(std::string str)
{
    size_t size = str.size();
    char* cstr = new char[size + 1];
    memcpy(cstr, str.c_str(), size);
    cstr[size] = '\0';
    return cstr;
}

char** ConvertListToCharStrArr(list<string>& list)
{
    unsigned long size = list.size();
    char ** cstr = new char*[size];

    std::list<std::string>::iterator iter = list.begin();
    for (unsigned long i = 0; i < size; i++)
    {
        auto it = std::next(list.begin(), i);
        cstr[i] = ConvertStringToCharStr(*it);
    }
    return cstr;
}

