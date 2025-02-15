package org.overmindv.components

import org.springframework.stereotype.Component

@Component
class InputHandler {
    fun readInt(prompt: String, errorMessage: String, validation: (Int) -> Boolean): Int {
        while (true) {
            print(prompt)
            val input = readLine()?.toIntOrNull()
            if (input != null && validation(input)) {
                return input
            }
            println(colorize(errorMessage, Color.RED))
        }
    }
}