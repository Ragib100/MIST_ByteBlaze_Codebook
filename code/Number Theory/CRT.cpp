// x == r1(mod m1), x == r2(mod m2) -> {x, lcm} or {-1,-1}
pll crt(ll r1,ll m1,ll r2,ll m2){
    ll g=__gcd(m1,m2);
    if((r2-r1)%g!=0)return{-1,-1};
    ll x,y; extgcd(m1/g,m2/g,x,y);
    ll m=m1/g*m2;
    ll ans=(r1+((__int128)(r2-r1)/g%(m2/g)*x%(m2/g))*m1)%m;
    return{(ans%m+m)%m,m};
}
