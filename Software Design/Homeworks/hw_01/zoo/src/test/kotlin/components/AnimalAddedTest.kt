package components

import org.junit.jupiter.api.Test
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.Mock
import org.mockito.junit.jupiter.MockitoExtension
import org.overmindv.components.AnimalAdder
import org.overmindv.components.InputHandler
import org.overmindv.services.Zoo
import java.io.ByteArrayInputStream
import java.io.ByteArrayOutputStream
import java.io.PrintStream
import kotlin.test.assertEquals
import kotlin.test.assertTrue

@ExtendWith(MockitoExtension::class)
class AnimalAdderTest {

    @Mock
    private lateinit var zoo: Zoo
    @Mock
    private lateinit var inputHandler: InputHandler
    private lateinit var animalAdder: AnimalAdder
    private lateinit var outputStream: ByteArrayOutputStream

    @BeforeEach
    fun setUp() {
        animalAdder = AnimalAdder(zoo, inputHandler)
        outputStream = ByteArrayOutputStream()
        System.setOut(PrintStream(outputStream))
    }

    @Test
    fun `chooseAnimal should add Rabbit when user chooses Herbo and Rabbit`() {
        // Choose: 1 (Herbo), 1 (Rabbit), food = 5, number = 3, kind = 7
        val input = "1\n1\n5\n3\n7"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        animalAdder.chooseAnimal()

        val output = outputStream.toString()
        assertTrue(output.contains("Choose animal:"))
        assertTrue(output.contains("Choose herbo:"))
    }

    @Test
    fun `chooseAnimal should add Monkey when user chooses Herbo and Monkey`() {
        // Choose: 1 (Herbo), 2 (Monkey), food = 10, number = 2, kind = 5
        val input = "1\n2\n10\n2\n5"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        animalAdder.chooseAnimal()

        val output = outputStream.toString()
        assertTrue(output.contains("Choose animal:"))
        assertTrue(output.contains("Choose herbo:"))
    }

    @Test
    fun `chooseAnimal should add Tiger when user chooses Predator and Tiger`() {
        // Choose: 2 (Predator), 1 (Tiger), food = 8, number = 4
        val input = "2\n1\n8\n8\n4"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        animalAdder.chooseAnimal()

        val output = outputStream.toString()
        assertTrue(output.contains("Choose animal:"))
        assertTrue(output.contains("Choose predator:"))
    }

    @Test
    fun `chooseAnimal should add Wolf when user chooses Predator and Wolf`() {
        // Choose: 2 (Predator), 2 (Wolf), food = 12, number = 6
        val input = "2\n2\n12\n6"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        animalAdder.chooseAnimal()

        val output = outputStream.toString()
        assertTrue(output.contains("Choose animal:"))
        assertTrue(output.contains("Choose predator:"))
    }

    @Test
    fun `chooseAnimal should exit when user chooses 0`() {
        val input = "0\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        val exception = org.junit.jupiter.api.Assertions.assertThrows(SecurityException::class.java) {
            animalAdder.chooseAnimal()
        }

        assertEquals("Exit called", exception.message)
    }
}