#include <bits/stdc++.h>

int main() {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    int x = (a << 6) | b;
    int y = (c << 6) | d;
    std::cout << ((x < y) ? ("No") : ("Yes")) << std::endl;
    return 0;
}