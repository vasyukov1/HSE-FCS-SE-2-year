package com.vasyukov.lava_15_sqlite_sp

import android.app.AlertDialog
import android.content.SharedPreferences
import android.os.Bundle
import android.view.LayoutInflater
import android.widget.*
import androidx.appcompat.app.AppCompatActivity

class ToDoActivity : AppCompatActivity() {

    private lateinit var taskEditText: EditText
    private lateinit var priorityCheckBox: CheckBox
    private lateinit var addButton: Button
    private lateinit var tasksListView: ListView
    private lateinit var settingsButton: Button
    private lateinit var dbHelper: TaskDatabaseHelper
    private lateinit var sharedPreferences: SharedPreferences
    private lateinit var adapter: ArrayAdapter<String>
    private var sortByPriority = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_to_do)

        sharedPreferences = getSharedPreferences("AppSettings", MODE_PRIVATE)
        sortByPriority = sharedPreferences.getBoolean("sort_by_priority", false)

        taskEditText = findViewById(R.id.taskEditText)
        priorityCheckBox = findViewById(R.id.priorityCheckBox)
        addButton = findViewById(R.id.addButton)
        tasksListView = findViewById(R.id.tasksListView)
        settingsButton = findViewById(R.id.settingsButton)

        dbHelper = TaskDatabaseHelper(this)
        updateTasksList()

        addButton.setOnClickListener {
            val task = taskEditText.text.toString().trim()
            val priority = priorityCheckBox.isChecked

            if (task.isNotEmpty()) {
                dbHelper.addTask(task, priority)
                updateTasksList()
                taskEditText.text.clear()
                priorityCheckBox.isChecked = false
            } else {
                Toast.makeText(this, "Input Task", Toast.LENGTH_SHORT).show()
            }
        }

        tasksListView.onItemClickListener = AdapterView.OnItemClickListener { _, _, position, _ ->
            val tasks = dbHelper.getAllTasks(sortByPriority)
            if (position < tasks.size) {
                showEditDeleteDialog(tasks[position])
            }
        }

        settingsButton.setOnClickListener {
            showSettingsDialog()
        }
    }

    private fun updateTasksList() {
        val tasks = dbHelper.getAllTasks(sortByPriority)
        val tasksTextList = tasks.map {
            (if (it.priority) "★ " else "  ") + it.task
        }
        adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, tasksTextList)
        tasksListView.adapter = adapter
    }

    private fun showEditDeleteDialog(task: Task) {
        val dialogView = LayoutInflater.from(this).inflate(R.layout.edit_task_dialog, null)
        val taskEditText = dialogView.findViewById<EditText>(R.id.dialogTaskEditText)
        val priorityCheckBox = dialogView.findViewById<CheckBox>(R.id.dialogPriorityCheckBox)

        taskEditText.setText(task.task)
        priorityCheckBox.isChecked = task.priority

        AlertDialog.Builder(this)
            .setTitle("Edit Task")
            .setView(dialogView)
            .setPositiveButton("Save") { _, _ ->
                val newTask = taskEditText.text.toString().trim()
                val newPriority = priorityCheckBox.isChecked
                if (newTask.isNotEmpty()) {
                    dbHelper.updateTask(task.id, newTask, newPriority)
                    updateTasksList()
                } else {
                    Toast.makeText(this, "Task cannot be empty", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Delete") { _, _ ->
                dbHelper.deleteTask(task.id)
                updateTasksList()
            }
            .setNeutralButton("Cancel", null)
            .show()
    }

    private fun showSettingsDialog() {
        AlertDialog.Builder(this)
            .setTitle("Sorting")
            .setSingleChoiceItems(
                arrayOf("By Date", "By Priority"),
                if (sortByPriority) 1 else 0
            ) { dialog, which ->
                sortByPriority = which == 1
                sharedPreferences.edit().putBoolean("sort_by_priority", sortByPriority).apply()
                updateTasksList()
                dialog.dismiss()
            }
            .setNegativeButton("Cancel", null)
            .show()
    }
}