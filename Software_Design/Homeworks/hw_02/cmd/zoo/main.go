package main

import (
	"fmt"
	"hw_02/internal/domain/animal"
	"time"
)

func main() {
	gender, _ := animal.NewGender("male")
	foodType, _ := animal.NewFoodType("meat")

	a, err := animal.NewAnimal("1", "Lion", "Simba", time.Now(), gender, foodType)
	if err != nil {
		fmt.Println("Error creating animal:", err)
		return
	}

	fmt.Printf("Animal created: %s (%s)\n", a.Name(), a.Species())

	a.MakeSick()
	fmt.Printf("Animal is sick: %v\n", !a.HealthStatus().IsHealthy())
}
