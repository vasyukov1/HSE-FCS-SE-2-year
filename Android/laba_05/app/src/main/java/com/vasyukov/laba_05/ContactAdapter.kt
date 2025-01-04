package com.vasyukov.laba_05

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.recyclerview.widget.RecyclerView

data class Contact(
    val name: String,
    val number: String,
    val photoId: Int
)

class ContactAdapter(private val contacts: List<Contact>) :
    RecyclerView.Adapter<ContactAdapter.ContactViewHolder>() {

    class ContactViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val nameTextView: TextView = itemView.findViewById(R.id.contactName)
        val numberTextView: TextView = itemView.findViewById(R.id.contactNumber)
        val photoImageView: ImageView = itemView.findViewById(R.id.contactPhoto)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ContactViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_contact, parent, false)
        return ContactViewHolder(view)
    }

    override fun onBindViewHolder(holder: ContactViewHolder, position: Int) {
        val contact = contacts[position]
        holder.nameTextView.text = contact.name
        holder.numberTextView.text = contact.number
        holder.photoImageView.setImageResource(contact.photoId)

        holder.numberTextView.setOnClickListener {
            val clipboardManager = it.context.getSystemService(android.content.Context.CLIPBOARD_SERVICE) as android.content.ClipboardManager
            val clip = android.content.ClipData.newPlainText("Phone Number", contact.number)
            clipboardManager.setPrimaryClip(clip)

            Toast.makeText(it.context, "Номер скопирован: ${contact.number}", Toast.LENGTH_SHORT).show()
        }
    }

    override fun getItemCount(): Int = contacts.size
}