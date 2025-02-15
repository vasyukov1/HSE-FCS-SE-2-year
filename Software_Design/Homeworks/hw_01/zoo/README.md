# Software Design | Mini HW 1

Author: Alexander Vasyukov

## Description of project "Zoo"

This project is a console app for management of zoo in Kotlin using SOLID and DI with Spring.

Testing is made using JUnit and JaCoCo is used to evaluate code coverage.

Functions:
1. Show total food consumption
2. Add animal
3. Add thing
4. Show animals
5. Show kind animals
6. Show things
7. Show inventory

## Project launch

1. Download Kotlin and Gradle.
2. Clone git repository:
   ```bash
   git clone https://github.com/vasyukov1/HSE-FCS-SE-2-year/
   cd HSE-FCS-SE-2-year/Software_Design/Homeworks/hw_01/zoo
    ```
3. Build project
   ```bash
   ./gradlew build
   ```
4. Run application
   ```bash
   java -jar build/libs/zoo-1.0-SNAPSHOT.jar
   ```
5. Start test
   ```bash
   ./gradlew test
   ```
6. Get report
   ```bash
   ./gradlew jacocoTestReport
   ```

7. To show coverage report, open `build/custom-jacoco-report/index.html` in the browser.

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
│   │   │   │   ├── AnimalAdderTest.kt 
│   │   │   │   ├── InputHandlerTest.kt
│   │   │   │   ├── OutputHandlerTest.kt
│   │   │   │   ├── ThingAdderTest.kt
│   │   │   │   ├── VetClinic.kt
│   │   │   │   ├── ZooTest.kt 
├── build.gradle.kts 
├── README.md
```

## OOP and SOLID

1. **S – Single responsibility principle (SRP)**
   - There are classes with different functions:
      - `Zoo` is responsible for management of zoo.
      - `VetClinic` is responsible for animal's health.
      - `Menu` is responsible for interaction with user.
      - `AnimalAdder` and `ThingAdder` are responsible for addition of animals and things.
      - `InputHandler` is responsible for checking of correct input.
      - `OutputHandler` is responsible for output of information about zoo.
      - `Color` is responsible for store information with colors.

2. **O - Open/closed principle (OCP)**
   - To add a new animals you have to inherit from `Herbo` or `Predator`.
   - If you want to create a new type of animals, you can create it inherited from `Animal`.
   - To add a new thing you have to inherit from `Thing`.
   - You can extend the functionality without significant changes.

3. **L – Liskov substitution principle (LSP)**
   - There is heritage and subclasses can replace by base classes.
   - For instance, `Rabbit -> Herbo -> Animal` or `Computer -> Thing`

4. **I – Interface segregation principle (ISP)**
   - There is interface `IAlive` for living entities having field `food`.
   - There is interface `IInventory` for zoo's inventory (things and animals) having field `number`.

5. **D – Dependency inversion principle (DIP)**
   - Framework `Spring` is using for dependency transfer to construction of classes.
   - Service `Zoo` uses `VetClinic`
   - Component `Menu` uses `Zoo`, `AnimalAdder`, `ThingAdder`, `InputHandler`, `OutputHandler`.

P.S. I don't know why all tests work except `ZooTest` and `VetClinic`. :(