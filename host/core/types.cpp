#include "types.hpp"


Type string_to_type(const std::string& str)
{
    assert(str.size() >= 2);

    if ('u' == str[0])
    {
        if (str == "u8") return Type::U8;
    }
    else if ('f' == str[0])
    {
        if (str == "f16") return Type::F16;
    }

    return Type::UNDEFINED;
};

unsigned size_of_type(const Type& type)
{
    auto it = c_type_size.find(type);
    assert(it != c_type_size.end());
    return it->second;
};



#define EXP_MASK_F32 0x7F800000U
#define EXP_MASK_F16     0x7C00U
float float16to32(uint16_t x)
{
    // this is storage for output result
    uint32_t u = x;

    // get sign in 32bit format
    uint32_t s = ((u & 0x8000) << 16);

    // check for NAN and INF
    if ((u & EXP_MASK_F16) == EXP_MASK_F16) {
        // keep mantissa only
        u &= 0x03FF;

        // check if it is NAN and raise 10 bit to be align with intrin
        if (u) {
            u |= 0x0200;
        }

        u <<= (23 - 10);
        u |= EXP_MASK_F32;
        u |= s;
    } else if ((x & EXP_MASK_F16) == 0) { // check for zero and denormals. both are converted to zero
        u = s;
    } else {
        // abs
        u = (u & 0x7FFF);

        // shift mantissa and exp from f16 to f32 position
        u <<= (23 - 10);

        // new bias for exp (f16 bias is 15 and f32 bias is 127)
        u += ((127 - 15) << 23);

        // add sign
        u |= s;
    }

    // finaly represent result as float and return
    return *reinterpret_cast<float *>(&u);
}

