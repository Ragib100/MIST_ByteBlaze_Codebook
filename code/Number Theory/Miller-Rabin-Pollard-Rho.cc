using u64 = uint64_t;
using u128 = __uint128_t;

u64 binpower(u64 b, u64 e, u64 mod) {
  u64 r = 1;
  b %= mod;
  while (e) {
    if (e & 1) r = (u128)r * b % mod;
    b = (u128)b * b % mod;
    e >>= 1;
  }
  return r;
}

bool check_composite(u64 n, u64 a, u64 d, int s) {
  u64 x = binpower(a, d, n);
  if (x == 1 || x == n - 1) return false;
  for (int r = 1; r < s; r++) {
    x = (u128)x * x % n;
    if (x == n - 1) return false;
  }
  return true;
}

// These bases make the test deterministic for every n < 2^64.
bool MillerRabin(u64 n) {
  if (n < 2) return false;
  int s = 0;
  u64 d = n - 1;
  while ((d & 1) == 0) {
    d >>= 1;
    s++;
  }
  for (u64 a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
    if (n == a) return true;
    if (check_composite(n, a, d, s)) return false;
  }
  return true;
}

// One non-trivial factor of a composite n. Floyd cycle finding on
// x -> x^2 + c; a c that only finds n itself is retried with the next one.
u64 pollard(u64 n) {
  if (n % 2 == 0) return 2;
  for (u64 c = 1;; c++) {
    auto f = [&](u64 x) { return ((u128)x * x + c) % n; };
    u64 x = 2, y = 2, d = 1;
    while (d == 1) {
      x = f(x);
      y = f(f(y));
      d = __gcd(x > y ? x - y : y - x, n);
    }
    if (d != n) return d;
  }
}

// Full factorisation, unsorted. sort(out.begin(), out.end()) if it matters.
void factor(u64 n, vector<u64>& out) {
  if (n == 1) return;
  if (MillerRabin(n)) {
    out.push_back(n);
    return;
  }
  u64 d = pollard(n);
  factor(d, out);
  factor(n / d, out);
}
