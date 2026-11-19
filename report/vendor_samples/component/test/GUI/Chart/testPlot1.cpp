/// @file      testPlotLines.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-19

#ifdef AST_WITH_MATPLOT
#include "ast/AstPlot.hpp"
#include "ast/QwtBackend.hpp"

AST_USING_NAMESPACE

int main()
{
    aUseQwtBackend();

    using plt::pi;
    using plt::on;
    std::vector<double> x = plt::linspace(0, 2 * pi);
    std::vector<double> y = plt::transform(x, [](double x) { return sin(x); });

    plt::plot(x, y, "-o");
    plt::hold(on);
    plt::plot(x, plt::transform(y, [](double y) { return -y; }), "--xr");
    plt::plot(x, plt::transform(x, [](double x) { return x / pi - 1.; }), "-:gs");
    plt::plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");

    plt::show();
}
#else
#include <stdio.h>
int main(){
    puts("没有启用matplot++库");
    return 0;
}
#endif
