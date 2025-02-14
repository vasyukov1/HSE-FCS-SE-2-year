import org.junit.jupiter.api.Assertions.*
import org.junit.jupiter.api.Test

import org.overmindv.models.Tiger
import org.overmindv.services.VetClinic
import org.springframework.boot.test.context.SpringBootTest

@SpringBootTest(classes = [VetClinic::class])
class VetClinicTest {

    private val vetClinic = VetClinic()

    @Test
    fun `animal is healthy`() {
        val tiger = Tiger(10, 5)
        assertTrue(vetClinic.checkHealth(tiger))
    }

    @Test
    fun `animal is not healthy`() {
        val tiger = Tiger(5, 0)
        assertFalse(vetClinic.checkHealth(tiger))
    }

}