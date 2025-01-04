package com.vasyukov.laba_05

import android.Manifest
import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.annotation.SuppressLint
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.provider.ContactsContract
import android.text.Editable
import android.text.TextWatcher
import android.util.Log
import android.view.View
import android.view.inputmethod.EditorInfo
import android.view.inputmethod.InputMethodManager
import android.widget.EditText
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView

class MainActivity : AppCompatActivity() {

    companion object {
        private var isFirstLoad = true
    }

    private lateinit var recyclerView: RecyclerView
    private lateinit var searchInput: EditText
    private lateinit var adapter: ContactAdapter
    private val contacts = mutableListOf<Contact>()
    private val filteredContacts = mutableListOf<Contact>()
    private var photos = listOf(
        R.drawable.avatar_01,
        R.drawable.avatar_02,
        R.drawable.avatar_03,
        R.drawable.avatar_04,
        R.drawable.avatar_05,
        R.drawable.avatar_06,
        R.drawable.avatar_07,
        R.drawable.avatar_08,
        R.drawable.avatar_09,
        R.drawable.avatar_10
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        searchInput = findViewById(R.id.searchInput)
        recyclerView = findViewById(R.id.contactsRecyclerView)

        adapter = ContactAdapter(filteredContacts)
        recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = adapter

        if (isFirstLoad) {
            isFirstLoad = false
            startAnimation(searchInput, recyclerView)
        } else {
            showViews(searchInput, recyclerView)
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_CONTACTS) == PackageManager.PERMISSION_GRANTED) {
            fetchContact()
        } else {
            requestPermissionLauncher.launch(Manifest.permission.READ_CONTACTS)
        }

        searchInput.addTextChangedListener(object : TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                filterContacts(s.toString())
            }
            override fun afterTextChanged(s: Editable?) {}
        })

        searchInput.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_SEARCH) {
                hideKeyboard()
                true
            } else {
                false
            }
        }
    }

    @SuppressLint("NotifyDataSetChanged")
    private fun filterContacts(query: String) {
        val lowercaseQuery = query.lowercase()
        filteredContacts.clear()
        filteredContacts.addAll(
            contacts.filter { it.name.lowercase().contains(lowercaseQuery) }
        )

        val noContact = findViewById<LinearLayout>(R.id.noContactsLayout)
        if (filteredContacts.isEmpty()) {
            recyclerView.visibility = View.GONE
            noContact.visibility = View.VISIBLE
        } else {
            recyclerView.visibility = View.VISIBLE
            noContact.visibility = View.GONE
        }

        adapter.notifyDataSetChanged()
    }

    private val requestPermissionLauncher = registerForActivityResult(ActivityResultContracts.RequestPermission()) { isGranted: Boolean ->
        if (isGranted) {
            fetchContact()
        } else {
            Log.e("MainActivity", "Доступ к контактам запрещён")
        }
    }

    private fun hideKeyboard() {
        val inputMethodManager = getSystemService(INPUT_METHOD_SERVICE) as InputMethodManager
        currentFocus?.let {
            inputMethodManager.hideSoftInputFromInputMethod(it.windowToken, 0)
        }
    }

    @SuppressLint("NotifyDataSetChanged")
    private fun fetchContact() {
        val resolver = contentResolver
        val cursor = resolver.query(
            ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
            arrayOf(
                ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME,
                ContactsContract.CommonDataKinds.Phone.NUMBER
            ),
            null,
            null,
            null
        )

        cursor?.use {
            val nameIndex = it.getColumnIndex(ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME)
            val numberIndex = it.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER)
            while (it.moveToNext()) {
                val noName = "Нет номера"
                val name = it.getString(nameIndex) ?: "Неизвестное имя"
                val number = it.getString(numberIndex) ?: noName

                var photoId = 0
                if (number != noName) {
                    number.forEach { c ->
                        if ('0' <= c && c <= '9') {
                            photoId += c - '0'
                        }
                    }
                }
                val photo = photos[photoId % 10]
                contacts.add(Contact(name, number, photo))
            }
        }

        filteredContacts.clear()
        filteredContacts.addAll(contacts)
        adapter.notifyDataSetChanged()
    }

    private fun showViews(vararg views: android.view.View) {
        views.forEach {
            it.visibility = android.view.View.VISIBLE
            ObjectAnimator.ofFloat(it, "alpha", 0f, 1f).apply {
                duration = 0
                start()
            }
        }
    }

    private fun startAnimation(searchInput: EditText, recyclerView: RecyclerView) {

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

                    showViews(searchInput, recyclerView)
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