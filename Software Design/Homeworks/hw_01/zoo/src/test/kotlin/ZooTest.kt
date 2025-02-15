import org.junit.jupiter.api.Assertions.*
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.extension.ExtendWith
import org.mockito.InjectMocks
import org.mockito.Mock
import org.mockito.Mockito.*
import org.mockito.junit.jupiter.MockitoExtension
import org.overmindv.models.*
import org.overmindv.services.VetClinic
import org.overmindv.services.Zoo

@ExtendWith(MockitoExtension::class)
class ZooTest {
    @Mock
    private lateinit var vetClinic: VetClinic

    @InjectMocks
    private lateinit var zoo: Zoo

    @Test
    fun `test total food consumption`() {
        val tiger = Tiger(1, 1)
        val wolf = Wolf(3, 1)
        `when`(vetClinic.checkHealth(tiger)).thenReturn(true)
        `when`(vetClinic.checkHealth(wolf)).thenReturn(true)
        zoo.addAnimal(tiger)
        zoo.addAnimal(wolf)
        val expectedTotalFood = tiger.food + wolf.food

        assertEquals(expectedTotalFood, zoo.totalFoodConsumption())
    }

    @Test
    fun `test add animal accepted`() {
        val animal = Animal(1, 1)
        `when`(vetClinic.checkHealth(animal)).thenReturn(true)
        zoo.addAnimal(animal)

        assertEquals(1, zoo.getAnimals().size)
        assertTrue(zoo.getAnimals().contains(animal))
        assertTrue(zoo.getInventory().contains(animal))
    }

    @Test
    fun `test add animal rejected`() {
        val animal = Animal(1, 1)
        `when`(vetClinic.checkHealth(animal)).thenReturn(false)
        zoo.addAnimal(animal)

        assertEquals(0, zoo.getAnimals().size)
        assertFalse(zoo.getAnimals().contains(animal))
        assertFalse(zoo.getInventory().contains(animal))
    }

    @Test
    fun `test add thing`() {
        val table = Table(1)
        val computer = Computer(1)
        zoo.addThing(table)
        zoo.addThing(computer)

        assertEquals(2, zoo.getThings().size)
        assertTrue(zoo.getThings().contains(table))
        assertTrue(zoo.getInventory().contains(computer))
    }

    @Test
    fun `test get interactive animals`() {
        val rabbit = Rabbit(2, 2, 0)
        val monkey = Monkey(5, 3, 7)

        `when`(vetClinic.checkHealth(rabbit)).thenReturn(true)
        `when`(vetClinic.checkHealth(monkey)).thenReturn(true)
        zoo.addAnimal(rabbit)
        zoo.addAnimal(monkey)

        assertEquals(1, zoo.getInteractiveAnimals().size)
        assertTrue(zoo.getInteractiveAnimals().contains(monkey))
        assertFalse(zoo.getInteractiveAnimals().contains(rabbit))
    }
}