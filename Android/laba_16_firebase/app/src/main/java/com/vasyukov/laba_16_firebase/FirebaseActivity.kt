package com.vasyukov.laba_16_firebase

import android.annotation.SuppressLint
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.google.android.gms.auth.api.identity.BeginSignInRequest
import com.google.android.gms.auth.api.identity.Identity
import com.google.android.gms.auth.api.identity.SignInClient
import com.google.android.gms.common.api.ApiException
import com.google.firebase.FirebaseApp
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.GoogleAuthProvider
import com.google.firebase.database.*
import com.google.firebase.messaging.FirebaseMessaging

class FirebaseActivity : AppCompatActivity() {
    private lateinit var auth: FirebaseAuth
    private lateinit var oneTapClient: SignInClient
    private lateinit var signInRequest: BeginSignInRequest
    private lateinit var database: DatabaseReference
    private val messages: MutableList<Message> = mutableListOf()
    private lateinit var adapter: MessageAdapter

    @SuppressLint("MissingInflatedId")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_firebase)

        if (FirebaseApp.getApps(this).isEmpty()) {
            Log.e("FCM", "Firebase не инициализирован!")
        } else {
            Log.d("FCM", "Firebase успешно инициализирован")
        }

        FirebaseMessaging.getInstance().token
            .addOnCompleteListener { task ->
                if (!task.isSuccessful) {
                    Log.w("FCM", "Ошибка получения токена", task.exception)
                    return@addOnCompleteListener
                }

                val token = task.result
                Log.d("FCM", "Текущий токен: $token")
            }

        auth = FirebaseAuth.getInstance()
        oneTapClient = Identity.getSignInClient(this)
        database = FirebaseDatabase.getInstance().getReference("messages")

        signInRequest = BeginSignInRequest.builder()
            .setGoogleIdTokenRequestOptions(
                BeginSignInRequest.GoogleIdTokenRequestOptions.builder()
                    .setSupported(true)
                    .setServerClientId("703415905245-tphic4til4fllnon4d03qrblru2clu6u.apps.googleusercontent.com")
                    .setFilterByAuthorizedAccounts(false)
                    .build()
            )
            .build()

        val signInButton = findViewById<Button>(R.id.sign_in_button)
        signInButton.setOnClickListener { signInWithGoogle() }

        val messageInput = findViewById<EditText>(R.id.message_input)
        val sendButton = findViewById<Button>(R.id.send_button)

        sendButton.setOnClickListener {
            val text = messageInput.text.toString().trim()
            if (text.isNotEmpty()) {
                val userName = auth.currentUser?.displayName ?: "Anon"
                val userId = auth.currentUser?.uid ?: ""
                val userPhoto = auth.currentUser?.photoUrl?.toString() ?: ""
                val message = Message(userName, text, userPhoto, userId)
                database.push().setValue(message)
                messageInput.text.clear()
            }
        }

        val messagesList = findViewById<RecyclerView>(R.id.messages_list)
        adapter = MessageAdapter(this, messages)

        messagesList.layoutManager = LinearLayoutManager(this)
        messagesList.adapter = adapter

        database.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                messages.clear()
                for (data in snapshot.children) {
                    val message = data.getValue(Message::class.java)
                    if (message != null) messages.add(message)
                }
                adapter.notifyDataSetChanged()
                messagesList.scrollToPosition(messages.size - 1)
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Ошибка загрузки сообщений", error.toException())
            }
        })
    }

    private fun signInWithGoogle() {
        oneTapClient.beginSignIn(signInRequest)
            .addOnSuccessListener { result ->
                val signInIntent = result.pendingIntent.intentSender
                if (signInIntent != null) {
                    startIntentSenderForResult(signInIntent, RC_SIGN_IN, null, 0, 0, 0, null)
                } else {
                    Log.e("SignIn", "Ошибка: signInIntent == null")
                }
            }
            .addOnFailureListener { e -> Log.e("SignIn", "Ошибка входа", e) }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == RC_SIGN_IN) {
            try {
                val credential = oneTapClient.getSignInCredentialFromIntent(data)
                val idToken = credential.googleIdToken
                if (idToken != null) {
                    val firebaseCredential = GoogleAuthProvider.getCredential(idToken, null)
                    auth.signInWithCredential(firebaseCredential)
                        .addOnSuccessListener {
                            Log.d("SignIn", "Успешный вход: ${it.user?.displayName}")
                        }
                        .addOnFailureListener { e -> Log.e("SignIn", "Ошибка аутентификации", e) }
                }
            } catch (e: ApiException) {
                Log.e("SignIn", "Ошибка API", e)
            }
        }
    }

    companion object {
        private const val RC_SIGN_IN = 123
    }
}
