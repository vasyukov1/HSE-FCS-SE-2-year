package org.overmindv

import org.overmindv.components.Menu
import org.overmindv.services.Zoo

import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication

@SpringBootApplication
class Application

fun main(args: Array<String>) {
    val context = runApplication<Application>(*args)
    val zoo = context.getBean(Zoo::class.java)

    val menu = Menu(zoo)
    menu.start()
}
