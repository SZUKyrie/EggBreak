#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

#define GROUP 10

int generate_random_Int(int Min, int Max) {
    random_device rd;  // 随机种子
    mt19937 gen(rd()); // 梅森旋转算法引擎
    uniform_int_distribution<int> dist(Min, Max);
    return dist(gen);
}


int __bruteForce(int f, int e) {
    if (f == 0 || f == 1) return f;
    if (e == 1) return f;
    int ans = INT_MAX;
    for (int k = 1; k <= f; k++) {
        int res = 1 + max(__bruteForce(k - 1, e - 1), __bruteForce(f - k, e));
        ans = min(ans, res);
    }
    return ans;
}

void bruteForce(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __bruteForce(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}

int __eggDP(int f, int e) {
    vector<vector<int>> dp(f + 2, vector<int>(e + 2, 0));
    for (int i = 1; i <= f; i++) {
        dp[i][1] = i;
    }
    for (int j = 1; j <= e; j++) {
        dp[1][j] = 1;
    }
    for (int i = 2; i <= f; i++) {
        for (int j = 2; j <= e; j++) {
            dp[i][j] = INT_MAX;
            for (int k = 1; k <= i; k++) {
                int res = 1 + max(dp[k - 1][j - 1], dp[i - k][j]);
                dp[i][j] = min(dp[i][j], res);
            }
        }        
    }
    return dp[f][e];
}

void eggDP(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggDP(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}

int __eggDP_e(int f, int e) {
    vector<vector<int>> dp(f + 2, vector<int>(e + 2, 0));
    for (int i = 1; i <= f; i++) {
        dp[i][1] = i;
    }
    for (int j = 1; j <= e; j++) {
        dp[1][j] = 1;
    }
    for (int j = 2; j <= e; j++) {
        for (int i = 2; i <= f; i++) {
            dp[i][j] = INT_MAX;
            for (int k = 1; k <= i; k++) {
                int res = 1 + max(dp[k - 1][j - 1], dp[i - k][j]);
                dp[i][j] = min(dp[i][j], res);
            }
        }        
    }
    return dp[f][e];
}

// cache使用效率低，导致速度稍微慢些
void eggDP_e(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggDP_e(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}

int __eggDPSpaceOpt(int f, int e) {
    vector<vector<int>> dp(f + 1, vector<int>(2, 0));
    for (int i = 1; i <= f; i++) {
        dp[i][1] = i;
    }
    dp[1][0] = 1;
    // 注意这个刷表的顺序
    for (int j = 2; j <= e; j++) {
        for (int i = 2; i <= f; i++) {
            dp[i][j % 2] = INT_MAX;
            for (int k = 1; k <= i; k++) {
                int res = 1 + max(dp[k - 1][(j - 1) % 2], dp[i - k][j % 2]);
                dp[i][j % 2] = min(dp[i][j % 2], res);
            }
        }        
    }
    return dp[f][e % 2];
}

void eggDPSpaceOpt(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggDPSpaceOpt(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}

int __eggDPBinaryOpt(int f, int e) {
    vector<vector<int>> dp(f + 2, vector<int>(e + 2, 0));
    for (int i = 1; i <= f; i++) {
        dp[i][1] = i;
    }
    for (int j = 1; j <= e; j++) {
        dp[1][j] = 1;
    }
    for (int i = 2; i <= f; i++) {
        for (int j = 2; j <= e; j++) {
            dp[i][j] = INT_MAX;
            int left = 0, right = i;
            int mid;
            while (left < right) {
                mid = (left + right + 1) / 2;
                int broken = dp[mid - 1][j - 1];
                int unbroken = dp[i - mid][j];
                if (broken > unbroken) {
                    dp[i][j] = min(dp[i][j], 1 + broken);
                    right = mid - 1;
                }
                else {
                    dp[i][j] = min(dp[i][j], 1 + unbroken);
                    left = mid;
                }
            }
        }        
    }
    return dp[f][e];
}

// 二分查找+动态规划方法
void eggDPBinaryOpt(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggDPBinaryOpt(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}

int __eggDPBinarySpaceOpt(int f, int e) {
    vector<vector<int>> dp(f + 1, vector<int>(2, 0));
    for (int i = 1; i <= f; i++) {
        dp[i][1] = i;
    }
    dp[1][0] = 1;
    // 注意这个刷表的顺序
    for (int j = 2; j <= e; j++) {
        for (int i = 2; i <= f; i++) {
            dp[i][j % 2] = INT_MAX;
            int left = 0, right = i;
            int mid;
            int ans = INT_MAX;
            while (left < right) {
                mid = (left + right + 1) / 2;
                int broken = dp[mid - 1][(j - 1) % 2];
                int unbroken = dp[i - mid][j % 2];
                if (broken > unbroken) {
                    ans = min(ans, 1 + broken);
                    right = mid - 1;
                }
                else {
                    ans = min(ans, 1 + unbroken);
                    left = mid;
                }
            }
            dp[i][j % 2] = ans;
        }        
    }
    return dp[f][e % 2];
}

// 空间优化+二分查找+动态规划方法
void eggDPBinarySpaceOpt(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggDPBinarySpaceOpt(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}


int __eggRedefineDPOpt(int f, int e) {
    if (f == 1) return 1;
    vector<vector<int>> dp(f + 1, vector<int>(e + 1, 0));
    for (int i = 1; i <= e; i++) {
        dp[1][i] = 1; // 无论是几个鸡蛋，都是只能测一层楼
    }
    int res = -1;
    for (int i = 2; i <= f; i++) {
        for (int j = 1; j <= e; j++) {
            dp[i][j] = 1 + dp[i - 1][j - 1] + dp[i - 1][j];
        }
        if (dp[i][e] >= f) {
            res = i;
            break;
        }
    }
    return res;
}

// 重新定义动态规划方法
void eggRedefineDPOpt(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggRedefineDPOpt(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}

int __eggRedefineDPSpaceOpt(int f, int e) {
    if (f == 1) return 1;
    vector<int> dp(e + 1, 1);
    dp[0] = 0;
    int res = -1;
    for (int i = 2; i <= f; i++) {
        // 注意刷表顺序
        for (int j = e; j >= 1; j--) {
            dp[j] = 1 + dp[j - 1] + dp[j];
        }
        if (dp[e] >= f) {
            res = i;
            break;
        }
    }
    return res;
}

// 重新定义动态规划 + 空间优化方法（刷表）
void eggRedefineDPSpaceOpt(int f, int e) {
    auto start = chrono::high_resolution_clock::now();
    int res = __eggRedefineDPSpaceOpt(f, e);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Result = " << res << "  Time = " << duration << "ms";
}


int main() {
    for (int i = 1; i <= GROUP; i++) {
        cout << "Group " << i << ": " << endl;
        int f = generate_random_Int(0, 10000000);
        int e = generate_random_Int(1, 10000000);
        cout << "楼层数f: " << f << " , 鸡蛋数e: " << e << endl;

        // // 蛮力法
        // cout << "BruteForce : ";
        // bruteForce(f, e);
        // cout << endl;

        // // 普通动态规划方法
        // cout << "eggDP : ";
        // eggDP(f, e);
        // cout << endl;

        // // 普通动态规划方法（先遍历鸡蛋）
        // cout << "eggDP_e : ";
        // eggDP_e(f, e);
        // cout << endl;

        // // 空间优化动态规划方法（滚动数组）
        // cout << "eggDPSpaceOpt : ";
        // eggDPSpaceOpt(f, e);
        // cout << endl;

        // // 二分查找+动态规划方法
        // cout << "eggDPBinaryOpt : ";
        // eggDPBinaryOpt(f, e);
        // cout << endl;

        // // 空间优化+二分查找+动态规划方法
        // cout << "eggDPBinarySpaceOpt : ";
        // eggDPBinarySpaceOpt(f, e);
        // cout << endl;

        // // 重新定义动态规划方法
        // cout << "eggRedefineDPOpt : ";
        // eggRedefineDPOpt(f, e);
        // cout << endl;

        // 重新定义动态规划 + 空间优化方法（刷表）
        cout << "eggRedefineDPSpaceOpt : ";
        eggRedefineDPSpaceOpt(f, e);
        cout << endl;



        cout << "-----------------------------------------------" << endl;
    }


    return 0;
}