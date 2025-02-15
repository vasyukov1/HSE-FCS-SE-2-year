package services

import org.junit.jupiter.api.Assertions.*
import org.junit.jupiter.api.Test
import org.overmindv.models.Animal
import org.overmindv.services.VetClinic
import org.springframework.boot.test.context.SpringBootTest


@SpringBootTest(classes = [VetClinic::class])
class VetClinicTest {
    val vetClinic = VetClinic()

    @Test
    fun testCheckHealthStatistics() {
        val animal = Animal(food = 100, number = 1)
        val results = List(1000) { vetClinic.checkHealth(animal) }
        val trueCount = results.count { it }
        val ratio = trueCount.toDouble() / results.size

        assertTrue(ratio in 0.6..0.8) {
            "Wait, that accepted would be about 70%, but got ${ratio * 100}%"
        }
    }
}