#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

#include <glm/glm.hpp>


our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){

    //Generate an object from Texture2D class
    our::Texture2D* texture = new our::Texture2D();

    //TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    //Bind this texture that we will use
    texture->bind();
    
    //Specify texture parameters without storing data in texture just allocate memory for it
    //for depth buffer we need only 1 mip level
    //else we need to calculate how many mips to allocate 
    GLsizei levels=1;
    if(format != GL_DEPTH_COMPONENT24){
        levels = (GLsizei)glm::floor(glm::log2((float)glm::max(size[0], size[1]))) + 1;
    }

    //glTexStorage2D(target,mip levels,internalformat,width,height)
    /*here we chose the texture target or type to be 2D array of pixels
    levels is the number of mipmap levels to be generated
    internalformat is the sized internal format of the texture data
    like RGBA8 (means each pixel color is 4 channel and each channel is 8 bits)
    width and height are image size in pixels
    */
    glTexStorage2D(GL_TEXTURE_2D, levels, format, size[0], size[1]);

    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();

    //Bind the texture such that we upload the image data to its storage
    //TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"

    //Bind this texture that we will use
    texture->bind();

    //Specify texture parameters and data
    //glTexImage2D(target,level,internalformat,width,height,border must be 0,format,type,data)
    /*here we chose the texture target or type to be 2D array of pixels
    level: the level of specified number. Level 0 is the base image level.
    internalformat: thenumber of color components in the texture. 
    format: the format of the pixel data.
    width and height are image size in pixels
    type: the data type of the pixel data.
    data: pointer to data stored in texture
    */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixels);
    
    if(generate_mipmap){
        //glGenerateMipmap(target/type)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    stbi_image_free(pixels); //Free image data after uploading to GPU
    return texture;
}