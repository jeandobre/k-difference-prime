 #!/bin/bash

echo -e "\nQual a versão do programa que deseja testar (1, 2, 3 ou 4)?"
read versao

programa="./kdifferenceprime"
programa="$programa$versao"

echo $programa

$programa -a CAACAC -b CGTCCTGCC -k 3
$programa -a CAACACCC -b CGTCCTGCC -k 3
$programa -a CAACACC -b CCCGTGCCC -k 3
$programa -a CGAAGCAAGC -b AAAAAAAAAAAAAAAA -k 6
$programa -a ACGAAAAGCAAACCAA -b AAAAAAAAAAAAAAAAAAAA -k 6
$programa -a ACGGCAG -b AACGAAAGG -k 2
$programa -a CCCGGCCC -b CCCGTGCCC -k 1
$programa -a CCC -b TTT -k 3
$programa -a TACTGACTG -b TACTGACTG -k 5
$programa -a AAAAA -b AAAAA -k 2
$programa -a CCC -b TTT -k 4
$programa -a CCC -b TTT -k 0
$programa -a ACGT -b TGCA -k 1
$programa -a ACGT -b TGCA -k 2

$programa -a ACGT -b TGCA -k 2 -vs0
$programa -a ACGT -b TGCA -k 2 -sm
$programa -a ACGT -b TGCA -k 2 -st
$programa -a ACGT -b TGCA -k 2 -aaaaaa
$programa -a ACGT -b TGCA -k 2 -a AACCG
$programa -a ACGT -b TGCA -k 2 -ts 2
$programa -a ACGT -b TGCA -k 2 -log

