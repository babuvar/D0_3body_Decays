#! /bin/tcsh -f

#foreach stream (0 1 2 3 4 5)

#foreach stream (2)
#foreach stream (0 1 3 4 5)
foreach stream ( 2)

#foreach i (charged charm mixed uds)
foreach i (charm)

#foreach j (11 13 15 17 19 21 23 25 27 7 9)

#bsub -q b_index ./script4s.csh $i 1$stream $j $stream


#end

#foreach k (31 33 35 37 39 41 43 45 47 49 51 55 61 63 65)
foreach k (45)

bsub -q s ./script4s.csh $i $stream $k $stream
#echo "bsub -q s ./script4s.csh $i $stream $k $stream"

end

end

end

