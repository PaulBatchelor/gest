if [ "$#" -lt 2 ]
then
    printf "Usage: $0 in.lil out.lil\n"
    exit 1
fi

IN=$1
OUT=$2
sed -e "s/^wavout.*$/verify #md5/" -e "/^computes/d" < $IN > $OUT
#printf "#md5 verify %s\n" $(../lilgest $OUT) >> $OUT
MD5=$(../lilgest $OUT)
sed -e "s/^verify #md5/verify $MD5/" -i $OUT
