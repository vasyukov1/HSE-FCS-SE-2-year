package com.vasyukov.lava_15_sqlite_sp

import android.content.ContentValues
import android.content.Context
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper

class TaskDatabaseHelper(context: Context) : SQLiteOpenHelper(context, DATABASE_NAME, null, DATABASE_VERSION) {

    companion object {
        private const val DATABASE_NAME = "tasks.db"
        private const val DATABASE_VERSION = 1
        const val TABLE_NAME = "tasks"
        const val COLUMN_ID = "id"
        const val COLUMN_TASK = "task"
        const val COLUMN_PRIORITY = "priority"
        const val COLUMN_CREATED_AT = "created_at"
    }

    override fun onCreate(db: SQLiteDatabase) {
        val createTableQuery = """
            CREATE TABLE $TABLE_NAME (
                $COLUMN_ID INTEGER PRIMARY KEY AUTOINCREMENT,
                $COLUMN_TASK TEXT NOT NULL,
                $COLUMN_PRIORITY INTEGER DEFAULT 0,
                $COLUMN_CREATED_AT DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        """.trimIndent()
        db.execSQL(createTableQuery)
    }

    override fun onUpgrade(db: SQLiteDatabase, oldVersion: Int, newVersion: Int) {
        db.execSQL("DROP TABLE IF EXISTS $TABLE_NAME")
        onCreate(db)
    }

    fun addTask(task: String, priority: Boolean): Long {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(COLUMN_TASK, task)
            put(COLUMN_PRIORITY, if (priority) 1 else 0)
        }
        return db.insert(TABLE_NAME, null, values)
    }

    fun getAllTasks(sortByPriority: Boolean = false): List<Task> {
        val tasks = mutableListOf<Task>()
        val db = readableDatabase
        val sortOrder = if (sortByPriority)
            "$COLUMN_PRIORITY DESC, $COLUMN_CREATED_AT DESC"
        else
            "$COLUMN_CREATED_AT DESC"

        val cursor = db.query(
            TABLE_NAME,
            arrayOf(COLUMN_ID, COLUMN_TASK, COLUMN_PRIORITY),
            null, null, null, null, sortOrder
        )

        cursor.use {
            while (it.moveToNext()) {
                val id = it.getLong(it.getColumnIndexOrThrow(COLUMN_ID))
                val taskText = it.getString(it.getColumnIndexOrThrow(COLUMN_TASK))
                val priority = it.getInt(it.getColumnIndexOrThrow(COLUMN_PRIORITY)) == 1
                tasks.add(Task(id, taskText, priority))
            }
        }
        return tasks
    }

    fun updateTask(id: Long, newTask: String, newPriority: Boolean): Int {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(COLUMN_TASK, newTask)
            put(COLUMN_PRIORITY, if (newPriority) 1 else 0)
        }
        return db.update(TABLE_NAME, values, "$COLUMN_ID = ?", arrayOf(id.toString()))
    }

    fun deleteTask(id: Long): Int {
        val db = writableDatabase
        return db.delete(TABLE_NAME, "$COLUMN_ID = ?", arrayOf(id.toString()))
    }
}

data class Task(val id: Long, val task: String, val priority: Boolean)