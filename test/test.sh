runtest () {
    ../lilgest t/$1.lil

    if [ ! "$?" -eq 0 ]
    then
        printf "fail"
        return
    fi
    printf "ok"
}

check () {
    NSPACES=$(expr 16 - ${#1})
    printf "%s:%"$NSPACES"s\n" $1 $(runtest $1)
}

check p0
check p1
check p2
check p3
check p4
check p5
check p6
check example
check gestlang
check ramp
check repeat
check return
check scalar
check seqphrase
check shrinkgrow
check shuf
check skew
check skew2
check smoothstep
check synthwave
check tick
check metaphrase
check metabehavior
check metanode
check metatarget
