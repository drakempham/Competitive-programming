#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class FenwickCount {
public:
    int n;
    vector<int> bit;
    int max_step;


    FenwickCount(int n) {
        this->n = n;
        bit.assign(n + 1, 0);
        max_step = 1;
        while (max_step * 2 <= n) max_step *= 2;
    }

    void add(int i, int val) {
        for (; i <= n; i += i & -i) bit[i] += val;
    }

    int query(int i) {
        int total = 0;
        for (; i > 0; i -= i & -i) total += bit[i];
        return total;
    }

    int kthSmall(int k_val) {
        int idx = 0;
        for (int step = max_step; step > 0; step >>= 1) {
            int next_step = idx + step;
            if (next_step <= n && bit[next_step] < k_val) {
                idx = next_step;
                k_val -= bit[next_step];
            }
        }
        return idx + 1;
    }

    int kthSmall_diff(const FenwickCount& other, int k_val) {
        int idx = 0;


        for (int step = max_step; step > 0; step >>= 1) {
              int next_step = idx + step;
            if (next_step <= n) {
                int val = bit[next_step] - other.bit[next_step];
                if (val < k_val) {
                    idx = next_step;
                    k_val -= val;
                }
            }
        }
        return idx + 1;
    }
};

class FenwickSum {
public:
    int n;
    vector<long long> bit;

    FenwickSum(int n) {
        this->n = n;
        bit.assign(n + 1, 0);
    }

    void add(int i, long long val) {
        for (; i <= n; i += i & -i) bit[i] += val;
    }

    long long query(int i) {
        long long total = 0;
        for (; i > 0; i -= i & -i) total += bit[i];
        return total;
    }
};

class Solution {
public:
    long long maxSum(vector<int>& nums, int k) {
        int n = nums.size();
        
        vector<int> vals = nums;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int m = vals.size();

        unordered_map<int, int> pos;
        for (int i = 0; i < m; ++i) {
            pos[vals[i]] = i + 1;
        }
        FenwickCount count(m);
        FenwickSum full_sum(m);

        long long total_sum = 0;
        for (int num : nums) {
            int idx = pos[num];
            count.add(idx, 1);
            full_sum.add(idx, num);
            total_sum += num;
        }

        long long ans = -1e18; 

        auto curr_sum = [&](FenwickCount& count_bit, FenwickSum& sum_bit, int t) -> long long {
            if (t <= 0) return 0;
            int idx = count_bit.kthSmall(t);
            int cnt_b = count_bit.query(idx - 1);
            long long sum_b = sum_bit.query(idx - 1);
            int need = t - cnt_b;
            return sum_b + (long long)need * vals[idx - 1];
        };

        for (int l = 0; l < n; ++l) {
            FenwickCount i_count(m);
            FenwickSum i_sum(m);
            long long curr = 0;

            for (int r = l; r < n; ++r) {
                int num = nums[r];
                int idx = pos[num];

                i_count.add(idx, 1);
                i_sum.add(idx, num);
                curr += num;

                int length = r - l + 1;
                long long candi = curr;
                int out_cnt = n - length;
                int limit = min({k, length, out_cnt});

                if (limit > 0) {
                    int lo = 1, hi = limit;
                    int best_temp = 0;

                    while (lo <= hi) {
                        int mid = lo + (hi - lo) / 2;

                        int in_idx = i_count.kthSmall(mid);
                        int out_idx = count.kthSmall_diff(i_count, out_cnt - mid + 1);

                        if (vals[out_idx - 1] > vals[in_idx - 1]) {
                            best_temp = mid;
                            lo = mid + 1;
                        } else {
                            hi = mid - 1;
                        }
                    }

                    if (best_temp > 0) {
                        long long small_in = curr_sum(i_count, i_sum, best_temp);

                        int t_out = out_cnt - best_temp;
                        long long sum_out_t = 0;

                        if (t_out > 0) {
                            int out_idx = count.kthSmall_diff(i_count, t_out);
                            int cnt_b = count.query(out_idx - 1) - i_count.query(out_idx - 1);
                              int need = t_out - cnt_b;
                            sum_out_t = full_sum.query(out_idx - 1) - i_sum.query(out_idx - 1) + (long long)need * vals[out_idx - 1];
                        }

                        long long big_out = (total_sum - curr) - sum_out_t;
                          candi = curr + big_out - small_in;
                    }
                }
                
                if (candi > ans) {
                    ans = candi;
                }
            }
        }
        return ans;
    }
};