package org.overmindv.services

import org.overmindv.components.colorize
import org.overmindv.components.Color
import org.overmindv.interfaces.IInventory
import org.overmindv.models.Animal
import org.overmindv.models.Herbo
import org.overmindv.models.Thing
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Service

@Service
class Zoo(@Autowired private var vetClinic: VetClinic) {
    private val animals = mutableListOf<Animal>()
    private val things = mutableListOf<Thing>()

    fun addAnimal(animal: Animal) {
        if (vetClinic.checkHealth(animal)) {
            animals.add(animal)
            println(colorize("${animal.javaClass.simpleName} is accepted to the zoo.", Color.GREEN))
        } else {
            println(colorize("${animal.javaClass.simpleName} is rejected by veterinary clinic.", Color.RED))
        }
    }

    fun addThing(thing: Thing) {
        things.add(thing)
    }

    fun totalFoodConsumption() = animals.sumOf { it.food }
    fun getAnimals(): List<Animal> = animals
    fun getInteractiveAnimals(): List<Herbo> = animals.filterIsInstance<Herbo>().filter { it.kindness > 5 }
    fun getThings(): List<Thing> = things
    fun getInventory(): List<IInventory> = animals + things
}