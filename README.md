# cse_332_assembler_10_bit
10 bit assembler for cse 332 project processor

##How to run:

    #1 compile the assembler.cpp file
    #2 write your instuctions in a .txt file("filename".txt)
    #3 run assembler.cpp || assembler.o file.
    #4 enter "filename"
    #5 if there is no error in the input file, the output will be saved in a "filename".mc.txt file
    #6 if there is any error the error messages will be shown in the terminal, and there will be no output.



##instruction formats:
    
    add $rd, $rs ,$rt
    sub $rd, $rs, $rt
    and $rd, $rs, $rt  
    or $rd, $rs, $rt
    xor $rd, $rs, $rt
    cmp  $rd, $rs, $rt
    nor $rd, $rs, $rt
    nand $rd, $rs, $rt
    addi $rd, $rs, immediate
    subi $rd, $rs, immediate
    sll $rd, $rs, immediate
    srl $rd, $rs, immediate
    lw $rd, offset($rs)
    sw $rs, offset($rd)
    lui $rd, hex
    andi $rd, $rs, immediate
    
 
 
 
##oppcodes:

    add 	0000    
    sub 	0001
    and 	0010
    or  	0011
    xor 	0100
    cmp	0101
    nor 	0110
    nand	0111
    addi	1000
    subi	1001
    sll 	1010
    srl 	1011
    lw  	1100
    sw  	1101
    lui 	1110
    andi	1111
 
 
 
 
##Extras:

    two example input files are provided
