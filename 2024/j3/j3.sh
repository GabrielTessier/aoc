echo "Partie 1 :"
cat input.txt | grep -o "mul([0-9][0-9]*,[0-9][0-9]*)" | sed 's/mul(//g' | sed 's/)/ * p/g' | sed 's/,/ /g' | dc | awk 'BEGIN {s=0} {s+=$1} END {printf "%.0f\n", s}'

echo "\nPartie 2 :"
cat input.txt | tr -d '\n' | sed -e "s/^/do()/" | sed -e "s/don't()/\n/g" | perl -p -e 's/^.*?do\(\)/do\(\)/' | grep "^do()" | grep -o "mul([0-9][0-9]*,[0-9][0-9]*)" | sed 's/mul(//g' | sed 's/)/ * p/g' | sed 's/,/ /g' | dc | awk 'BEGIN {s=0} {s+=$1} END {printf "%.0f\n", s}'