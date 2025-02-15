package org.overmindv.components

import org.overmindv.models.Computer
import org.overmindv.models.Table
import org.overmindv.services.Zoo

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Component
import kotlin.system.exitProcess

@Component
class ThingAdder @Autowired constructor(private val zoo: Zoo) {
    fun addThing() {
        var thingType = ""
        var number: Int? = null

        while (true) {
            val chooseThingHint = """
                Choose thing:
                    1. Computer
                    2. Table
                    0. Exit
            """.trimIndent()
            println(chooseThingHint)

            var isCorrect = true
            when (readLine()?.toIntOrNull()) {
                1 -> thingType = "Computer"
                2 -> thingType = "Table"
                0 -> exitProcess(0)
                else -> {
                    isCorrect = false
                    println(colorize("Incorrect input.", Color.RED))
                }
            }

            if (isCorrect) {
                break
            }
        }

        while (number == null || number <= 0) {
            print("Input number for thing: ")
            val input = readln()
            number = input.toIntOrNull()

            if (number == null || number <= 0) {
                println(colorize("Incorrect input. Number must be greater that 0.", Color.RED))
            }
        }

        when (thingType) {
            "Computer" -> zoo.addThing(Computer(number))
            "Table" -> zoo.addThing(Table(number))
        }
        println(colorize("$thingType is added to the zoo.", Color.GREEN))
    }
}