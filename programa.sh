make

# Lista de instâncias
instancias=(
  "n9k5_A.txt"
  "n9k5_B.txt"
  "n9k5_C.txt"
  "n9k5_D.txt"
  "n14k5_A.txt"
  "n14k5_B.txt"
  "n14k5_C.txt"
  "n14k5_D.txt"
  "n22k3_A.txt"
  "n22k3_B.txt"
  "n22k3_C.txt"
  "n22k3_D.txt"
  "n31k5_A.txt"
  "n31k5_B.txt"
  "n31k5_C.txt"
  "n31k5_D.txt"
  "n43k6_A.txt"
  "n43k6_B.txt"
  "n43k6_C.txt"
  "n43k6_D.txt"
  "n64k9_A.txt"
  "n64k9_B.txt"
  "n64k9_C.txt"
  "n64k9_D.txt"
  "n120k7_A.txt"
  "n120k7_B.txt"
  "n120k7_C.txt"
  "n120k7_D.txt"
  "n199k17_A.txt"
  "n199k17_B.txt"
  "n199k17_C.txt"
  "n199k17_D.txt"
)


valores=(
    428
    506
    559
    408
    471
    565
    569
    471
    605
    777
    777
    605
    650
    933
    939
    656
    801
    1203
    1208
    802
    934
    1503
    1510
    932
    1029
    2052
    2040
    1046
    1672
    3302
    3301
    1672
)


# Loop para iterar através das instâncias
for ((i=0; i<${#instancias[@]}; i++))
do
  
    arquivoDeEntrada="instancias/${instancias[$i]}"
    valorInstancia="${valores[$i]}"

  echo "Executando $arquivoDeEntrada com melhor valor  $valorInstancia"
  ./programa "$arquivoDeEntrada" "$valorInstancia"
done


# Executar o programa compilado
./programa

