/// @file      plot_lines.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-30

#include "ast/AstPlot.hpp"


AST_USING_NAMESPACE

int main()
{
    plt::figure();
    plt::plot({1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});
}
