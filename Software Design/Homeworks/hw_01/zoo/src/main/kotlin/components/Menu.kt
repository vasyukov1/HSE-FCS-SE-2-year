package org.overmindv.components

import org.overmindv.services.Zoo
import kotlin.system.exitProcess

class Menu(private val zoo: Zoo) {
    fun start() {
        while (true) {
            val menu = """
                components.Menu:
                    1. Output total food consumption
                    2. Add baseClasses.Animal
                    3. Add baseClasses.Thing
                    4. Output animals
                    5. Output kind animals
                    6. Output things
                    7. Output inventory
                    0. Exit
            """.trimIndent()

            println(menu)

            when (readLine()?.toIntOrNull()) {
                1 -> zoo.totalFoodConsumption()
                2 -> chooseAnimal()
                3 -> chooseThing()
                4 -> outputAnimals(zoo)
                5 -> outputKindAnimals(zoo)
                6 -> outputThings(zoo)
                7 -> outputInventory(zoo)
                0 -> exitProcess(0)
                else -> println("Invalid option")
            }
        }
    }

    private fun chooseAnimal() {
        val chooseAnimalMenu = """
        Choose baseClasses.Animal:
            1. animalTypesClasses.Herbo
            2. animalTypesClasses.Predator
            0. Back
    """.trimIndent()

        println(chooseAnimalMenu)

        val animalTypeChoose = readLine()?.toIntOrNull()
        if (animalTypeChoose == 1) {
            addHerbo()
        } else if (animalTypeChoose == 2) {
            addPredator()
        } else if (animalTypeChoose == 0) {
            start()
        } else {
            println("Input is incorrect.")
        }

    }

    private fun addHerbo() {

    }

    private fun addPredator() {

    }

    private fun chooseThing() {

    }

    private fun outputAnimals(zoo: Zoo) {
        println("List of animals in zoo:")
        zoo.getAnimals().forEach {
            println(it)
        }
    }

    private fun outputKindAnimals(zoo: Zoo) {

    }

    private fun outputThings(zoo: Zoo) {

    }

    private fun outputInventory(zoo: Zoo) {
        zoo.inventoryReport()
    }
}
