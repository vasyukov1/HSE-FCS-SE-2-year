package org.overmindv.models

import org.overmindv.interfaces.IAlive
import org.overmindv.interfaces.IInventory

open class Animal(override var food: Int, override var number: Int) : IAlive, IInventory