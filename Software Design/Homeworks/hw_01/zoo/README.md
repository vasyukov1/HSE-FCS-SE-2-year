# Software Design | Mini HW 1

## Description of project "Zoo"

This is a console app for management of zoo in Kotlin using SOLID and DI with Spring.

Functions:
1. Show total food consumption
2. Add animal
3. Add thing
4. Show animals
5. Show kind animals
6. Show things
7. Show inventory

## Project structure

```
zoo/
├── src/
│   ├── main/
│   │   ├── kotlin/
│   │   │   ├── org.overmindv/
│   │   │   │   ├── components/
│   │   │   │   │   ├── AnimalAdder.kt
│   │   │   │   │   ├── Color.kt
│   │   │   │   │   ├── InputHandler.kt
│   │   │   │   │   ├── Menu.kt
│   │   │   │   │   ├── OutputHandler.kt
│   │   │   │   │   ├── ThingAdder.kt
│   │   │   │   ├── interfaces/
│   │   │   │   │   ├── IAlive.kt
│   │   │   │   │   ├── IInventory.kt
│   │   │   │   ├── models/
│   │   │   │   │   ├── Animal.kt
│   │   │   │   │   ├── Computer.kt
│   │   │   │   │   ├── Herbo.kt
│   │   │   │   │   ├── Monkey.kt
│   │   │   │   │   ├── Predator.kt
│   │   │   │   │   ├── Rabbit.kt 
│   │   │   │   │   ├── Table.kt
│   │   │   │   │   ├── Thing.kt
│   │   │   │   │   ├── Tiger.kt
│   │   │   │   │   ├── Wolf.kt
│   │   │   │   ├── services/
│   │   │   │   │   ├── VetClinic.kt
│   │   │   │   │   ├── Zoo.kt
│   │   │   │   ├── Main.kt
│   ├── test/
│   │   ├── kotlin/
│   │   │   ├── org.overmindv/
│   │   │   │   ├── components/
│   │   │   │   │   ├── AnimalAdderTest.kt 
│   │   │   │   │   ├── InputHandlerTest.kt
│   │   │   │   │   ├── OutputHandlerTest.kt
│   │   │   │   │   ├── ThingAdderTest.kt
│   │   │   │   ├── services/
│   │   │   │   │   ├── ZooTest.kt 
├── build.gradle.kts 
├── README.md
```

## Project launch

1. Download Kotlin and Gradle.
2. Clone git repository:
   ```bash
   git clone https://github.com/vasyukov1/HSE-FCS-SE-2-year/
   cd "Software Design"/Homeworks/hw_01/zoo
    ```
3. Start project
   ```bash
   ./gradlew run
   ```
4. 

### OOP and SOLID


### SOLID

### DI


### 