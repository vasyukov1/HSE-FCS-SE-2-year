package com.vasyukov.laba_16_firebase

import android.content.Context
import android.graphics.Color
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import kotlin.random.Random

class MessageAdapter(private val context: Context, private val messages: List<Message>) :
    RecyclerView.Adapter<MessageAdapter.MessageViewHolder>() {

    class MessageViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val messageContainer: View = view.findViewById(R.id.message_container)
        val userName: TextView = view.findViewById(R.id.user_name)
        val messageText: TextView = view.findViewById(R.id.message_text)
        val userPhoto: ImageView = view.findViewById(R.id.user_photo)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MessageViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.item_message, parent, false)
        return MessageViewHolder(view)
    }

    private val userColors = mutableMapOf<String, Int>()
    private fun getUserColor(userId: String): Int {
        return userColors.getOrPut(userId) {
            val random = Random(userId.hashCode().toLong())
            Color.rgb(random.nextInt(256), random.nextInt(256), random.nextInt(256))
        }
    }

    override fun onBindViewHolder(holder: MessageViewHolder, position: Int) {
        val message = messages[position]
        holder.userName.text = message.userName
        holder.messageText.text = message.text
        holder.userName.setTextColor(getUserColor(message.userId))

        if (message.userPhoto.isNotEmpty()) {
            Glide.with(holder.itemView.context)
                .load(message.userPhoto)
                .into(holder.userPhoto)
        } else {
            holder.userPhoto.setImageResource(R.drawable.ic_default_user)
        }

        val currentUserId = FirebaseAuth.getInstance().currentUser?.uid
        if (message.userId == currentUserId) {
            holder.messageContainer.setBackgroundResource(R.drawable.my_message_bg)
            holder.userName.setTextColor(Color.BLACK)
        }

        holder.itemView.setOnLongClickListener {
            val currentUserId = FirebaseAuth.getInstance().currentUser?.uid
            if (message.userId == currentUserId || currentUserId == null) {
                showDeleteDialog(message)
            } else {
                Toast.makeText(holder.itemView.context, "Вы можете удалить только свои сообщения", Toast.LENGTH_SHORT).show()
            }
            true
        }
    }

    override fun getItemCount(): Int = messages.size

    private fun showDeleteDialog(message: Message) {
        AlertDialog.Builder(context)
            .setTitle("Удалить сообщение?")
            .setMessage("Это действие нельзя отменить.")
            .setPositiveButton("Удалить") { _, _ ->
                deleteMessage(message)
            }
            .setNegativeButton("Отмена", null)
            .show()
    }

    private fun deleteMessage(message: Message) {
        val database = FirebaseDatabase.getInstance().getReference("messages")
        database.orderByChild("text").equalTo(message.text).addListenerForSingleValueEvent(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                for (data in snapshot.children) {
                    data.ref.removeValue()
                }
            }
            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Ошибка удаления", error.toException())
            }
        })
    }
}
