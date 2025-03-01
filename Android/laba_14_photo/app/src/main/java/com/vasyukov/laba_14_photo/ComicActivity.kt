package com.vasyukov.laba_14_photo

import android.app.Activity
import android.app.AlertDialog
import android.content.ContentValues
import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.view.View
import android.widget.EditText
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.io.File
import java.io.FileOutputStream
import java.io.OutputStream

class ComicActivity : AppCompatActivity() {

    private lateinit var imageViews: Array<ImageView>
    private lateinit var textViews: Array<TextView>
    private var currentImageIndex = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_comic)

        imageViews = arrayOf(
            findViewById(R.id.image1),
            findViewById(R.id.image2),
            findViewById(R.id.image3)
        )

        textViews = arrayOf(
            findViewById(R.id.text1),
            findViewById(R.id.text2),
            findViewById(R.id.text3)
        )

        for (i in imageViews.indices) {
            imageViews[i].setOnClickListener { openCamera(i) }
            textViews[i].setOnClickListener { showTextInputDialog(i) }
        }

        findViewById<View>(R.id.saveComicButton).setOnClickListener {
            val comicLayout = findViewById<View>(R.id.comicLayout)
            val uri = saveComicToGallery(comicLayout, this)

            if (uri != null) {
                Toast.makeText(this, "Комикс сохранён в галерею!", Toast.LENGTH_SHORT).show()
            } else {
                Toast.makeText(this, "Ошибка сохранения", Toast.LENGTH_SHORT).show()
            }
        }
    }

    private fun openCamera(index: Int) {
        currentImageIndex = index
        val intent = Intent(MediaStore.ACTION_IMAGE_CAPTURE)
        startActivityForResult(intent, REQUEST_IMAGE_CAPTURE)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == Activity.RESULT_OK) {
            val imageBitmap = data?.extras?.get("data") as Bitmap
            imageViews[currentImageIndex].setImageBitmap(imageBitmap)
        }
    }

    private fun showTextInputDialog(index: Int) {
        val editText = EditText(this)
        AlertDialog.Builder(this)
            .setTitle("Введите реплику")
            .setView(editText)
            .setPositiveButton("OK") { _, _ ->
                textViews[index].text = editText.text.toString()
            }
            .setNegativeButton("Отмена", null)
            .show()
    }

    private fun saveComicToGallery(view: View, context: Context): Uri? {
        val bitmap = Bitmap.createBitmap(view.width, view.height, Bitmap.Config.ARGB_8888)
        val canvas = Canvas(bitmap)
        canvas.drawColor(Color.WHITE)
        view.draw(canvas)

        val filename = "comic_${System.currentTimeMillis()}.png"
        var fos: OutputStream? = null
        var imageUri: Uri? = null

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            val contentValues = ContentValues().apply {
                put(MediaStore.Images.Media.DISPLAY_NAME, filename)
                put(MediaStore.Images.Media.MIME_TYPE, "image/png")
                put(MediaStore.Images.Media.RELATIVE_PATH, Environment.DIRECTORY_PICTURES + "/Comics")
            }
            val resolver = context.contentResolver
            imageUri = resolver.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, contentValues)
            fos = imageUri?.let { resolver.openOutputStream(it) }
        } else {
            val directory = File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), "Comics")
            if (!directory.exists()) directory.mkdirs()
            val file = File(directory, filename)
            fos = FileOutputStream(file)
            imageUri = Uri.fromFile(file)
        }

        fos?.use { bitmap.compress(Bitmap.CompressFormat.PNG, 100, it) }
        return imageUri
    }

    companion object {
        private const val REQUEST_IMAGE_CAPTURE = 1
    }
}