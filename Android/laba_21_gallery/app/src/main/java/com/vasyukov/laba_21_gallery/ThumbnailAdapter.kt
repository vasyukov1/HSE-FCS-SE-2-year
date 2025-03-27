package com.vasyukov.laba_21_gallery

import android.annotation.SuppressLint
import android.graphics.BitmapFactory
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import androidx.recyclerview.widget.RecyclerView
import java.io.File

class ThumbnailAdapter(
    private var images: List<File?>,
    private var currentIndex: Int,
    private val onClick: (Int) -> Unit
) : RecyclerView.Adapter<ThumbnailAdapter.ThumbnailViewHolder>() {

    class ThumbnailViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val thumbnailImage: ImageView = view.findViewById(R.id.thumbnailImage)
        val thumbnailFrame: View = view.findViewById(R.id.thumbnailFrame)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ThumbnailViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.item_thumbnail, parent, false)
        return ThumbnailViewHolder(view)
    }

    override fun onBindViewHolder(holder: ThumbnailViewHolder, position: Int) {
        val file = images[position]

        if (file != null) {
            val options = BitmapFactory.Options().apply { inSampleSize = 8 }
            val bitmap = BitmapFactory.decodeFile(file.absolutePath, options)
            holder.thumbnailImage.setImageBitmap(bitmap)
        } else {
            holder.thumbnailImage.setImageDrawable(null)
        }

        if (position == 1) {
            holder.thumbnailFrame.setBackgroundResource(R.drawable.thumbnail_border)
        } else {
            holder.thumbnailFrame.setBackgroundResource(0)
        }

        holder.itemView.setOnClickListener {
            if (file != null) onClick(position)
        }
    }

    override fun getItemCount(): Int = images.size

    @SuppressLint("NotifyDataSetChanged")
    fun updateImages(newImages: List<File>, newIndex: Int) {
        images = newImages
        currentIndex = newIndex
        notifyDataSetChanged()
    }
}
