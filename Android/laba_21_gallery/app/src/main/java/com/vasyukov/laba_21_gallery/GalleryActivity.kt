package com.vasyukov.laba_21_gallery

import android.Manifest
import android.annotation.SuppressLint
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.view.LayoutInflater
import android.view.View
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.github.chrisbanes.photoview.PhotoView
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.progressindicator.LinearProgressIndicator
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.File

class GalleryActivity : AppCompatActivity() {
    private lateinit var photoView: PhotoView
    private lateinit var positionTextView: TextView
    private lateinit var thumbnailsContainer: LinearLayout
    private lateinit var prevButton: FloatingActionButton
    private lateinit var nextButton: FloatingActionButton
    private lateinit var progressIndicator: LinearProgressIndicator
    private lateinit var thumbnailsRecyclerView: RecyclerView

    private lateinit var thumbnailAdapter: ThumbnailAdapter
    private var imageFiles = mutableListOf<File>()
    private var currentPosition = 0

    private val requestPermissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestPermission()
    ) { isGranted ->
        if (isGranted) {
            loadImages()
        } else {
            showPermissionDeniedDialog()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_gallery)
        initViews()
        checkPermissions()
    }

    @SuppressLint("ClickableViewAccessibility")
    private fun initViews() {
        photoView = findViewById(R.id.photoView)
        positionTextView = findViewById(R.id.positionTextView)
        thumbnailsContainer = findViewById(R.id.thumbnailsContainer)
        prevButton = findViewById(R.id.prevButton)
        nextButton = findViewById(R.id.nextButton)
        progressIndicator = findViewById(R.id.progressIndicator)

        prevButton.setOnClickListener { showPreviousImage() }
        nextButton.setOnClickListener { showNextImage() }

        thumbnailsRecyclerView = findViewById(R.id.thumbnailsRecyclerView)

        thumbnailsRecyclerView.layoutManager = LinearLayoutManager(this, RecyclerView.HORIZONTAL, false)
        thumbnailAdapter = ThumbnailAdapter(imageFiles, currentPosition) { index ->
            currentPosition = index
            updateCurrentImage()
        }
        thumbnailsRecyclerView.adapter = thumbnailAdapter
    }

    private fun checkPermissions() {
        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.READ_MEDIA_IMAGES
            ) == PackageManager.PERMISSION_GRANTED
        ) {
            loadImages()
        } else {
            requestPermissionLauncher.launch(Manifest.permission.READ_MEDIA_IMAGES)
        }
    }

    private fun loadImages() {
        val directory = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)
        imageFiles = directory.listFiles { file ->
            file.isFile && file.name.matches(".*\\.(jpg|jpeg|png|gif|webp)$".toRegex(RegexOption.IGNORE_CASE))
        }?.sortedBy { it.name }?.toMutableList() ?: mutableListOf()

        if (imageFiles.isEmpty()) {
            Toast.makeText(this, "Нет изображений в галерее", Toast.LENGTH_SHORT).show()
            return
        }

        updateCurrentImage()
    }

    @SuppressLint("SetTextI18n")
    private fun updateCurrentImage() {
        if (imageFiles.isEmpty()) return
        progressIndicator.visibility = View.VISIBLE

        val currentFile = imageFiles[currentPosition]
        lifecycleScope.launch(Dispatchers.IO) {
            try {
                val bitmap = BitmapFactory.decodeFile(currentFile.absolutePath)

                runOnUiThread {
                    if (bitmap != null) {
                        photoView.setImageBitmap(bitmap)
                        positionTextView.text = "${currentPosition + 1}/${imageFiles.size}"
                        updateThumbnails()
                    } else {
                        Toast.makeText(this@GalleryActivity, "Ошибка загрузки изображения", Toast.LENGTH_SHORT).show()
                    }
                }
            } catch (e: Exception) {
                runOnUiThread {
                    Toast.makeText(this@GalleryActivity, "Ошибка: ${e.localizedMessage}", Toast.LENGTH_SHORT).show()
                }
            } finally {
                runOnUiThread {
                    progressIndicator.postDelayed({
                        progressIndicator.visibility = View.GONE
                    }, 300)
                }
            }
        }
    }


    private fun loadThumbnails() {
        thumbnailsContainer.removeAllViews()

        val start = when (currentPosition) {
            0 -> 0
            imageFiles.size - 1 -> imageFiles.size - 3
            else -> currentPosition - 1
        }

        val visibleThumbnails = when (start) {
            0 -> imageFiles.subList(0, 2)
            imageFiles.size - 3 -> imageFiles.subList(imageFiles.size - 3, imageFiles.size)
            else -> imageFiles.subList(start, start + 3)
        }

        visibleThumbnails.forEachIndexed { index, file ->
            val thumbnailView = LayoutInflater.from(this)
                .inflate(R.layout.item_thumbnail, thumbnailsContainer, false)
            val thumbnailImage = thumbnailView.findViewById<ImageView>(R.id.thumbnailImage)
            val thumbnailFrame = thumbnailView.findViewById<View>(R.id.thumbnailFrame)

            val options = BitmapFactory.Options().apply {
                inSampleSize = 8
            }
            val bitmap = BitmapFactory.decodeFile(file.absolutePath, options)
            thumbnailImage.setImageBitmap(bitmap)

            if (index == 1) {
                thumbnailFrame.setBackgroundResource(R.drawable.thumbnail_border)
            } else {
                thumbnailFrame.setBackgroundResource(0)
            }

            thumbnailView.setOnClickListener {
                currentPosition = index + start - 1
                updateCurrentImage()
                loadThumbnails()
            }

            thumbnailsContainer.addView(thumbnailView)
        }
    }


    private fun showPreviousImage() {
        if (currentPosition > 0) {
            currentPosition--
            updateCurrentImage()
        }
    }

    private fun showNextImage() {
        if (currentPosition < imageFiles.size - 1) {
            currentPosition++
            updateCurrentImage()
        }
    }

    private fun showPermissionDeniedDialog() {
        AlertDialog.Builder(this)
            .setTitle("Требуется доступ")
            .setMessage("Для работы галереи необходимо предоставить доступ к хранилищу")
            .setPositiveButton("Настройки") { _, _ ->
                openAppSettings()
            }
            .setNegativeButton("Выйти") { _, _ ->
                finish()
            }
            .setCancelable(false)
            .show()
    }

    private fun openAppSettings() {
        val intent = Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS).apply {
            data = Uri.fromParts("package", packageName, null)
        }
        startActivity(intent)
    }

    private fun updateThumbnails() {
        val start = maxOf(0, currentPosition - 1)
        val end = minOf(imageFiles.size, currentPosition + 2)
        val visibleThumbnails = imageFiles.subList(start, end)

        thumbnailAdapter.updateImages(visibleThumbnails, currentPosition - start)
        thumbnailsRecyclerView.smoothScrollToPosition(currentPosition - start + 1)
    }
}