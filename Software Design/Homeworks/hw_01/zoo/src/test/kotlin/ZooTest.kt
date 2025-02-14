import org.junit.jupiter.api.Assertions.*
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.Mockito.*
import org.mockito.junit.jupiter.MockitoExtension

import org.overmindv.models.Tiger
import org.overmindv.services.VetClinic
import org.overmindv.services.Zoo
import org.springframework.beans.factory.annotation.Autowired

import org.springframework.boot.test.context.SpringBootTest
import org.springframework.boot.test.mock.mockito.MockBean

@ExtendWith(MockitoExtension::class)
@SpringBootTest(classes = [Zoo::class])
class ZooTest {

    @MockBean
    private lateinit var vetClinic: VetClinic

    @Autowired
    private lateinit var zoo: Zoo

    @Test
    fun `add animal`() {
        val tiger = Tiger(10, 4)

        `when`(vetClinic.checkHealth(tiger)).thenReturn(true)

        val result = zoo.addAnimal(tiger)

        assertTrue(result)
        assertEquals(1, zoo.getAnimals().size)
    }

}