#!/usr/bin/env zsh
ulimit -H -m 2048000
ulimit -H -v 2048000
outdir=bin
mkdir -p $outdir/gcc
mkdir -p $outdir/clang
echo "#g++ version: $(g++ --version | head -n 1)"
echo "#clang++ version: $(clang++ --version | head -n 1)"
echo '#compiler[1];__NMAX__[2];__TVAR__[3];__CVAR__[4];compile_time(s)[5];memory_use(kb)[6];exit_status[7]'
for nmax in $(seq 16)
do
    for tvar in {'int8_t','int16_t','int32_t','int64_t','uint8_t','uint16_t','uint32_t','uint64_t'}
    do
        for cvar in {'int32_t','int64_t','uint32_t','uint64_t','float','double','long double'}
        do
            out=$(echo "$nmax $tvar $cvar" | sed -e 's/ /_/g')
            gcc_time=$(/usr/bin/time --quiet -f "%e;%M;%x\n" -o /dev/stdout g++ -D__NMAX__=$nmax -D__TVAR__=$tvar -D__CVAR__=$cvar -Wall -Wextra -std=c++17 -O3 -flto -o $outdir/gcc/gcc_$out ts.cc &)
            clang_time=$(/usr/bin/time --quiet -f "%e;%M;%x\n" -o /dev/stdout clang++ -fconstexpr-steps=-1 -D__NMAX__=$nmax -D__TVAR__=$tvar -D__CVAR__=$cvar -Wall -Wextra -std=c++17 -O3 -flto -o $outdir/clang/clang_$out ts.cc &)
            wait
            echo "\"g++\";$nmax;\"$tvar\";\"$cvar\";$gcc_time"
            echo "\"clang++\";$nmax;\"$tvar\";\"$cvar\";$clang_time"
        done
    done
done
exit 0
