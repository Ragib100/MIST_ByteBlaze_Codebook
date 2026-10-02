vector<int> prefix_function(const string&p){
    int m=p.size(); vector<int>f(m,0);
    for(int i=1,j=0;i<m;i++){
        while(j>0&&p[i]!=p[j]) j=f[j-1];
        if(p[i]==p[j])j++;f[i]=j;
    }
  return f;
}
vector<int> kmpSearch(const string&text,const string&pat){
    auto f=kmpFail(pat); int m=pat.size(); vector<int>res;
    for(int i=0,j=0;i<(int)text.size();i++){
        while(j>0&&text[i]!=pat[j]) j=f[j-1];
        if(text[i]==pat[j]) j++;
        if(j==m){res.push_back(i-m+1); j=f[j-1];}
    }
    return res;
}
// Period of s: len-f[len-1] (divides len)
