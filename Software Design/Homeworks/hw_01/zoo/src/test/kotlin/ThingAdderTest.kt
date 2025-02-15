import org.junit.jupiter.api.Test
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.Mock
import org.mockito.Mockito.*
import org.mockito.junit.jupiter.MockitoExtension
import org.overmindv.components.ThingAdder
import org.overmindv.models.Computer
import org.overmindv.services.Zoo
import java.io.ByteArrayInputStream
import java.io.ByteArrayOutputStream
import java.io.PrintStream
import kotlin.test.assertEquals

@ExtendWith(MockitoExtension::class)
class ThingAdderTest {

    @Mock
    private lateinit var zoo: Zoo
    private lateinit var thingAdder: ThingAdder
    private lateinit var outputStream: ByteArrayOutputStream

    @BeforeEach
    fun setUp() {
        thingAdder = ThingAdder(zoo)
        outputStream = ByteArrayOutputStream()
        System.setOut(PrintStream(outputStream))
    }

    @Test
    fun `addThing should add Computer when user chooses 1 and inputs correct number`() {
        val input = "1\n5\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        thingAdder.addThing()

        val output = outputStream.toString()
        assert(output.contains("Choose thing:"))
        assert(output.contains("Computer is added to the zoo."))
    }

    @Test
    fun `addThing should add Table when user chooses 2 and inputs correct number`() {
        val input = "2\n3\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        thingAdder.addThing()

        val output = outputStream.toString()
        assert(output.contains("Choose thing:"))
        assert(output.contains("Table is added to the zoo."))
    }

    @Test
    fun `addThing should handle incorrect input for thing type`() {
        val input = "99\n1\n5\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        thingAdder.addThing()

        val output = outputStream.toString()
        assert(output.contains("Incorrect input."))
        assert(output.contains("Computer is added to the zoo."))
    }

    @Test
    fun `addThing should handle incorrect input for number`() {
        val input = "1\n-5\n5\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))
        thingAdder.addThing()
        verify(zoo).addThing(Computer(5))

        val output = outputStream.toString()
        assert(output.contains("Incorrect input. Number must be greater that 0."))
        assert(output.contains("Computer is added to the zoo."))
    }

    @Test
    fun `addThing should exit when user chooses 0`() {
        val input = "0\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        val exception = org.junit.jupiter.api.Assertions.assertThrows(SecurityException::class.java) {
            thingAdder.addThing()
        }

        assertEquals("Exit called", exception.message)
    }
}