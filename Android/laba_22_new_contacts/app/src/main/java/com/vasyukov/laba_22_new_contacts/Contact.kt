package com.vasyukov.laba_22_new_contacts

data class Contact(
    val id: Long,
    val name: String,
    val phone: String,
    val comment: String?,
    val isFavorite: Boolean = false
)
