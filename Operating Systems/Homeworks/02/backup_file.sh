#!/bin/bash
# Backup of the file.

backup() {
    if [ ! -f "$1" ]; then
        echo "File '$1' not found."
        return 1
    fi
    timestamp=$(date +"%d%m%Y_%H%M%S")
    backup_name="${1}_backup_${timestamp}"
    cp "$1" "$backup_name"
    if [ $? -eq 0 ]; then
        echo "The backup of file '$1' is creating with name '$backup_name'."
    else
        echo "Error creating backup."
    fi
}

confirm() {
    read -p "Do you want to create the backup of file '$1'? (y/n): " answer
    if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
        backup "$1"
    else 
        echo "Backup canceled."
    fi
}

if [ -z "$1" ]; then
    echo "You need to enter the name of file"
    exit 1
fi

confirm "$1"
