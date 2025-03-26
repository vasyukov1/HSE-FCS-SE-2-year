package com.vasyukov.laba_22_new_contacts

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.core.net.toUri

class ContactsActivity : AppCompatActivity() {

    private lateinit var addButton: Button
    private lateinit var favoritesButton: Button
    private lateinit var contactsListView: ListView
    private lateinit var searchEditText: EditText
    private lateinit var dbHelper: ContactDatabaseHelper
    private lateinit var adapter: ContactAdapter

    companion object {
        private const val REQUEST_CALL_PHONE = 1
        private const val REQUEST_SEND_SMS = 2
    }

    private var showingFavorites = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_contacts)

        addButton = findViewById(R.id.addButton)
        favoritesButton = findViewById(R.id.favoritesButton)
        contactsListView = findViewById(R.id.contactsListView)
        searchEditText = findViewById(R.id.searchEditText)

        dbHelper = ContactDatabaseHelper(this)
        adapter = ContactAdapter(dbHelper.getAllContacts())
        contactsListView.adapter = adapter

        addButton.setOnClickListener {
            showContactDialog(null)
        }

        searchEditText.addTextChangedListener(object : TextWatcher {
            override fun afterTextChanged(s: Editable?) {
                adapter.updateContacts(dbHelper.getAllContacts(s.toString()))
            }
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {}
        })

        favoritesButton.setOnClickListener {
            showingFavorites = !showingFavorites
            updateContactsList()
            favoritesButton.text = if (showingFavorites) "Все контакты" else "Избранное"
        }

        checkPermissions()
    }

    private fun updateContactsList(searchQuery: String? = null) {
        val contacts = if (showingFavorites) {
            dbHelper.getFavoriteContacts()
        } else {
            dbHelper.getAllContacts(searchQuery)
        }
        adapter.updateContacts(contacts)
    }

    private inner class ContactAdapter(private var contacts: List<Contact>) : BaseAdapter() {
        private val inflater = LayoutInflater.from(this@ContactsActivity)

        fun updateContacts(newContacts: List<Contact>) {
            contacts = newContacts
            notifyDataSetChanged()
        }

        override fun getCount(): Int = contacts.size
        override fun getItem(position: Int): Contact = contacts[position]
        override fun getItemId(position: Int): Long = contacts[position].id

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
            val view = convertView ?: inflater.inflate(R.layout.contact_item, parent, false)
            val contact = getItem(position)

            view.findViewById<TextView>(R.id.contactNameTextView).text = contact.name
            view.findViewById<TextView>(R.id.contactPhoneTextView).text = contact.phone
            view.findViewById<TextView>(R.id.contactCommentTextView).text = contact.comment ?: ""

            view.findViewById<ImageButton>(R.id.callButton).setOnClickListener {
                makeCall(contact.phone)
            }

            view.findViewById<ImageButton>(R.id.smsButton).setOnClickListener {
                sendSms(contact.phone)
            }

            val favoriteIcon = view.findViewById<ImageView>(R.id.favoriteIcon)
            favoriteIcon.setImageResource(
                if (contact.isFavorite) R.drawable.full_star else R.drawable.empty_star
            )

            favoriteIcon.setOnClickListener {
                val newFavoriteState = !contact.isFavorite
                dbHelper.toggleFavorite(contact.id, newFavoriteState)
                updateContactsList(searchEditText.text.toString())
            }

            view.setOnLongClickListener {
                showContactDialog(contact)
                true
            }

            return view
        }
    }

    private fun showContactDialog(contact: Contact?) {
        val dialogView = LayoutInflater.from(this).inflate(R.layout.contact_dialog, null)
        val nameEditText = dialogView.findViewById<EditText>(R.id.nameEditText)
        val phoneEditText = dialogView.findViewById<EditText>(R.id.phoneEditText)
        val commentEditText = dialogView.findViewById<EditText>(R.id.commentEditText)

        contact?.let {
            nameEditText.setText(it.name)
            phoneEditText.setText(it.phone)
            commentEditText.setText(it.comment)
        }

        AlertDialog.Builder(this)
            .setTitle(if (contact == null) "Добавить контакт" else "Редактировать контакт")
            .setView(dialogView)
            .setPositiveButton("Сохранить") { _, _ ->
                val name = nameEditText.text.toString().trim()
                val phone = phoneEditText.text.toString().trim()
                val comment = commentEditText.text.toString().trim().takeIf { it.isNotEmpty() }

                if (name.isNotEmpty() && phone.isNotEmpty()) {
                    if (contact == null) {
                        dbHelper.addContact(name, phone, comment)
                    } else {
                        dbHelper.updateContact(contact.id, name, phone, comment)
                    }
                    adapter.updateContacts(dbHelper.getAllContacts(searchEditText.text.toString()))
                } else {
                    Toast.makeText(this, "Заполните имя и телефон", Toast.LENGTH_SHORT).show()
                }
            }
            .setNegativeButton("Отмена", null)
            .apply {
                if (contact != null) {
                    setNeutralButton("Удалить") { _, _ ->
                        dbHelper.deleteContact(contact.id)
                        adapter.updateContacts(dbHelper.getAllContacts(searchEditText.text.toString()))
                    }
                }
            }
            .show()
    }

    private fun makeCall(phoneNumber: String) {
        val callIntent = Intent(Intent.ACTION_CALL).apply {
            data = "tel:$phoneNumber".toUri()
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED) {
            try {
                startActivity(callIntent)
            } catch (e: Exception) {
                Toast.makeText(this, "Ошибка звонка: ${e.message}", Toast.LENGTH_SHORT).show()
            }
        } else {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.CALL_PHONE), REQUEST_CALL_PHONE)
        }
    }

    private fun sendSms(phoneNumber: String) {
        val smsIntent = Intent(Intent.ACTION_VIEW).apply {
            data = "smsto:$phoneNumber".toUri()
            putExtra("address", phoneNumber)
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.SEND_SMS) == PackageManager.PERMISSION_GRANTED) {
            try {
                startActivity(smsIntent)
            } catch (e: Exception) {
                Toast.makeText(this, "Ошибка отправки SMS: ${e.message}", Toast.LENGTH_SHORT).show()
            }
        } else {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.SEND_SMS), REQUEST_SEND_SMS)
        }
    }

    private fun checkPermissions() {
        val permissions = arrayOf(
            Manifest.permission.CALL_PHONE,
            Manifest.permission.SEND_SMS,
            Manifest.permission.READ_CONTACTS
        )

        val permissionsToRequest = permissions.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }.toTypedArray()

        if (permissionsToRequest.isNotEmpty()) {
            ActivityCompat.requestPermissions(this, permissionsToRequest, 0)
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        when (requestCode) {
            REQUEST_CALL_PHONE -> {
                if (!(grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                    Toast.makeText(this, "Разрешение на звонки отклонено", Toast.LENGTH_SHORT).show()
                }
            }
            REQUEST_SEND_SMS -> {
                if (!(grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
                   Toast.makeText(this, "Разрешение на SMS отклонено", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }
}