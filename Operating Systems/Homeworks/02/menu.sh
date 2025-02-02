#!/bin/bash
# Counter of the bill.

bill=0

print_menu() {
    echo "Choose the dish:"
    echo "1. Salad"
    echo "2. Borsch"
    echo "3. Pasta"
    echo "4. Dessert"
    echo "0. Check the bill."
}

add_salad() {
    salad=1.5
    bill=$(echo "scale=2; $bill + $salad" | bc)
    echo "Salad: $salad euro"
}

add_borsch() {
    borsch=0.5
    bill=$(echo "scale=2; $bill + $borsch" | bc)
    echo "Borsch: $borsch euro."
}

add_pasta() {
    pasta=2
    bill=$(echo "scale=2; $bill + $pasta" | bc)
    echo "Pasta: $pasta euro."
}

add_desert() {
    dessert=1
    bill=$(echo "scale=2; $bill + $dessert" | bc)
    echo "Dessert: $dessert euro."
}

get_bill() {
    echo "The bill is $bill euro."
}

while true; do
    print_menu
    read -p "Enter the number of option: " choice
    if [ "$choice" -eq 1 ]; then
        add_salad
    elif [ "$choice" -eq 2 ]; then
        add_borsch
    elif [ "$choice" -eq 3 ]; then
        add_pasta
    elif [ "$choice" -eq 4 ]; then
        add_desert
    elif [ "$choice" -eq 0 ]; then
        get_bill
        break
    else
        echo "Use correct options. From 0 to 4."
    fi
    echo
done
