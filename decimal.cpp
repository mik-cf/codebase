#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// 获取小数点后的精度
int getPrecision(const string &s) {
    size_t p = s.find('.');
    if (p == string::npos)
        return 0;
    return s.size() - p - 1;
}

// 将字符串转换为最简分数形式
pair<int64_t, int64_t> rationalize(const string &s) {
    int precision = getPrecision(s);
    long double x;
    stringstream ss(s);
    ss >> x;

    // 处理整数情况
    if (precision == 0)
        return {static_cast<int64_t>(x), 1};

    // 连分数展开算法
    vector<int64_t> a;
    long double rem = x;

    while (a.size() < 20) { // 限制最大迭代次数
        int64_t intPart = static_cast<int64_t>(rem);
        a.push_back(intPart);

        rem -= intPart;
        if (rem < 1e-15)
            break; // 达到精度限制

        rem = 1.0 / rem;
    }

    // 计算渐进分数
    vector<pair<int64_t, int64_t>> convergents;
    for (size_t i = 0; i < a.size(); ++i) {
        int64_t p = 1, q = 0;

        for (int j = i; j >= 0; --j) {
            swap(p, q);
            p += a[j] * q;
        }

        // 检查是否符合原始精度
        stringstream tmp;
        tmp << fixed << setprecision(precision) << static_cast<long double>(p) / q;
        string result_str = tmp.str();

        if (result_str == s) {
            // 约分
            int64_t g = __gcd(p, q);
            return {p / g, q / g};
        }

        convergents.emplace_back(p, q);
    }

    // 如果没有找到精确匹配，返回最接近的
    if (!convergents.empty()) {
        int64_t p = convergents.back().first;
        int64_t q = convergents.back().second;
        int64_t g = __gcd(p, q);
        return {p / g, q / g};
    }

    return {-1, -1}; // 错误情况
}

// 因子分解
map<int64_t, int64_t> factorize(int64_t x) {
    map<int64_t, int64_t> factors;
    if (x < 0) {
        factors[-1] = 1;
        x = -x;
    }

    for (int64_t i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            factors[i]++;
            x /= i;
        }
    }

    if (x > 1)
        factors[x]++;
    return factors;
}

// 合并两个因子分解结果
map<int64_t, int64_t> merge(const map<int64_t, int64_t> &a, const map<int64_t, int64_t> &b) {
    map<int64_t, int64_t> result = a;
    for (const auto &kv : b) {
        result[kv.first] += kv.second;
    }
    return result;
}

// 计算最大公约数
int64_t gcd(int64_t a, int64_t b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// 计算 a^b
int64_t power(int64_t a, int64_t b) {
    int64_t res = 1;
    while (b--)
        res *= a;
    return res;
}

// 处理带根号的有理化
void rationalizeWithRoot(const string &s) {
    int precision = getPrecision(s);
    long double x;
    stringstream ss(s);
    ss >> x;

    bool isNegative = (x < 0);
    if (isNegative)
        x = -x;

    // 计算 x 的平方
    long double x_squared = x * x;

    // 确定平方后的精度
    int new_precision = max(0, precision - 2); // 平方根会减少约一半的精度
    stringstream ss_squared;
    ss_squared << fixed << setprecision(new_precision) << x_squared;
    string s_squared = ss_squared.str();

    // 有理化平方后的数
    auto [p, q] = rationalize(s_squared);
    if (p == -1 || q == -1) {
        cout << "Error: Cannot rationalize.\n";
        return;
    }

    // 因子分解
    auto facp = factorize(p);
    auto facq = factorize(q);
    auto facpq = merge(facp, facq);

    // 计算 a√b/c
    int64_t a = 1, b = 1, c = q;
    for (const auto &[key, value] : facpq) {
        a *= power(key, value / 2);
        b *= power(key, value % 2);
    }

    // 约分
    int64_t g = gcd(a, c);
    a /= g;
    c /= g;

    // 输出结果
    if (isNegative)
        cout << "-";

    if (a == 0) {
        cout << "0\n";
        return;
    }

    if (a == 1 && b != 1) {
        cout << "sqrt(" << b << ")";
    } else if (a == 1 && b == 1) {
        cout << 1;
    } else if (a != 1 && b == 1) {
        cout << a;
    } else {
        cout << a << " sqrt(" << b << ")";
    }
    if (c != 1)
        cout << " / " << c;
    cout << endl;
}

int main() {
    string s;
    cout << "Enter a decimal number: ";
    cin >> s;

    // 验证输入格式
    regex decimalPattern("^-?\\d+(\\.\\d+)?$");
    if (!regex_match(s, decimalPattern)) {
        cout << "Invalid input. Please enter a valid decimal number.\n";
        return 1;
    }

    rationalizeWithRoot(s);
    return 0;
}