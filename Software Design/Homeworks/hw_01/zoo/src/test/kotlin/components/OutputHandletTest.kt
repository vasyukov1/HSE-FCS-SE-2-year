package components

import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.Mock
import org.mockito.Mockito.*
import org.mockito.junit.jupiter.MockitoExtension
import org.overmindv.components.OutputHandler
import org.overmindv.models.*
import org.overmindv.services.Zoo
import java.io.ByteArrayOutputStream
import java.io.PrintStream

@ExtendWith(MockitoExtension::class)
class OutputHandlerTest {

    @Mock
    private lateinit var zoo: Zoo

    private lateinit var outputHandler: OutputHandler
    private lateinit var outputStream: ByteArrayOutputStream

    @BeforeEach
    fun setUp() {
        outputHandler = OutputHandler(zoo)
        outputStream = ByteArrayOutputStream()
        System.setOut(PrintStream(outputStream))
    }

    @AfterEach
    fun tearDown() {
        System.setOut(System.out)
    }

    @Test
    fun `outputAnimals should print message when no animals`() {
        `when`(zoo.getAnimals()).thenReturn(emptyList())

        outputHandler.outputAnimals()

        val output = outputStream.toString()
        assert(output.contains("There are no animals."))
    }

    @Test
    fun `outputAnimals should print list of animals`() {
        val animals = listOf(Tiger(10, 2), Rabbit(5, 3, 7))
        `when`(zoo.getAnimals()).thenReturn(animals)

        outputHandler.outputAnimals()

        val output = outputStream.toString()
        assert(output.contains("List of animals in zoo:"))
        assert(output.contains("Tiger\t2"))
        assert(output.contains("Rabbit\t3"))
    }

    @Test
    fun `outputKindAnimals should print message when no kind animals`() {
        `when`(zoo.getInteractiveAnimals()).thenReturn(emptyList())

        outputHandler.outputKindAnimals()

        val output = outputStream.toString()
        assert(output.contains("There are no kind animals."))
    }

    @Test
    fun `outputKindAnimals should print list of kind animals`() {
        val kindAnimals = listOf(Monkey(10, 2, 5), Rabbit(5, 3, 7))
        `when`(zoo.getInteractiveAnimals()).thenReturn(kindAnimals)

        outputHandler.outputKindAnimals()

        val output = outputStream.toString()
        assert(output.contains("Kind animals:"))
        assert(output.contains("Monkey\t2\t5"))
        assert(output.contains("Rabbit\t3\t7"))
    }

    @Test
    fun `outputThings should print message when no things`() {
        `when`(zoo.getThings()).thenReturn(emptyList())

        outputHandler.outputThings()

        val output = outputStream.toString()
        assert(output.contains("There are no things."))
    }

    @Test
    fun `outputThings should print list of things`() {
        val things = listOf(Computer(5), Table(3))
        `when`(zoo.getThings()).thenReturn(things)

        outputHandler.outputThings()

        val output = outputStream.toString()
        assert(output.contains("Things:"))
        assert(output.contains("Computer\t5"))
        assert(output.contains("Table\t3"))
    }

    @Test
    fun `outputInventory should print message when no inventory`() {
        `when`(zoo.getInventory()).thenReturn(emptyList())

        outputHandler.outputInventory()

        val output = outputStream.toString()
        assert(output.contains("There are no inventory."))
    }

    @Test
    fun `outputInventory should print list of inventory`() {
        val inventory = listOf(Tiger(10, 2), Computer(5))
        `when`(zoo.getInventory()).thenReturn(inventory)

        outputHandler.outputInventory()

        val output = outputStream.toString()
        assert(output.contains("Inventory:"))
        assert(output.contains("Tiger\t2"))
        assert(output.contains("Computer\t5"))
    }
}