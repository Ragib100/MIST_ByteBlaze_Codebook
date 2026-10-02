const ll H1=131, H2=137, M1=1e9+7, M2=1e9+9;
struct Hasher{
    int n; string s; vector<pll> h, pw;
    Hasher(const string&s): n(s.size()), s(s), h(n+1), pw(n+1){
        pw[0]={1,1};
		for(int i=1;i<=n;i++)
            pw[i]={pw[i-1].fi*H1%M1,pw[i-1].se*H2%M2};
        h[0]={0,0};
		for(int i=0;i<n;i++)
            h[i+1]={(h[i].fi*H1+s[i])%M1,(h[i].se*H2+s[i])%M2};
	}
    pll get(int l,int r){
		++r;// 0-indexed [l,r]
        return{(h[r].fi-h[l].fi*pw[r-l].fi%M1+M1*2)%M1,
               (h[r].se-h[l].se*pw[r-l].se%M2+M2*2)%M2};
	}
    int lcp(int i1,int i2,int n1,int n2){
		// LCP of s[i1..] and s[i2..]
        int lo=0,hi=min(n1,n2),ans=0;
        while(lo<=hi){
			int m=(lo+hi)/2;
            if(get(i1,i1+m-1)==get(i2,i2+m-1)){ans=m; lo=m+1;}
			else hi=m-1;
		}
        return ans;
	}
};
