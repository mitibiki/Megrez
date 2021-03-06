/* =====================================================================
Copyright 2017 The Megrez Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
========================================================================*/

#ifndef MEGREZ_UTILS_H_
#define MEGREZ_UTILS_H_

#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include "megrez/basic.h"

namespace megrez {

template<typename T> 
std::string NumToString(T t) {
	std::stringstream ss;
	if (sizeof(T) > 1) ss << t;
	else ss << static_cast<int>(t);  // 避免使用char类型作为字符数据
	return ss.str();
}

template<typename T> 
std::string IntToStringHex(T i) {
	std::stringstream ss;
	ss << std::setw(sizeof(T) * 2)
	   << std::setfill('0')
	   << std::hex
	   << std::uppercase
	   << i;
	return ss.str();
}


template<typename T> 
const T *GetRoot(const void *buf) {
	EndianCheck();
	return reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(buf) +
		EndianScalar(*reinterpret_cast<const uofs_t *>(buf)));
}


inline int64_t StringToInt(const char *str) {
	#ifdef _MSC_VER
		return _strtoui64(str, nullptr, 10);
	#else
		return strtoull(str, nullptr, 10);
	#endif
}

inline bool LoadFile(const char *name, bool binary, std::string *buf) {
	std::ifstream ifs(name, binary ? std::ifstream::binary : std::ifstream::in);
	if (!ifs.is_open()) return false;
	*buf = std::string(std::istreambuf_iterator<char>(ifs),
					   std::istreambuf_iterator<char>());
	return !ifs.bad();
}

inline bool SaveFile(const char *name, const char *buf, size_t len, bool binary) {
	std::ofstream ofs(name, binary ? std::ofstream::binary : std::ofstream::out);
	if (!ofs.is_open()) return false;
	ofs.write(buf, len);
	return !ofs.bad();
}

inline bool SaveFile(const char *name, const std::string &buf, bool binary) {
	return SaveFile(name, buf.c_str(), buf.size(), binary);
}



inline vofs_t FieldIndexToOffset(vofs_t field_id) {
	const int fixed_fields = 2;
	return (field_id + fixed_fields) * sizeof(vofs_t);
}

inline size_t PaddingBytes(size_t buf_size, size_t scalar_size) {
	return ((~buf_size) + 1) & (scalar_size - 1);
}

inline size_t LookupEnum(const char **names, const char *name) {
	for (const char **p = names; *p; p++)
		if (!strcmp(*p, name))
			return p - names;
	return -1;
}

}  // namespace megrez

#endif  // MEGREZ_UTILS_H_