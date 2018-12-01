!#/bin/bash 


find ./ -name "*.txt" -not -name "validtc.txt || invalidtc.txt || validtc.txt || about.txt || hexHelper.txt || hexHelper.txt || oppcode.txt || regMap.txt" | xargs rm
