#include <bits/stdc++.h>

std::vector<std::pair<int64_t, char>> getSegment(std::vector<int> v) {
    std::vector<std::pair<int64_t, char>> segment;
    v.erase(v.begin());
    for (auto i : v) {
        if (i > 0) {
            if (segment.empty() or segment.back().second == '-') {
                segment.push_back({ 1, '+' });
            } else {
                segment.back().first++;
            }
        } else {
            if (segment.empty() or segment.back().second == '+') {
                segment.push_back({ 1, '-' });
            } else {
                segment.back().first++;
            }
        }
    }
    return segment;
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    
    int N; std::cin >> N;
    std::vector <int> v(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> v[i];
    }
    for (int i = N - 1; i; --i) {
        v[i] -= v[i - 1];
    }
    
    std::vector<std::pair<int64_t, char>> segment = getSegment(v);

    int64_t ans = 0ll;
    for (int i = 0; i < segment.size(); ++i) {
        if (segment[i].second == '+') { continue; }
        if (i == 0 || i == segment.size() - 1) { continue; }
        ans += segment[i - 1].first * segment[i + 1].first;
    }

    std::cout << ans << std::endl;
    return 0;
}