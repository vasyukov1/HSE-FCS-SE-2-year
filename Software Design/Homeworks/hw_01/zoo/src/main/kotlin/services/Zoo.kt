package org.overmindv.services

import org.overmindv.interfaces.IInventory
import org.overmindv.models.Animal
import org.overmindv.models.Herbo
import org.overmindv.models.Thing
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Service

@Service
class Zoo(@Autowired private var vetClinic: VetClinic ) {

    private val animals = mutableListOf<Animal>()
    private val things = mutableListOf<Thing>()
    private val inventory = mutableListOf<IInventory>()

    fun addAnimal(animal: Animal): Boolean {
        return if (vetClinic.checkHealth(animal)) {
            animals.add(animal)
            inventory.add(animal)
            println("baseClasses.Animal is accepted to the zoo.")
            true
        } else {
            println("baseClasses.Animal is rejected by veterinary clinic.")
            false
        }
    }

    fun addInventory(inventoryItem: IInventory) {
        inventory.add(inventoryItem)
        print("Inventory is added to the zoo.")
    }

    fun totalFoodConsumption() {
        val food = animals.sumOf { it.food }
        println("Animals eat: $food kg food per day.")
    }

    fun interactiveAnimals() {
        val kindnessAnimals = animals.filterIsInstance<Herbo>().filter { it.kindness > 5 }
        println("Number of interactive animals: ${kindnessAnimals.size}.")
    }

    fun inventoryReport() {
        println("List of inventory:")
        inventory.forEach {
            println(it)
        }
    }

    fun getAnimals(): List<Animal> {
        return animals
    }

}