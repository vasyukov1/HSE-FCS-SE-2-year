import org.junit.jupiter.api.Test
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.ArgumentMatchers.anyString
import org.mockito.Mock
import org.mockito.Mockito.`when`
import org.mockito.junit.jupiter.MockitoExtension
import org.overmindv.components.AnimalAdder
import org.overmindv.components.InputHandler
import org.overmindv.services.Zoo
import java.io.ByteArrayInputStream
import java.io.ByteArrayOutputStream
import java.io.PrintStream
import kotlin.test.assertTrue
import org.mockito.kotlin.check

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
    fun `chooseAnimal should give choice Herbo`() {
        val input = "1"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        `when`(inputHandler.readInt(anyString(), anyString(), check { it(1) })).thenReturn(1)
        animalAdder.chooseAnimal()

        val output = outputStream.toString()
        assertTrue(output.contains("Choose animal:"))
        assertTrue(output.contains("Choose herbo:"))
    }

    @Test
    fun `chooseAnimal should give choice Predator`() {
        val input = "2"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        `when`(inputHandler.readInt(anyString(), anyString(), check { it(2) })).thenReturn(2)
        animalAdder.chooseAnimal()

        val output = outputStream.toString()
        assertTrue(output.contains("Choose animal:"))
    }
}