out/uncomment $1 > $1.tmp.1
out/ecpp $1.tmp.1 > $1.tmp.2
rm $1.tmp.1
gcc $1.tmp.2 -o $2 -Wall -lm
