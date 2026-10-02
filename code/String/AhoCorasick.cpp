struct AhoCorasick{
    struct Node{
        int nxt[26]={}, link=0, out_link=0, words=0;
    };
    vector<Node>t; AhoCorasick(){t.emplace_back();}

    void insert(const string&s){
        int v=0;
        for(char c:s){
            int i=c-'a';
            if(!t[v].nxt[i]){t[v].nxt[i]=t.size(); t.emplace_back();}
            v=t[v].nxt[i];
        }
        t[v].words++;
    }
    void build(){
        queue<int> q;
        for(int i=0;i<26;i++) if(t[0].nxt[i]) q.push(t[0].nxt[i]);
        while(!q.empty()){
            int u=q.front(); q.pop();
            t[u].out_link=t[t[u].link].words?t[u].link:t[t[u].link].out_link;
            for(int i=0;i<26;i++){
                if(t[u].nxt[i]){
                    t[t[u].nxt[i]].link=t[t[u].link].nxt[i];
                    q.push(t[u].nxt[i]);
                }
                else t[u].nxt[i]=t[t[u].link].nxt[i];
            }
        }
    }
    // Traverse string s: int u=0; for(char c:s){ u=t[u].nxt[c-'a']; 
    //   int curr=u; while(curr) { ans+=t[curr].words; curr=t[curr].out_link; } }
};
