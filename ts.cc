#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>

#ifndef __NMAX__
#define __NMAX__ 4
#endif

#ifndef __TVAR__
#define __TVAR__ int32_t
#endif

#ifndef __CVAR__
#define __CVAR__ uint32_t
#endif

#ifndef __TRIVIAL__
constexpr bool self_avoiding=true;
#else
constexpr bool self_avoiding=false;
#endif

constexpr __TVAR__ NMAX=__NMAX__;

template<class Y,Y x,Y y> struct grid
{
	template<class...Nodes> static constexpr bool value(const Y u,const Y v,const Nodes...nodes)
    {
#ifdef __GNUC__
#ifndef __G_SPY__
        (void) u;
        (void) v;
#endif
#endif
        if(value(u,v) or value(nodes...)) return true;
        else return false;
    }
	static constexpr bool value(const Y u,const Y v)
    {
#ifdef __GNUC__
#ifndef __G_SPY__
        (void) u;
        (void) v;
#endif
#endif
        if(u==x and v==y) return true;
        else return false;
    }
};

template<class X,class Y,Y x,Y y,Y n,Y N> struct walker
{
    template<class...Nodes> static constexpr X move(X z,const Y u,const Y v,const Nodes...nodes)
	{
#ifdef __GNUC__
#ifndef __G_SPY__
        (void) u;
        (void) v;
#endif
#endif
		if constexpr(n==N) return z+static_cast<X>(1);
		else if constexpr(self_avoiding)
		{
			if(!grid<Y,x+1,y>::value(u,v,nodes...)) z=walker<X,Y,x+1,y,n+1,N>::move(z,x,y,u,v,nodes...);
			if(!grid<Y,x-1,y>::value(u,v,nodes...)) z=walker<X,Y,x-1,y,n+1,N>::move(z,x,y,u,v,nodes...);
			if(!grid<Y,x,y+1>::value(u,v,nodes...)) z=walker<X,Y,x,y+1,n+1,N>::move(z,x,y,u,v,nodes...);
			if(!grid<Y,x,y-1>::value(u,v,nodes...)) z=walker<X,Y,x,y-1,n+1,N>::move(z,x,y,u,v,nodes...);
            return z;
		}
		else
		{
			z=walker<X,Y,x+1,y,n+1,N>::move(z,x,y,u,v,nodes...);
			z=walker<X,Y,x-1,y,n+1,N>::move(z,x,y,u,v,nodes...);
			z=walker<X,Y,x,y+1,n+1,N>::move(z,x,y,u,v,nodes...);
			z=walker<X,Y,x,y-1,n+1,N>::move(z,x,y,u,v,nodes...);
            return z;
		}
	}
    static constexpr X move(X z,const Y u,const Y v)
	{
#ifdef __GNUC__
#ifndef __G_SPY__
        (void) u;
        (void) v;
#endif
#endif
		if constexpr(n==N) return z+static_cast<X>(1);
		else if constexpr(self_avoiding)
		{
			if(!grid<Y,x+1,y>::value(u,v)) z=walker<X,Y,x+1,y,n+1,N>::move(z,x,y,u,v);
			if(!grid<Y,x-1,y>::value(u,v)) z=walker<X,Y,x-1,y,n+1,N>::move(z,x,y,u,v);
			if(!grid<Y,x,y+1>::value(u,v)) z=walker<X,Y,x,y+1,n+1,N>::move(z,x,y,u,v);
			if(!grid<Y,x,y-1>::value(u,v)) z=walker<X,Y,x,y-1,n+1,N>::move(z,x,y,u,v);
            return z;
		}
        else
		{
			z=walker<X,Y,x+1,y,n+1,N>::move(z,x,y,u,v);
			z=walker<X,Y,x-1,y,n+1,N>::move(z,x,y,u,v);
			z=walker<X,Y,x,y+1,n+1,N>::move(z,x,y,u,v);
			z=walker<X,Y,x,y-1,n+1,N>::move(z,x,y,u,v);
            return z;
		}
	}
	static constexpr X move(X z)
	{
		if constexpr(n==N) return z+static_cast<X>(1);
		else
		{
			z=walker<X,Y,x+1,y,n+1,N>::move(z,x,y);
			z=walker<X,Y,x-1,y,n+1,N>::move(z,x,y);
			z=walker<X,Y,x,y+1,n+1,N>::move(z,x,y);
			z=walker<X,Y,x,y-1,n+1,N>::move(z,x,y);
            return z;
		}
	}
};

template<class X,class Y,Y N> struct saw
{
	const X z;
    constexpr saw():z(walker<X,Y,NMAX,NMAX,static_cast<Y>(0),N>::move(static_cast<X>(0))){}
};

template<class X,class Y,Y N> struct saws
{
    saws()
    {
        if constexpr(N>static_cast<Y>(0)) saws<X,Y,N-1> tmp;
        constexpr saw<X,Y,N> sw;
        std::printf("%" PRIuMAX ";%" PRIuMAX "\n",static_cast<uintmax_t>(N),static_cast<uintmax_t>(sw.z));
    }
};

int main()
{
    std::printf("#N[1];Z[2]\n");
    saws<__CVAR__,__TVAR__,NMAX-1> sws;
    return EXIT_SUCCESS;
}
