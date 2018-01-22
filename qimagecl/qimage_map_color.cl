kernel void qimage_map_color(
   read_only  image2d_t src, // source image
   write_only image2d_t dst  // dest image
   )
{
    const int2 pos = { get_global_id(0), get_global_id(1) };
    uint4 pixel = read_imageui(src, pos);
    uint4 newPixel = (uint4)(127, 127, 127, 255);

    // comparison of two vectors is element-wise and results in a new vector
    // use all() to check for all elements
    if (all(pixel == (uint4)(255, 0, 0, 255)) == 0)
        write_imageui(dst, pos, newPixel);
    else
        write_imageui(dst, pos, pixel);
}
