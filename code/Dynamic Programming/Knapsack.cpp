// 0/1 Knapsack O(nW):
ll kp01(int W, vi& w, vi& v) {
  int n = w.size();
  vl dp(W + 1, 0);
  for (int i = 0; i < n; i++)
    for (int j = W; j >= w[i]; j--) dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
  return dp[W];
}
// Unbounded (inner loop fwd):
ll kpUB(int W, vi& w, vi& v) {
  int n = w.size();
  vl dp(W + 1, 0);
  for (int i = 0; i < n; i++)
    for (int j = w[i]; j <= W; j++) dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
  return dp[W];
}
