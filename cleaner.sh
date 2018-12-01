!#/bin/bash 


find ./ -name "*.txt" | grep -v "validtc.txt|invalidtc.txt|validtc.txt|about.txt|hexHelper.txt|hexHelper.txt|oppcode.txt|regMap.txt" | xargs rm
