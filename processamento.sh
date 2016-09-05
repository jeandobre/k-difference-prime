 #!/bin/bash

echo -e "\nQuantos caracteres?"
read sz

programa="./kdifferenceprime1"
echo $programa 

$programa -a dados/alfa_A0$sz.txt -b dados/beta_0$sz.txt -k 30 -st
$programa -a dados/alfa_0$sz.txt -b dados/alfa_0$sz.txt -k 30 -st
$programa -a dados/alfa_A0$sz.txt -b dados/beta_C0$sz.txt -k 30 -st

programa="./kdifferenceprime2"
echo $programa 

$programa -a dados/alfa_A0$sz.txt -b dados/beta_0$sz.txt -k 30 -st
$programa -a dados/alfa_0$sz.txt -b dados/alfa_0$sz.txt -k 30 -st
$programa -a dados/alfa_A0$sz.txt -b dados/beta_C0$sz.txt -k 30 -st

programa="./kdifferenceprime3"
echo $programa 

$programa -a dados/alfa_A0$sz.txt -b dados/beta_0$sz.txt -k 30 -st
$programa -a dados/alfa_0$sz.txt -b dados/alfa_0$sz.txt -k 30 -st
$programa -a dados/alfa_A0$sz.txt -b dados/beta_C0$sz.txt -k 30 -st

programa="./kdifferenceprime4 -vs1"
echo $programa 

$programa -a dados/alfa_A0$sz.txt -b dados/beta_0$sz.txt -k 30 -st
$programa -a dados/alfa_0$sz.txt -b dados/alfa_0$sz.txt -k 30 -st
$programa -a dados/alfa_A0$sz.txt -b dados/beta_C0$sz.txt -k 30 -st

programa="./kdifferenceprime4 -vs2"
echo $programa 

$programa -a dados/alfa_A0$sz.txt -b dados/beta_0$sz.txt -k 30 -st
$programa -a dados/alfa_0$sz.txt -b dados/alfa_0$sz.txt -k 30 -st
$programa -a dados/alfa_A0$sz.txt -b dados/beta_C0$sz.txt -k 30 -st