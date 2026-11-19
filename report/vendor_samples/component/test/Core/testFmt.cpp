/// @file      testFmt.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-05

#if AST_WITH_FMT
#include <fmt/format.h>
#include <vector>

int main()
{
    std::vector<double> v = {10000000000., 6678137.1234678901, 1.234567890123456789e-10, 1.234567890123456789e+10};
    for (auto d : v)
    {
        fmt::println("{}", d);
        fmt::println("{:g}", d);
        fmt::println("{:#g}", d);
        fmt::println("{:#}", d);
        fmt::println("");
    }
}
#else
int main(){}
#endif
