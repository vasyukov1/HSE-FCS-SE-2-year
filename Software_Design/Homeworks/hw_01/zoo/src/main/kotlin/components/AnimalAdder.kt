package org.overmindv.components

import org.overmindv.models.*
import org.overmindv.services.Zoo
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Component
import kotlin.system.exitProcess

@Component
class AnimalAdder @Autowired constructor(
    private val zoo: Zoo,
    private val inputHandler: InputHandler
) {
    fun chooseAnimal() {
        val chooseAnimalMenu = """
            Choose animal:
                1. Herbo
                2. Predator
                0. Exit
        """.trimIndent()

        println(chooseAnimalMenu)
        when (inputHandler.readInt("Choose: ", "Invalid option.", { it in 0..2 })) {
            1 -> addHerbo()
            2 -> addPredator()
            0 -> exitProcess(0)
        }
    }

    private fun addHerbo() {
        val herboType = chooseHerbo()
        val food = inputHandler.readInt("Input food for animal: ", "Incorrect input. Number must be greater than 0.", { it > 0 })
        val number = inputHandler.readInt("Input number for animal: ", "Incorrect input. Number must be greater than 0.", { it > 0 })
        val kind = inputHandler.readInt("Input level of kindness for animal: ", "Incorrect input. Number must be from 0 to 10.", { it in 0..10 })

        when (herboType) {
            "Rabbit" -> zoo.addAnimal(Rabbit(food, number, kind))
            "Monkey" -> zoo.addAnimal(Monkey(food, number, kind))
        }
    }

    private fun chooseHerbo(): String {
        val chooseHerboHint = """
            Choose herbo:
                1. Rabbit
                2. Monkey
                0. Exit
        """.trimIndent()
        println(chooseHerboHint)

        return when (inputHandler.readInt("Choose herbo: ", "Invalid option.", { it in 0..2 })) {
            1 -> "Rabbit"
            2 -> "Monkey"
            else -> exitProcess(0)
        }
    }

    private fun addPredator() {
        val predatorType = choosePredator()
        val food = inputHandler.readInt("Input food for animal: ", "Incorrect input. Number must be greater than 0.", { it > 0 })
        val number = inputHandler.readInt("Input number for animal: ", "Incorrect input. Number must be greater than 0.", { it > 0 })

        when (predatorType) {
            "Tiger" -> zoo.addAnimal(Tiger(food, number))
            "Wolf" -> zoo.addAnimal(Wolf(food, number))
        }
    }

    private fun choosePredator(): String {
        val choosePredatorPrompt = """
            Choose herbo:
                1. Rabbit
                2. Monkey
                0. Exit
        """.trimIndent()
        println(choosePredatorPrompt)

        return when (inputHandler.readInt("Choose predator: ", "Invalid option.", { it in 0..2 })) {
            1 -> "Tiger"
            2 -> "Wolf"
            else -> exitProcess(0)
        }
    }
}