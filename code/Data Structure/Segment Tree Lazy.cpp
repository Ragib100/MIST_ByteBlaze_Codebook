struct SegTree {
  int n;
  vector<ll> tree, lazy;
  const ll NEUTRAL = 0; 
  
  SegTree(int n) : n(n), tree(4 * n, NEUTRAL), lazy(4 * n, 0) {}
  
  ll combine(ll left, ll right) {
    return left + right; 
  }

  void push(int v, int tl, int tr) {
    if (lazy[v] == 0) return;
    int m = (tl + tr) / 2;
    tree[2 * v] += lazy[v] * (m - tl + 1);
    lazy[2 * v] += lazy[v];
    tree[2 * v + 1] += lazy[v] * (tr - m);
    lazy[2 * v + 1] += lazy[v];
    lazy[v] = 0;
  }
  
  void build(const vector<int>& a, int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = n - 1;
    if (tl == tr) {
      tree[v] = a[tl];
      return;
    }
    int m = (tl + tr) / 2;
    build(a, 2 * v, tl, m);
    build(a, 2 * v + 1, m + 1, tr);
    tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
  }
  
  void update(int l, int r, int val, int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = n - 1;
    if (l > tr || r < tl) return;
    if (l <= tl && tr <= r) {
      tree[v] += 1LL * val * (tr - tl + 1); 
      lazy[v] += val;
      return;
    }
    push(v, tl, tr);
    int m = (tl + tr) / 2;
    update(l, r, val, 2 * v, tl, m);
    update(l, r, val, 2 * v + 1, m + 1, tr);
    tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
  }
  
  ll query(int l, int r, int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = n - 1;
    if (l > tr || r < tl) return NEUTRAL; 
    if (l <= tl && tr <= r) return tree[v];
    push(v, tl, tr);
    int m = (tl + tr) / 2;
    return combine(query(l, r, 2 * v, tl, m), query(l, r, 2 * v + 1, m + 1, tr));
  }
  
  void pointSet(int pos, int val, int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = n - 1;
    if (tl == tr) {
      tree[v] = val;
      lazy[v] = 0;
      return;
    }
    push(v, tl, tr);
    int m = (tl + tr) / 2;
    if (pos <= m)
      pointSet(pos, val, 2 * v, tl, m);
    else
      pointSet(pos, val, 2 * v + 1, m + 1, tr);
    tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
  }
};
