package org.overmindv.components

enum class Color(val code: String) {
    RED("\u001B[31m"),
    GREEN("\u001B[32m"),
    YELLOW("\u001B[33m"),
    BLUE("\u001B[34m"),
    PURPLE("\u001B[35m"),
    CYAN("\u001B[36m"),
    WHITE("\u001B[37m"),
    RESET("\u001B[0m")
}

fun colorize(text: String, color: Color): String {
    return "${color.code}$text${Color.RESET.code}"
}