struct SAM {
    struct State { int len=0, link=0, nxt[26]={0}, fpos=0; ll cnt=0; };
    vector<State> st; int sz=2, last=1;
    SAM(int n) { st.resize(2*n+2); }
    // 1-Based indexing. 1 = empty string
    void add(char ch) {
        int c=ch-'a', cur=sz++, p=last, q, cl;
        st[cur].len=st[last].len+1; st[cur].cnt=1; st[cur].fpos=st[cur].len-1;
        for(; p && !st[p].nxt[c]; p=st[p].link) st[p].nxt[c]=cur;
        if(!p) st[cur].link=1;
        else if(st[p].len+1 == st[q=st[p].nxt[c]].len) st[cur].link=q;
        else {
            st[cl=sz++] = st[q]; st[cl].len=st[p].len+1; st[cl].cnt=0;
            for(; p && st[p].nxt[c]==q; p=st[p].link) st[p].nxt[c]=cl;
            st[q].link = st[cur].link = cl;
        }
        last=cur;
    }
 
    void build() {
        vector<int> o(sz-2); iota(all(o), 2);
        sort(all(o), [&](int a, int b){ return st[a].len<st[b].len; });
        for(int i=sz-3; i>=0; i--) st[st[o[i]].link].cnt += st[o[i]].cnt;
    }
 
    // --- Walk string in main ---
    // int u=1; for(char c: s) if(!(u=sam.st[u].nxt[c-'a'])) break;
    // ll occ = u ? sam.st[u].cnt : 0; // Requires build() first
    // int first_start = u ? sam.st[u].fpos - s.length() + 1 : -1; // 0-indexed start pos
 
    // --- Distinct substrings ---
    // ll total=0; for(int i=2;i<sz;i++) total += st[i].len - st[st[i].link].len;
};
