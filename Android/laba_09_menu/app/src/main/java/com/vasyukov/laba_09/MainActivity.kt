package com.vasyukov.laba_09

import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.content.ClipData
import android.content.ClipboardManager
import android.graphics.Color
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.ContextMenu
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.widget.Toolbar
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    private lateinit var toolbar: Toolbar
    private lateinit var inputField: EditText
    private lateinit var actionButton: Button
    private lateinit var label: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        inputField = findViewById(R.id.inputField)
        actionButton = findViewById(R.id.actionButton)
        label = findViewById(R.id.label)

        toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar)

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(inputField, actionButton, label, toolbar)
        } else {
            showViews(inputField, actionButton, label, toolbar)
        }

        registerForContextMenu(inputField)
        registerForContextMenu(actionButton)

        actionButton.setOnClickListener {
            label.text = "Вы нажали кнопку!"
        }
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.options_menu, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.show_hide_field -> {
                val isVisible = item.isChecked
                item.isChecked = !isVisible
                inputField.visibility = if (isVisible) View.GONE else View.VISIBLE
                true
            }
            R.id.show_hide_button -> {
                val isVisible = item.isChecked
                item.isChecked = !isVisible
                actionButton.visibility =  if (isVisible) View.GONE else View.VISIBLE
                true
            }
            R.id.change_theme -> {
                val isDarkTheme = item.isChecked
                item.isChecked = !isDarkTheme
                AppCompatDelegate.setDefaultNightMode(
                    if (isDarkTheme) AppCompatDelegate.MODE_NIGHT_NO else AppCompatDelegate.MODE_NIGHT_YES
                )
                true
            }
            R.id.self_destruction -> {
                inputField.visibility = View.GONE
                actionButton.visibility = View.GONE
                label.visibility = View.GONE
                toolbar.visibility = View.GONE

                inputField.isEnabled = false
                actionButton.isEnabled = false
                toolbar.isEnabled = false

                val selfDestructionMessage = findViewById<TextView>(R.id.selfDestructionMessage)
                selfDestructionMessage.visibility = View.VISIBLE
                true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onCreateContextMenu(
        menu: ContextMenu?,
        v: View?,
        menuInfo: ContextMenu.ContextMenuInfo?
    ) {
        super.onCreateContextMenu(menu, v, menuInfo)
        when (v?.id) {
            R.id.inputField -> menuInflater.inflate(R.menu.context_menu_edittext, menu)
            R.id.actionButton -> menuInflater.inflate(R.menu.context_menu_button, menu)
        }
    }

    override fun onContextItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.clear_text -> {
                inputField.text.clear()
                true
            }
            R.id.sample_text -> {
                inputField.setText("Пример текста")
                true
            }
            R.id.copy_text -> {
                val clipboard = getSystemService(CLIPBOARD_SERVICE) as ClipboardManager
                val clip = ClipData.newPlainText("Copied Text", inputField.text.toString())
                clipboard.setPrimaryClip(clip)
                Toast.makeText(this, "Текст скопирован", Toast.LENGTH_SHORT).show()
                true
            }
            R.id.change_button_text -> {
                actionButton.text = "Кнопка нажата!"
                true
            }
            R.id.change_button_color -> {
                actionButton.setBackgroundColor(Color.rgb(
                    (0..255).random(),
                    (0..255).random(),
                    (0..255).random()
                ))
                true
            }
            else -> super.onContextItemSelected(item)
        }
    }

    private fun showViews(vararg views: View) {
        views.forEach {
            it.visibility = View.VISIBLE
            ObjectAnimator.ofFloat(it, "alpha", 0f, 1f).apply {
                duration = 0
                start()
            }
        }
    }

    private fun startAnimation(vararg views: View) {
        val fioLabel = findViewById<TextView>(R.id.fioLabel)
        val groupLabel = findViewById<TextView>(R.id.groupLabel)
        val avatarImage = findViewById<ImageView>(R.id.avatarImage)

        listOf(fioLabel, groupLabel, avatarImage).forEach { view ->
            ObjectAnimator.ofFloat(view, "alpha", 0f, 1f).apply {
                duration = 1000
                start()
            }
        }

        Handler(Looper.getMainLooper()).postDelayed({
            val fadeOutAvatar = ObjectAnimator.ofFloat(avatarImage, "alpha", 1f, 0f).apply {
                duration = 1000
            }
            val moveFioDown = ObjectAnimator.ofFloat(fioLabel, "alpha", 1f, 0f).apply {
                duration = 1000
            }
            val moveGroupDown = ObjectAnimator.ofFloat(groupLabel, "alpha", 1f, 0f).apply {
                duration = 1000
            }

            fadeOutAvatar.addUpdateListener {
                if (it.animatedFraction == 1f) {
                    avatarImage.visibility = ImageView.GONE
                    fioLabel.visibility = TextView.GONE
                    groupLabel.visibility = TextView.GONE

                    showViews(views[0], views[1], views[2], views[3])
                    listOf(fioLabel, groupLabel).forEach { element ->
                        ObjectAnimator.ofFloat(element, "alpha", 0f, 1f).apply {
                            duration = 1000
                            start()
                        }
                    }
                }
            }

            AnimatorSet().apply {
                playTogether(fadeOutAvatar, moveFioDown, moveGroupDown)
                start()
            }

        }, 2000)
    }
}