package org.overmindv.components

import org.overmindv.services.Zoo
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Component

@Component
class OutputHandler(@Autowired private val zoo: Zoo) {
    fun outputAnimals() {
        val animals = zoo.getAnimals()
        if (animals.isEmpty()) {
            println(colorize("There are no animals.", Color.YELLOW))
        } else {
            println(colorize("List of animals in zoo:", Color.CYAN))
            println(colorize("\tAnimal\tNumber", Color.CYAN))
            animals.forEach {
                println(colorize("\t${it.javaClass.simpleName}\t${it.number}", Color.CYAN))
            }
        }
    }

    fun outputKindAnimals() {
        val kindAnimals = zoo.getInteractiveAnimals()
        if (kindAnimals.isEmpty()) {
            println(colorize("There are no kind animals.", Color.YELLOW))
        } else {
            println(colorize("Kind animals:", Color.CYAN))
            println(colorize("\tAnimal\tNumber\tKindness", Color.CYAN))
            kindAnimals.forEach {
                println(colorize("\t${it.javaClass.simpleName}\t${it.number}\t${it.kindness}", Color.CYAN))
            }
        }
    }

    fun outputThings() {
        val things = zoo.getThings()
        if (things.isEmpty()) {
            println(colorize("There are no things.", Color.YELLOW))
        } else {
            println(colorize("Things:", Color.CYAN))
            println(colorize("\tItem\tNumber", Color.CYAN))
            things.forEach {
                println(colorize("\t${it.javaClass.simpleName}\t${it.number}", Color.CYAN))
            }
        }
    }

    fun outputInventory() {
        val inventory = zoo.getInventory()
        if (inventory.isEmpty()) {
            println(colorize("There are no inventory.", Color.YELLOW))
        } else {
            println(colorize("Inventory:", Color.CYAN))
            println(colorize("\tItem\tNumber", Color.CYAN))
            inventory.forEach {
                println(colorize("\t${it.javaClass.simpleName}\t${it.number}", Color.CYAN))
            }
        }
    }
}