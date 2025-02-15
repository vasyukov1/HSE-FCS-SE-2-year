package components

import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test
import org.overmindv.components.InputHandler
import java.io.ByteArrayInputStream
import java.io.ByteArrayOutputStream
import java.io.PrintStream
import kotlin.test.assertEquals

class InputHandlerTest {
    private lateinit var inputHandler: InputHandler
    private lateinit var outputStream: ByteArrayOutputStream

    @BeforeEach
    fun setUp() {
        inputHandler = InputHandler()
        outputStream = ByteArrayOutputStream()
        System.setOut(PrintStream(outputStream))
    }

    @AfterEach
    fun tearDown() {
        System.setOut(System.out)
    }

    @Test
    fun `readInt should return correct input`() {
        val input = "5\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        val result = inputHandler.readInt("Enter a number: ", "Invalid input.", { it > 0 })

        assertEquals(5, result)
    }

    @Test
    fun `readInt should handle invalid input and retry`() {
        val input = "-5\n5\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        val result = inputHandler.readInt("Enter a number: ", "Invalid input.", { it > 0 })
        assertEquals(5, result)

        val output = outputStream.toString()
        assert(output.contains("Invalid input."))
    }

    @Test
    fun `readInt should handle non-numeric input and retry`() {
        val input = "abc\n5\n"
        System.setIn(ByteArrayInputStream(input.toByteArray()))

        val result = inputHandler.readInt("Enter a number: ", "Invalid input.", { it > 0 })
        assertEquals(5, result)

        val output = outputStream.toString()
        assert(output.contains("Invalid input."))
    }
}