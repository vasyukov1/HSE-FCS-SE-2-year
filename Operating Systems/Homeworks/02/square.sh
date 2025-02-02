#!/bin/bash
# Outputs a countdown of squares of numbers, starting from the number entered by the user.

number=$1

if ! [[ $number =~ ^[0-9]+$ ]]; then
    echo "Please, send the positive number."
    exit 1
fi

while [ $number -ge 0 ]; do
    sq=$(echo $number^2 | bc) 
    echo "$number^2 = $sq"
    number=$((number - 1))
    sleep 1
done 

echo "Countdown is done!"
