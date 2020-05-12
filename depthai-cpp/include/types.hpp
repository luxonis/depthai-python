#pragma once

#include <assert.h>

#include <string>
#include <unordered_map>
#include <map>




// TODO: make the same as numpy types ('f16', 'f<16', 'i64', ...)

enum class Type {
    UNDEFINED,
    U8,
    F16
};


const std::map<Type, unsigned int> c_type_size = {
    {Type::UNDEFINED, 1},
    {Type::U8,        1},
    {Type::F16,       2}
};

/*

const std::unordered_map<Type, unsigned int> c_type_size =
{
    {Type::UNDEFINED, 1},
    {Type::U8,        1},
    {Type::F16,       2}
};
*/

Type     string_to_type (const std::string& str);
unsigned int size_of_type   (const Type& type);


float float16to32(uint16_t x);
