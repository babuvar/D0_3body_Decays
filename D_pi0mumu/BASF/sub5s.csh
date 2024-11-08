#! /bin/tcsh -f

#foreach stream (0 1 2 3 4 5)
#foreach stream (0)


#foreach stream (2)
#foreach stream (0 1 3 4 5)
foreach stream ( 2)

foreach i (charm uds bsbs nonbsbs)

foreach k (43 53 67 69 71)

bsub -q b_index ./script5s.csh $i $stream $k $stream
#./testscript.csh $i $stream $k $stream

end

end

end

