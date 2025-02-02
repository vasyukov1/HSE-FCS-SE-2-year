#!/bin/bash
# Count the Fibonacci number which enter the user.

if [ -z "$1" ]; then
    echo "You need to enter the number!"
    exit 1
fi

n=$1

fibonacci() {
    local num=$1
    if [ "$num" -eq 0 ]; then
        echo 0
    elif [ "$num" -eq 1 ]; then
        echo 1
    else
        local a=$(fibonacci $((num - 1)))
        local b=$(fibonacci $((num - 2)))
        echo $((a + b))
    fi
}

result=$(fibonacci $n)
echo "F($n) = $result"
