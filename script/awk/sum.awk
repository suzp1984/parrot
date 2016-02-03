#!/bin/awk -f
# sum up repeat rows
# by awk script
# awk -f sum.awk codes_file.txt | sort -t, -k2 -n
# following is a sample code
# Zhu Ran,9469,5122,4347
# Zhu Ran,1,1,1
# wanghao,96,2,94
# qiyancai,961,338,623

BEGIN {
    #print "# Code sum"
    FS=","
}

NF != 4 {
    # print "error line field numbers"
}

NF == 4 {
    # find array location first by "$1"
    if ($1 in incs)
    {
        incs[$1] += $2
        decs[$1] += $3
        sums[$1] += $4
        users[$1]++
    } else {
        incs[$1] = $2
        decs[$1] = $3
        sums[$1] = $4
        users[$1] = 1
    }
    
}

END {
    #print "# print the result"
    
    for (i in incs) {
        print i "," incs[i] "," decs[i] "," sums[i]
    }

    #num = asorti(incs, indices)
    #for (i=1; i<num; i++) {
    #    print indices[i] "," incs[indices[i]]
    #}
}
