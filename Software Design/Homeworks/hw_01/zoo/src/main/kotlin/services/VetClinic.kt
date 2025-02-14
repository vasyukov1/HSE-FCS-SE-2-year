package org.overmindv.services

import org.overmindv.models.Animal
import org.springframework.stereotype.Service

@Service
class VetClinic {
    fun checkHealth(animal: Animal): Boolean {
        return animal.number > 3
    }
}