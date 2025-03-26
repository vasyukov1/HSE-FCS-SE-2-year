package com.vasyukov.laba_22_new_contacts

import android.content.ContentValues
import android.content.Context
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper

class ContactDatabaseHelper(context: Context) : SQLiteOpenHelper(context, DATABASE_NAME, null, DATABASE_VERSION) {

    companion object {
        private const val DATABASE_NAME = "contacts.db"
        private const val DATABASE_VERSION = 1
        const val TABLE_NAME = "contacts"
        const val COLUMN_ID = "id"
        const val COLUMN_NAME = "name"
        const val COLUMN_PHONE = "phone"
        const val COLUMN_COMMENT = "comment"
        const val COLUMN_FAVORITE = "favorite"
    }

    override fun onCreate(db: SQLiteDatabase) {
        val createTableQuery = """
            CREATE TABLE $TABLE_NAME (
                $COLUMN_ID INTEGER PRIMARY KEY AUTOINCREMENT,
                $COLUMN_NAME TEXT NOT NULL,
                $COLUMN_PHONE TEXT NOT NULL,
                $COLUMN_COMMENT TEXT,
                $COLUMN_FAVORITE INTEGER DEFAULT 0
            )
        """.trimIndent()
        db.execSQL(createTableQuery)
    }

    override fun onUpgrade(db: SQLiteDatabase, oldVersion: Int, newVersion: Int) {
        db.execSQL("DROP TABLE IF EXISTS $TABLE_NAME")
        onCreate(db)
    }

    fun addContact(name: String, phone: String, comment: String?): Long {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(COLUMN_NAME, name)
            put(COLUMN_PHONE, phone)
            put(COLUMN_COMMENT, comment)
        }
        return db.insert(TABLE_NAME, null, values)
    }

    fun getAllContacts(searchQuery: String? = null): List<Contact> {
        val contacts = mutableListOf<Contact>()
        val db = readableDatabase

        val selection = if (!searchQuery.isNullOrEmpty()) {
            "$COLUMN_NAME LIKE ? OR $COLUMN_PHONE LIKE ?"
        } else null

        val selectionArgs = if (!searchQuery.isNullOrEmpty()) {
            arrayOf("%$searchQuery%", "%$searchQuery%")
        } else null

        val cursor = db.query(
            TABLE_NAME,
            arrayOf(COLUMN_ID, COLUMN_NAME, COLUMN_PHONE, COLUMN_COMMENT, COLUMN_FAVORITE),
            selection,
            selectionArgs,
            null, null, "$COLUMN_NAME COLLATE NOCASE ASC"
        )

        cursor.use {
            while (it.moveToNext()) {
                val id = it.getLong(it.getColumnIndexOrThrow(COLUMN_ID))
                val name = it.getString(it.getColumnIndexOrThrow(COLUMN_NAME))
                val phone = it.getString(it.getColumnIndexOrThrow(COLUMN_PHONE))
                val comment = it.getString(it.getColumnIndexOrThrow(COLUMN_COMMENT))
                val isFavorite = it.getInt(it.getColumnIndexOrThrow(COLUMN_FAVORITE)) == 1
                contacts.add(Contact(id, name, phone, comment, isFavorite))
            }
        }
        return contacts
    }

    fun updateContact(id: Long, name: String, phone: String, comment: String?): Int {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(COLUMN_NAME, name)
            put(COLUMN_PHONE, phone)
            put(COLUMN_COMMENT, comment)
        }
        return db.update(TABLE_NAME, values, "$COLUMN_ID = ?", arrayOf(id.toString()))
    }

    fun deleteContact(id: Long): Int {
        val db = writableDatabase
        return db.delete(TABLE_NAME, "$COLUMN_ID = ?", arrayOf(id.toString()))
    }

    fun toggleFavorite(id: Long, isFavorite: Boolean): Int {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(COLUMN_FAVORITE, if (isFavorite) 1 else 0)
        }
        return db.update(TABLE_NAME, values, "$COLUMN_ID = ?", arrayOf(id.toString()))
    }

    fun getFavoriteContacts(): List<Contact> {
        return getAllContacts().filter { it.isFavorite }
    }
}