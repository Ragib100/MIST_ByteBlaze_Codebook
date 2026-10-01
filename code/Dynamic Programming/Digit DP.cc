// Counts the values in [L, R] with some property: f(R) - f(L - 1).
// State: position, still hugging the upper bound, seen a non-zero digit yet.
string s;          // the upper bound, as a digit string
int dp[20][2][2];

int f(int pos, int tight, int started) {
  if (pos == (int)s.size()) return started;  // <- put the property here
  int& ans = dp[pos][tight][started];
  if (ans != -1) return ans;
  ans = 0;
  int hi = tight ? s[pos] - '0' : 9;
  for (int d = 0; d <= hi; d++)
    ans += f(pos + 1, tight && d == hi, started || d);
  return ans;
}
// memset(dp, -1, sizeof dp); s = to_string(R); f(0, 1, 0);
