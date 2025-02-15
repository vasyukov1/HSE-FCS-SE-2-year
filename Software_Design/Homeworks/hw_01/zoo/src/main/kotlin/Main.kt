package org.overmindv

import org.overmindv.components.Menu
import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication

@SpringBootApplication
class Application

fun main(args: Array<String>) {
    val context = runApplication<Application>(*args)
    val menu = context.getBean(Menu::class.java)
    menu.start()
}
