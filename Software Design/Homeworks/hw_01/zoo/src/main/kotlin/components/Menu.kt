package org.overmindv.components

import org.overmindv.services.Zoo
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Component
import kotlin.system.exitProcess

@Component
class Menu @Autowired constructor(
    private val zoo: Zoo,
    private val animalAdder: AnimalAdder,
    private val thingAdder: ThingAdder,
    private val inputHandler: InputHandler,
    private val outputHandler: OutputHandler
) {
    fun start() {
        while (true) {
            val menu = """
                Menu:
                    1. Show total food consumption
                    2. Add animal
                    3. Add thing
                    4. Show animals
                    5. Show kind animals
                    6. Show things
                    7. Show inventory
                    0. Exit
            """.trimIndent()

            println(menu)
            when (inputHandler.readInt("Choose: ", "Invalid option.", { it in 0..7 })) {
                1 -> println(colorize("Animals eat: ${zoo.totalFoodConsumption()} kg food per day.", Color.PURPLE))
                2 -> animalAdder.chooseAnimal()
                3 -> thingAdder.addThing()
                4 -> outputHandler.outputAnimals()
                5 -> outputHandler.outputKindAnimals()
                6 -> outputHandler.outputThings()
                7 -> outputHandler.outputInventory()
                0 -> exitProcess(0)
            }
        }
    }
}