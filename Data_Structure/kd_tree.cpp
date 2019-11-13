// 寻找近点
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 5;
typedef long long ll;

namespace KD_Tree {

const int DIM = 2;

inline ll sqr(int x) { return 1LL * x * x; }

struct Point {
  int x[DIM], id, c;

  ll dist2(const Point &b) const {
    return sqr(x[0] - b.x[0]) + sqr(x[1] - b.x[1]);
  }
};
struct QNode {
  Point p;
  ll dis2;

  QNode() {}
  QNode(Point _p, ll _dis2) : p(_p), dis2(_dis2) {}

  bool operator<(const QNode &b) const {
    return dis2 < b.dis2 || (dis2 == b.dis2 && p.id < b.p.id);
  }
} ans;
struct cmpx {
  int div;
  cmpx(int _div) : div(_div) {}
  bool operator()(const Point &a, const Point &b) {
    for (int i = 0; i < DIM; ++i)
      if (a.x[(i + div) % DIM] != b.x[(i + div) % DIM])
        return a.x[(i + div) % DIM] < b.x[(i + div) % DIM];
    return true;
  }
};

bool cmp(const Point &a, const Point &b, int div) {
  cmpx cp = cmpx(div);
  return cp(a, b);
}

struct Node {
  Point e;
  Node *lc, *rc;
  int div;
} node_pool[MAXN], *tail, *root;
void init() { tail = node_pool; }
Node *build(Point *a, int l, int r, int div) {
  if (l >= r) return nullptr;
  Node *p = tail++;
  p->div = div;
  int mid = (l + r) >> 1;
  nth_element(a + l, a + mid, a + r, cmpx(div));
  p->e = a[mid];
  p->lc = build(a, l, mid, div ^ 1);
  p->rc = build(a, mid + 1, r, div ^ 1);
  return p;
}
void search(Point p, Node *x, int div) {
  if (!x) return;
  if (cmp(p, x->e, div)) {
    search(p, x->lc, div ^ 1);
    if (ans.dis2 == -1) {
      if (x->e.c <= p.c) ans = QNode(x->e, p.dist2(x->e));
      search(p, x->rc, div ^ 1);
    } else {
      QNode temp(x->e, p.dist2(x->e));
      if (x->e.c <= p.c && temp < ans) ans = temp;
      if (sqr(x->e.x[div] - p.x[div]) <= ans.dis2) search(p, x->rc, div ^ 1);
    }
  } else {
    search(p, x->rc, div ^ 1);
    if (ans.dis2 == -1) {
      if (x->e.c <= p.c) ans = QNode(x->e, p.dist2(x->e));
      search(p, x->lc, div ^ 1);
    } else {
      QNode temp(x->e, p.dist2(x->e));
      if (x->e.c <= p.c && temp < ans) ans = temp;
      if (sqr(x->e.x[div] - p.x[div]) <= ans.dis2) search(p, x->lc, div ^ 1);
    }
  }
}
void search(Point p) {
  ans.dis2 = -1;
  search(p, root, 0);
}
} // namespace KD_Tree

void solve() {
  static KD_Tree::Point p[MAXN];
  int n, m;
  cin >> n >> m;
  for (int i = 0; i < n; ++i) {
    p[i].id = i;
    cin >> p[i].x[0] >> p[i].x[1] >> p[i].c;
  }
  KD_Tree::init();
  KD_Tree::root = KD_Tree::build(p, 0, n, 0);

  for (KD_Tree::Point q; m; --m) {
    cin >> q.x[0] >> q.x[1] >> q.c;
    KD_Tree::search(q);
    cout << KD_Tree::ans.p.x[0] << ' ' << KD_Tree::ans.p.x[1] << ' '
         << KD_Tree::ans.p.c << '\n';
  }
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int o_o;
  for (cin >> o_o; o_o; --o_o) solve();

  return 0;
}

// 寻找远点
inline void cmin(int &a, int b) { b < a ? a = b : 1; }
inline void cmax(int &a, int b) { a < b ? a = b : 1; }
inline int ibs(int a) { return a < 0 ? -a : a; }
struct D {
  int d[2], mx0, mx1, mi0, mi1;
  D *l, *r;
} t[N], *rt;
int cpd, ans;
inline bool cmp(const D &a, const D &b) {
  return (a.d[cpd] ^ b.d[cpd]) ? a.d[cpd] < b.d[cpd]
                               : a.d[cpd ^ 1] < b.d[cpd ^ 1];
}
inline void kd_upd(D *u) {
  if (u->l) {
    cmax(u->mx0, u->l->mx0);
    cmax(u->mx1, u->l->mx1);
    cmin(u->mi0, u->l->mi0);
    cmin(u->mi1, u->l->mi1);
  }
  if (u->r) {
    cmax(u->mx0, u->r->mx0);
    cmax(u->mx1, u->r->mx1);
    cmin(u->mi0, u->r->mi0);
    cmin(u->mi1, u->r->mi1);
  }
}
D *kd_bld(int l, int r, int d) {
  int m = l + r >> 1;
  cpd = d;
  std::nth_element(t + l + 1, t + m + 1, t + r + 1, cmp);
  t[m].mx0 = t[m].mi0 = t[m].d[0];
  t[m].mx1 = t[m].mi1 = t[m].d[1];
  if (l ^ m) t[m].l = kd_bld(l, m - 1, d ^ 1);
  if (r ^ m) t[m].r = kd_bld(m + 1, r, d ^ 1);
  kd_upd(t + m);
  return t + m;
}
inline void kd_ins(D *ne) {
  int cd = 0;
  D *u = rt;
  while (true) {
    cmax(u->mx0, ne->mx0), cmin(u->mi0, ne->mi0);
    cmax(u->mx1, ne->mx1), cmin(u->mi1, ne->mi1);
    if (ne->d[cd] < u->d[cd]) {
      if (u->l)
        u = u->l;
      else {
        u->l = ne;
        return;
      }
    } else {
      if (u->r)
        u = u->r;
      else {
        u->r = ne;
        return;
      }
    }
    cd ^= 1;
  }
}
inline int dist(int x, int y, D *u) {
  int r = 0;
  if (x < u->mi0)
    r = u->mi0 - x;
  else if (x > u->mx0)
    r = x - u->mx0;
  if (y < u->mi1)
    r += u->mi1 - y;
  else if (y > u->mx1)
    r += y - u->mx1;
  return r;
}
inline void kd_quy(D *u, const int &x, const int &y) {
  int dl, dr, d0;
  d0 = ibs(u->d[0] - x) + ibs(u->d[1] - y);
  if (d0 < ans) ans = d0;
  dl = u->l ? dist(x, y, u->l) : inf;
  dr = u->r ? dist(x, y, u->r) : inf;
  if (dl < dr) {
    if (dl < ans) kd_quy(u->l, x, y);
    if (dr < ans) kd_quy(u->r, x, y);
  } else {
    if (dr < ans) kd_quy(u->r, x, y);
    if (dl < ans) kd_quy(u->l, x, y);
  }
}
